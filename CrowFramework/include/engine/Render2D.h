#pragma once
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Shader;

namespace crow
{
    void Render2D_Init(GLuint quadVao);
    void Render2D_DrawRect(Shader& shader,
        const glm::vec2& offsetNdc,
        const glm::vec2& scaleNdc,
        const glm::vec3& color);
}
