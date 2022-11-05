#pragma once

#include "Hazel/Renderer/RenderCommand.h"
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Renderer/Shader.h"

namespace Hazel {

    //Deals with Meshes, and Scenes
	class Renderer
	{
	public:
        static void Init();
        static void Shutdown();
        static void OnWindowResize(uint32_t width, uint32_t height);

        static void BeginScene(OrthographicCamera& camera);; //TODO: Add camera. lights, environments
        static void EndScene(); 

        //Submit can be overloaded to be able to receive mesh, raw vertex array, etc.
        //In the future, the mesh would have a material and the material is related to a shader.
        static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));


        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };
        static SceneData* s_SceneData;
	};
}

