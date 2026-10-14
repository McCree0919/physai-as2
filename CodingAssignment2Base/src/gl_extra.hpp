#pragma once

#include "gl_loader.hpp"

// gl_loader.hpp 里没有 glUniform1f：运行时向驱动查询它的地址
using Uniform1fFn = void (*)(int, float);
static Uniform1fFn glUniform1f = nullptr;

static bool load_gl_extra() {
    glUniform1f = reinterpret_cast<Uniform1fFn>(glfwGetProcAddress("glUniform1f"));
    return glUniform1f != nullptr;
}