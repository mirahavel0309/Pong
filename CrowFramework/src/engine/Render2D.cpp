#include <engine/Render2D.h>
#include <engine/Shader.h>

namespace crow
{
    static GLuint g_quadVao = 0;

    void Render2D_Init(GLuint quadVao)
    {
        g_quadVao = quadVao;
    }

    void Render2D_DrawRect(Shader& shader,
        const glm::vec2& offsetNdc,
        const glm::vec2& scaleNdc,
        const glm::vec3& color)
    {
        if (g_quadVao == 0) return;

        shader.Use();
        shader.SetVec3("uColor", color.x, color.y, color.z);
        shader.SetVec2("uScale", scaleNdc.x, scaleNdc.y);
        shader.SetVec2("uOffset", offsetNdc.x, offsetNdc.y);

        glBindVertexArray(g_quadVao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }
}
