#pragma once

#include "Hazel/Renderer/VertexArray.h"

namespace Hazel {

    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void Bind() const override;
        virtual void Unbind() const override;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        

        //Maybe it is possible to add multiple index buffers.
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
    private:
        uint32_t m_RendererID;
        uint32_t m_VertexBufferIndex = 0; //Index to allow to add multiple vertex buffers 
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer> m_IndexBuffer;
    };

}

