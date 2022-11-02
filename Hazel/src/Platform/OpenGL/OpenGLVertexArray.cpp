#include "hzpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Hazel {

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
        case Hazel::ShaderDataType::Float:    return GL_FLOAT;
        case Hazel::ShaderDataType::Float2:   return GL_FLOAT;
        case Hazel::ShaderDataType::Float3:   return GL_FLOAT;
        case Hazel::ShaderDataType::Float4:   return GL_FLOAT;
        case Hazel::ShaderDataType::Mat3:     return GL_FLOAT;
        case Hazel::ShaderDataType::Mat4:     return GL_FLOAT;
        case Hazel::ShaderDataType::Int:      return GL_INT;
        case Hazel::ShaderDataType::Int2:     return GL_INT;
        case Hazel::ShaderDataType::Int3:     return GL_INT;
        case Hazel::ShaderDataType::Int4:     return GL_INT;
        case Hazel::ShaderDataType::Bool:     return GL_BOOL;
        }

        HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererID);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
    {
        HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind(); //Binds the buffer we are dealing with

        //Sets up the layout for this buffer
        const auto& layout = vertexBuffer->GetLayout();
        for (const auto& element : layout)
        {
            glEnableVertexAttribArray(m_VertexBufferIndex);
            //Index=0 Becuase we are setting up the first attribute (we only have one attribute)
            //Size=2 This is the component count. The specific vertex attribute is size 2.
            // Type= GL_FLOAT
            // GL_FALSE  Not normalized becasue they are floats. We dont need OPGL to normalize for us (that is, we dont need OPGL to make it between 0 and 1
            //Stride is the size of each vertex in bytes, In this case is 8 because we have 2 floats per vertex
            //Pointer is the offset to the particular attribute within each vertex. In this case is zero because we only have one attribute
            
            glVertexAttribPointer(m_VertexBufferIndex,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)element.Offset);
            m_VertexBufferIndex++;
        }
        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
    {
        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }

}
