#include "hzpch.h"
#include "Renderer.h"

namespace Hazel {

    Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        //Make sure that the shaders have the correct uniforms
        //Those uniforms are envirionment map, cube maps sample from, camera details ( projection, view matrix
        // //Where the camera is in world space
        //any calculations for lighting.
        //This is copying the data. We cannot use directly from the camera as that is expected to change sometimes before we render the frame.
        s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
        
    }

    void Renderer::EndScene()
    {
    }

    //I have a mesh [VertexArray]
    //I have a material [Shader]
    //We can to apply a transform
    //This transform is for EACH object but The camera is for the whole scene
    void Renderer::Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();

        //This is the name of the variable we actually use in the shader program.
        shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->UploadUniformMat4("u_Transform", transform); shader->UploadUniformMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}