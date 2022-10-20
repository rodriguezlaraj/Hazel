#pragma once

#include "RenderCommand.h"

namespace Hazel {

    //Deals with Meshes, and Scenes
	class Renderer
	{
	public:
        static void BeginScene(); //TODO: Add camera. lights, environments
        static void EndScene(); 

        //Submit can be overloaded to be able to receive mesh, raw vertex array, etc.
        //In the future, the mesh would have a material and the material is related to a shader.
        static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}

