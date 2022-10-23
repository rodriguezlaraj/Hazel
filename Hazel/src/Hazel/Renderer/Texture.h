#pragma once

#include <string>

#include "Hazel/Core.h"

namespace Hazel {

    class Texture
    {
    public:
        virtual ~Texture() = default;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void Bind(uint32_t slot = 0) const = 0;
    };

    class Texture2D : public Texture
    {
    public:
        //TODO: Load from memory, solid color textures, texture factory for gradients, grids, etc, Error texture that the engine
        //generates to show that the engine had a problem with the texture (e.g. blinking magenta)
        static Ref<Texture2D> Create(const std::string& path);
    };

}

