#pragma once

#include <memory>
#include "Hazel/Renderer/Buffer.h"

namespace Hazel {

    //Vertex Array is an encapsulation of a Vertex buffer + Vertex layout. The Vertex layout is a combination of all the attribute pointers for one vertex.
    // Vertex Buffer is the data which can have multiple elements (e.g. coordinates, color) which means multiple attribute pointers
    // In this function we set *ALL* the different attributes that the buffer has for each vertex. 
    class VertexArray
    {
    public:
        virtual ~VertexArray() {}

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
        virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

        virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

        static Ref<VertexArray> Create();
    };

}