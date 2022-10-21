#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {

    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        //Make sure that the shaders have the correct uniforms
        //Those uniforms are envirionment map, cube maps sample from, camera details ( projection, view matrix
        // //Where the camera is in world space
        //any calculations for lighting.
        //This is copying the data. We cannot use directly from the camera as that is expected to change sometimes before we render the frame.
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
        
    }

    void Renderer::EndScene()
    {
    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray)
    {
        shader->Bind();

        //This is the name of the variable we actually use in the shader program.
        shader->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}