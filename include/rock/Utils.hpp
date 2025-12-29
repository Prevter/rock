#pragma once
#include <cocos2d.h>

namespace rock::util {
    cocos2d::CCGLProgram* getShaderProgram(
        char const* name,
        char const* vertShader,
        char const* fragShader
    );
} // namespace rock::util