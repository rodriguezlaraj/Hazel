#include "hzpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Hazel {

    void OpenGLRendererAPI::Init()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        ///*
         //glEnable(GL_BLEND) - glDisable(GL_BLEND)
         //glBlendFunc(src, dest)
         //    src = how the src RGBA factor is computed. Default is GL_ONE
         //    dest = how the dest RGBA factor is computer. Default is GL_ZERO
         //glBlendEquation(mode)
         //    mode = how we combine the src and dest colors
         //    Default value is GL_FUNC_ADD
         //By default
         //    1 + 0 = 1 (Use the source value)
         //src alpha = 0
         //dest = 1 - 0 = 0
         //R = (r source x alpha source) + (r dest x (1-alpha source)) = r dest

         //*/
         //glEnable(GL_BLEND); //Activate blending function
         //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//Activate blending function
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRendererAPI::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRendererAPI::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
    {

        //glEnable(GL_DEPTH_TEST);// Enable depth test to make sure further fragments do not block closer fragments
        //glDepthFunc(GL_LESS);// Enable depth test to make sure further fragments do not block closer fragments // Accept fragment if it closer to the camera than the former one


 


        //TODO: It is assumed that we always have GL_UNSIGNED_INT and GL_TRIANGLES
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);//6 Elements Indices drawed,  nullptr because it is bounded. otherwise woudl be a pointer to that index
    }

}
