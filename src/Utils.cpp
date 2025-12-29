#include <rock/Utils.hpp>

namespace rock::util {
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

        program = new cocos2d::CCGLProgram();
        program->initWithVertexShaderByteArray(vertShader, fragShader);
        program->addAttribute("a_position", cocos2d::kCCVertexAttrib_Position);
        program->addAttribute("a_color", cocos2d::kCCVertexAttrib_Color);
        program->addAttribute("a_texCoord", cocos2d::kCCVertexAttrib_TexCoords);
        program->addAttribute("a_texCoord2", 3);

        if (!program->link()) {
            geode::log::error("Failed to link shader");
            program->release();
            return nullptr;
        }

        program->updateUniforms();
        cache->addProgram(program, name);
        program->release();

        return program;
    }
} // namespace rock::util