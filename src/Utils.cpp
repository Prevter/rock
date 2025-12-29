#include <rock/Utils.hpp>

#include <Geode/Result.hpp>

namespace rock::util {
    static geode::Result<GLuint> compileShader(GLenum type, char const* src) {
        GLuint shader = glCreateShader(type);

        GLchar const* sources[] = {
    #ifdef GEODE_IS_MOBILE
            (type == GL_VERTEX_SHADER
                ? "precision highp float;\n"
                : "#extension GL_OES_standard_derivatives : enable\n"
                  "precision mediump float;\n"),
    #endif
            "uniform mat4 CC_PMatrix;\n"
            "uniform mat4 CC_MVMatrix;\n"
            "uniform mat4 CC_MVPMatrix;\n"
            "uniform vec4 CC_Time;\n"
            "uniform vec4 CC_SinTime;\n"
            "uniform vec4 CC_CosTime;\n"
            "uniform vec4 CC_Random01;\n",
            src
        };

        glShaderSource(shader, std::size(sources), sources, nullptr);
        glCompileShader(shader);

        GLint compiled = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

            if (infoLen > 1) {
                std::string infoLog(infoLen, '\0');
                glGetShaderInfoLog(shader, infoLen, nullptr, infoLog.data());
                glDeleteShader(shader);
                switch (type) {
                    case GL_VERTEX_SHADER:
                        return geode::Err("Vertex shader compilation error: {}", infoLog);
                    case GL_FRAGMENT_SHADER:
                        return geode::Err("Fragment shader compilation error: {}", infoLog);
                    default:
                        return geode::Err("Shader compilation error: {}", infoLog);
                }
            }

            glDeleteShader(shader);
            return geode::Err("Shader compilation failed with unknown error");
        }

        return geode::Ok(shader);
    }

    static geode::Result<GLuint> createShaderProgram(char const* vertShader, char const* fragShader) {
        GEODE_UNWRAP_INTO(auto vert, compileShader(GL_VERTEX_SHADER, vertShader));
        GEODE_UNWRAP_INTO(auto frag, compileShader(GL_FRAGMENT_SHADER, fragShader));

        GLuint program = glCreateProgram();
        glAttachShader(program, vert);
        glAttachShader(program, frag);

        glBindAttribLocation(program, cocos2d::kCCVertexAttrib_Position, "a_position");
        glBindAttribLocation(program, cocos2d::kCCVertexAttrib_Color, "a_color");
        glBindAttribLocation(program, cocos2d::kCCVertexAttrib_TexCoords, "a_texCoord");
        glBindAttribLocation(program, 3, "a_texCoord2");

        glLinkProgram(program);

        glDeleteShader(vert);
        glDeleteShader(frag);

        GLint linked = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linked);
        if (!linked) {
            GLint infoLen = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

            if (infoLen > 1) {
                std::string infoLog(infoLen, '\0');
                glGetProgramInfoLog(program, infoLen, nullptr, infoLog.data());
                glDeleteProgram(program);
                return geode::Err("Program linking error: {}", infoLog);
            }

            glDeleteProgram(program);
            return geode::Err("Program linking failed with unknown error");
        }

        return geode::Ok(program);
    }

    cocos2d::CCGLProgram* getShaderProgram(
        char const* name,
        char const* vertShader,
        char const* fragShader
    ) {
        auto cache = cocos2d::CCShaderCache::sharedShaderCache();
        auto program = cache->programForKey(name);
        if (program) {
            return program;
        }

        auto result = createShaderProgram(vertShader, fragShader);
        if (result.isErr()) {
            geode::log::error("{}", result.unwrapErr());
            return nullptr;
        }

        // manually create CCGLProgram
        program = new cocos2d::CCGLProgram();
        program->m_uProgram = result.unwrap();
        program->m_pHashForUniforms = nullptr;
        program->m_uVertShader = 0;
        program->m_uFragShader = 0;

        program->updateUniforms();
        cache->addProgram(program, name);
        program->release();

        return program;
    }
} // namespace rock::util
