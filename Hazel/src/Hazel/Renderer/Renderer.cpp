#include "hzpch.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/Renderer2D.h"

namespace Hazel {

    Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

    void Renderer::Init()
    {
        HZ_PROFILE_FUNCTION();
        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::Shutdown()
    {
        Renderer2D::Shutdown();
    }


    //TODO: Shutdown needs to be created, similar to Renderer2D.

    //The renderer needs to handle when there is a window resize by ultimately triggering a call to our Graphics API (Vulkan, OpenGL, etc) "Set View Port"
    //We can render multiple view ports in a single frame. This allows us to see different cameras in a single frame. 
    //TODO: Frame buffers will be handled separately . Frame buffer pool. The application needs toknow about all frame buffers
    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }

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
    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
    {
        shader->Bind();

        //This is the name of the variable we actually use in the shader program.
        //We know at this point that this is an OpenGLShader becuase that is the only type of shader we currently have. 
        //In the future, this will not be required and will be done differently. UploadUniformMat4 is OGL function and will not be here anyway.
        //This is done to ahve the system working without writing unnecessary work for abstraction.
        shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
        shader->SetMat4("u_Transform", transform);

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}