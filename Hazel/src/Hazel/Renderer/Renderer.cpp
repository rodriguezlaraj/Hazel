#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {

    void Renderer::BeginScene()
    {
        //Make sure that the shaders have the correct uniforms
        //Those uniforms are envirionment map, cube maps sample from, camera details ( projection, view matrix
        // //Where the camera is in world space
        //any calculations for lighting.
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
    {
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}