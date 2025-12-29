#include <rock/RoundedRect.hpp>
#include <rock/Utils.hpp>

namespace rock {
    namespace shaders {
        constexpr auto ROUNDED_RECT_VERT_SHADER = R"(attribute vec4 a_position;
attribute vec4 a_color;
attribute vec2 a_texCoord;

#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
varying lowp vec2 v_uv;
#else
varying vec4 v_fragmentColor;
varying vec2 v_uv;
#endif

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    v_fragmentColor = a_color;
    v_uv = a_texCoord;
})";

        constexpr auto ROUNDED_RECT_FRAG_SHADER = R"(#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_uv;
uniform vec4 u_radii;
uniform vec2 u_size;

float sdRoundRectFast(vec2 uv, vec2 size, float r) {
    r = min(r, min(size.x, size.y) * 0.5);
    vec2 halfSize = size * 0.5;
    vec2 p = (uv - 0.5) * size;
    vec2 corner = halfSize - vec2(r);
    vec2 q = abs(p) - corner;
    vec2 outside = max(q, 0.0);
    float outsideLen = length(outside);
    float inside = min(max(q.x, q.y), 0.0);
    return outsideLen + inside - r;
}

float sdRoundRect(vec2 uv, vec2 size, vec4 radii) {
    vec2 halfSize = size * 0.5;
    vec2 p = (uv - 0.5) * size;

    float r;
    if (p.x >= 0.0) {
        if (p.y >= 0.0) {
            r = radii.y; // top-right
        } else {
            r = radii.z; // bottom-right
        }
    } else {
        if (p.y >= 0.0) {
            r = radii.x; // top-left
        } else {
            r = radii.w; // bottom-left
        }
    }

    float rc = min(r, min(halfSize.x, halfSize.y));

    vec2 corner = halfSize - vec2(rc);
    vec2 q = abs(p) - corner;

    vec2 outside = max(q, 0.0);
    float outsideLen = length(outside);
    float inside = min(max(q.x, q.y), 0.0);

    return outsideLen + inside - rc;
}

void main() {
    float dist;
    if (all(equal(u_radii.xyzw, u_radii.xxxx))) {
        dist = sdRoundRectFast(v_uv, u_size, u_radii.x);
    } else {
        dist = sdRoundRect(v_uv, u_size, u_radii);
    }
    float aa = fwidth(dist);
    float alpha = 1.0 - smoothstep(-aa, aa, dist);
    if (alpha < 0.01) discard;
    gl_FragColor = vec4(v_fragmentColor.rgb, v_fragmentColor.a * alpha);
})";

        constexpr auto ROUNDED_SPRITE_VERT_SHADER = R"(attribute vec4 a_position;
attribute vec4 a_color;
attribute vec2 a_texCoord;
attribute vec2 a_texCoord2;

#ifdef GL_ES
varying lowp vec4 v_fragmentColor;
varying lowp vec2 v_uv;
varying lowp vec2 v_localUV;
#else
varying vec4 v_fragmentColor;
varying vec2 v_uv;
varying vec2 v_localUV;
#endif

void main() {
    gl_Position = CC_MVPMatrix * a_position;
    v_fragmentColor = a_color;
    v_uv = a_texCoord;
    v_localUV = a_texCoord2;
})";

        constexpr auto ROUNDED_SPRITE_FRAG_SHADER = R"(#ifdef GL_ES
precision lowp float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_uv;
varying vec2 v_localUV;
uniform vec4 u_radii;
uniform vec2 u_size;
uniform sampler2D CC_Texture0;

float sdRoundRectFast(vec2 uv, vec2 size, float r) {
    r = min(r, min(size.x, size.y) * 0.5);
    vec2 halfSize = size * 0.5;
    vec2 p = (uv - 0.5) * size;
    vec2 corner = halfSize - vec2(r);
    vec2 q = abs(p) - corner;
    vec2 outside = max(q, 0.0);
    float outsideLen = length(outside);
    float inside = min(max(q.x, q.y), 0.0);
    return outsideLen + inside - r;
}

float sdRoundRect(vec2 uv, vec2 size, vec4 radii) {
    vec2 halfSize = size * 0.5;
    vec2 p = (uv - 0.5) * size;

    float r;
    if (p.x >= 0.0) {
        if (p.y >= 0.0) {
            r = radii.y; // top-right
        } else {
            r = radii.z; // bottom-right
        }
    } else {
        if (p.y >= 0.0) {
            r = radii.x; // top-left
        } else {
            r = radii.w; // bottom-left
        }
    }

    float rc = min(r, min(halfSize.x, halfSize.y));

    vec2 corner = halfSize - vec2(rc);
    vec2 q = abs(p) - corner;

    vec2 outside = max(q, 0.0);
    float outsideLen = length(outside);
    float inside = min(max(q.x, q.y), 0.0);

    return outsideLen + inside - rc;
}

void main() {
    float dist;
    if (all(equal(u_radii.xyzw, u_radii.xxxx))) {
        dist = sdRoundRectFast(v_localUV, u_size, u_radii.x);
    } else {
        dist = sdRoundRect(v_localUV, u_size, u_radii);
    }
    float aa = fwidth(dist);
    float mask = 1.0 - smoothstep(-aa, aa, dist);
    if (mask < 0.01) discard;
    vec4 texColor = texture2D(CC_Texture0, v_uv);
    vec3 rgb = texColor.rgb * v_fragmentColor.rgb * mask;
    float a = texColor.a * v_fragmentColor.a * mask;
    gl_FragColor = vec4(rgb, a);
})";
    }

    RoundedRect::~RoundedRect() = default;

    RoundedRect* RoundedRect::create(
        cocos2d::ccColor4B color,
        float radius,
        cocos2d::CCSize const& size
    ) {
        return create(color, Radii::uniform(radius), size);
    }

    RoundedRect* RoundedRect::create(cocos2d::ccColor4B color, float radius) {
        return create(color, Radii::uniform(radius), cocos2d::CCDirector::get()->getWinSize());
    }

    RoundedRect* RoundedRect::create(
        cocos2d::ccColor4B color,
        Radii const& radii,
        cocos2d::CCSize const& size
    ) {
        auto ret = new RoundedRect();
        if (ret->init(color, radii, size)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    RoundedRect* RoundedRect::create(cocos2d::ccColor4B color, Radii const& radii) {
        return create(color, radii, cocos2d::CCDirector::get()->getWinSize());
    }

    void RoundedRect::setRadii(Radii const& radii) {
        m_radii = radii;
    }

    void RoundedRect::setRadius(float radius) {
        m_radii = Radii::uniform(radius);
    }

    Radii const& RoundedRect::getRadii() const {
        return m_radii;
    }

    bool RoundedRect::init(cocos2d::ccColor4B color, Radii const& radii, cocos2d::CCSize const& size) {
        if (!CCNodeRGBA::init()) {
            return false;
        }

        this->setAnchorPoint({0.5f, 0.5f});
        this->setContentSize(size);
        this->setColor({color.r, color.g, color.b});
        this->setOpacity(color.a);
        m_radii = radii;

        this->setShaderProgram(util::getShaderProgram(
            "rock_rounded_rect",
            shaders::ROUNDED_RECT_VERT_SHADER,
            shaders::ROUNDED_RECT_FRAG_SHADER
        ));

        m_radiiLoc = m_pShaderProgram->getUniformLocationForName("u_radii");
        m_sizeLoc = m_pShaderProgram->getUniformLocationForName("u_size");

        return true;
    }

    void RoundedRect::draw() {
        if (!m_pShaderProgram) return;

        ccGLEnable(m_eGLServerState);
        m_pShaderProgram->use();
        m_pShaderProgram->setUniformsForBuiltins();

        cocos2d::ccGLBlendFunc(m_blendFunc.src, m_blendFunc.dst);
        auto size = this->getContentSize();

        m_pShaderProgram->setUniformLocationWith4f(
            m_radiiLoc,
            m_radii.topLeft,
            m_radii.topRight,
            m_radii.bottomRight,
            m_radii.bottomLeft
        );

        m_pShaderProgram->setUniformLocationWith2f(
            m_sizeLoc,
            size.width,
            size.height
        );

        cocos2d::ccGLEnableVertexAttribs(cocos2d::kCCVertexAttribFlag_PosColorTex);
        constexpr std::array<cocos2d::ccVertex2F, 4> texCoords = {{
            {0.f, 0.f},
            {1.f, 0.f},
            {0.f, 1.f},
            {1.f, 1.f}
        }};

        glVertexAttribPointer(
            cocos2d::kCCVertexAttrib_Position,
            2, GL_FLOAT, GL_FALSE,
            0, m_squareVertices.data()
        );
        glVertexAttribPointer(
            cocos2d::kCCVertexAttrib_Color,
            4, GL_FLOAT, GL_FALSE,
            0, m_squareColors.data()
        );
        glVertexAttribPointer(
            cocos2d::kCCVertexAttrib_TexCoords,
            2, GL_FLOAT, GL_FALSE,
            0, texCoords.data()
        );

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void RoundedRect::updateColor() {
        for (size_t i = 0; i < 4; ++i) {
            m_squareColors[i] = {
                _displayedColor.r / 255.0f,
                _displayedColor.g / 255.0f,
                _displayedColor.b / 255.0f,
                _displayedOpacity / 255.0f,
            };
        }
    }

    void RoundedRect::updateVertices() {
        m_squareVertices[1].x = m_obContentSize.width;
        m_squareVertices[2].y = m_obContentSize.height;
        m_squareVertices[3].x = m_obContentSize.width;
        m_squareVertices[3].y = m_obContentSize.height;
    }

    cocos2d::ccBlendFunc RoundedRect::getBlendFunc() {
        return m_blendFunc;
    }

    void RoundedRect::setBlendFunc(cocos2d::ccBlendFunc blendFunc) {
        m_blendFunc = blendFunc;
    }

    void RoundedRect::setColor(cocos2d::ccColor3B const& color) {
        CCNodeRGBA::setColor(color);
        this->updateColor();
    }

    void RoundedRect::setOpacity(GLubyte opacity) {
        CCNodeRGBA::setOpacity(opacity);
        this->updateColor();
    }

    void RoundedRect::setContentSize(cocos2d::CCSize const& contentSize) {
        CCNode::setContentSize(contentSize);
        this->updateVertices();
    }

    RoundedSprite::~RoundedSprite() = default;

    RoundedSprite* RoundedSprite::create(char const* filename, Radii const& radii) {
        auto ret = new RoundedSprite();
        if (ret->initWithFile(filename, radii)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    RoundedSprite* RoundedSprite::createWithSpriteFrame(cocos2d::CCSpriteFrame* spriteFrame, Radii const& radii) {
        auto ret = new RoundedSprite();
        if (ret->initWithSpriteFrame(spriteFrame, radii)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    RoundedSprite* RoundedSprite::createWithSpriteFrameName(char const* spriteFrameName, Radii const& radii) {
        auto ret = new RoundedSprite();
        if (ret->initWithSpriteFrameName(spriteFrameName, radii)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    RoundedSprite* RoundedSprite::createWithTexture(cocos2d::CCTexture2D* texture, Radii const& radii) {
        auto ret = new RoundedSprite();
        if (ret->initWithTexture(texture, radii)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    RoundedSprite* RoundedSprite::create(char const* filename, float radius) {
        return create(
            filename,
            Radii::uniform(radius)
        );
    }

    RoundedSprite* RoundedSprite::createWithSpriteFrame(cocos2d::CCSpriteFrame* spriteFrame, float radius) {
        return createWithSpriteFrame(
            spriteFrame,
            Radii::uniform(radius)
        );
    }

    RoundedSprite* RoundedSprite::createWithSpriteFrameName(char const* spriteFrameName, float radius) {
        return createWithSpriteFrameName(
            spriteFrameName,
            Radii::uniform(radius)
        );
    }

    RoundedSprite* RoundedSprite::createWithTexture(cocos2d::CCTexture2D* texture, float radius) {
        return createWithTexture(texture, Radii::uniform(radius));
    }

    bool RoundedSprite::initWithTexture(cocos2d::CCTexture2D* texture, Radii const& radii) {
        if (!CCSprite::initWithTexture(texture)) {
            return false;
        }

        return this->init(radii);
    }

    bool RoundedSprite::initWithFile(char const* filename, Radii const& radii) {
        if (!CCSprite::initWithFile(filename)) {
            return false;
        }

        return this->init(radii);
    }

    bool RoundedSprite::initWithSpriteFrame(cocos2d::CCSpriteFrame* spriteFrame, Radii const& radii) {
        if (!CCSprite::initWithSpriteFrame(spriteFrame)) {
            return false;
        }

        return this->init(radii);
    }

    bool RoundedSprite::initWithSpriteFrameName(char const* spriteFrameName, Radii const& radii) {
        if (!CCSprite::initWithSpriteFrameName(spriteFrameName)) {
            return false;
        }

        return this->init(radii);
    }

    bool RoundedSprite::init(Radii const& radii) {
        m_radii = radii;
        this->setShaderProgram(util::getShaderProgram(
            "rock_rounded_sprite",
            shaders::ROUNDED_SPRITE_VERT_SHADER,
            shaders::ROUNDED_SPRITE_FRAG_SHADER
        ));

        m_radiiLoc = m_pShaderProgram->getUniformLocationForName("u_radii");
        m_sizeLoc = m_pShaderProgram->getUniformLocationForName("u_size");

        return true;
    }

    void RoundedSprite::draw() {
        if (!m_pShaderProgram) return;

        ccGLEnable(m_eGLServerState);
        m_pShaderProgram->use();
        m_pShaderProgram->setUniformsForBuiltins();

        cocos2d::ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
        auto size = this->getContentSize();

        m_pShaderProgram->setUniformLocationWith4f(
            m_radiiLoc,
            m_radii.topRight,
            m_radii.bottomRight,
            m_radii.bottomLeft,
            m_radii.topLeft
        );

        m_pShaderProgram->setUniformLocationWith2f(
            m_sizeLoc,
            size.width,
            size.height
        );

        cocos2d::ccGLBindTexture2D(m_pobTexture->getName());
        cocos2d::ccGLEnableVertexAttribs(cocos2d::kCCVertexAttribFlag_PosColorTex);

        constexpr std::array<cocos2d::ccVertex2F, 4> localUV = {{
            {0.f, 0.f},
            {1.f, 0.f},
            {0.f, 1.f},
            {1.f, 1.f}
        }};

        uintptr_t offset = reinterpret_cast<uintptr_t>(&m_sQuad);
        glVertexAttribPointer(
            cocos2d::kCCVertexAttrib_Position,
            2, GL_FLOAT, GL_FALSE,
            sizeof(cocos2d::ccV3F_C4B_T2F),
            reinterpret_cast<void*>(offset + offsetof(cocos2d::ccV3F_C4B_T2F, vertices))
        );
        glVertexAttribPointer(
            cocos2d::kCCVertexAttrib_Color,
            4, GL_UNSIGNED_BYTE, GL_TRUE,
            sizeof(cocos2d::ccV3F_C4B_T2F),
            reinterpret_cast<void*>(offset + offsetof(cocos2d::ccV3F_C4B_T2F, colors))
        );
        glVertexAttribPointer(
            cocos2d::kCCVertexAttrib_TexCoords,
            2, GL_FLOAT, GL_FALSE,
            sizeof(cocos2d::ccV3F_C4B_T2F),
            reinterpret_cast<void*>(offset + offsetof(cocos2d::ccV3F_C4B_T2F, texCoords))
        );

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(
            3,
            2, GL_FLOAT, GL_FALSE,
            0,
            localUV.data()
        );

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glDisableVertexAttribArray(3);
    }

    void RoundedSprite::setRadii(Radii const& radii) {
        m_radii = radii;
    }

    void RoundedSprite::setRadius(float radius) {
        m_radii = Radii::uniform(radius);
    }

    Radii const& RoundedSprite::getRadii() const {
        return m_radii;
    }
} // namespace rock
