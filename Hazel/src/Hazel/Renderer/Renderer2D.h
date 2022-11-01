#pragma once

#include "OrthographicCamera.h"

#include "Texture.h"

namespace Hazel {

    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();//In a complex renderer, it can probably update the vertex data to the gpu in a batch renderer

        // Primitives
        //vec 2 and vec3 are used becuase we can use that z coordinate to sort in the z axis. Z can be used for sorted layers.
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);
    };

}
