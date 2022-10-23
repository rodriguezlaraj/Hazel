#include "hzpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Hazel {

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
        : m_Path(path)
    {
        int width, height, channels; //BPP= Bits Per Pixel

        //Flips the image vertically
        //OGL expects bottom left to be the zero
        //Typically when loading images they come from top to bottom
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);//4 channels RGBA, using 0 retains the format of the image
        HZ_CORE_ASSERT(data, "Failed to load image!");
        m_Width = width;
        m_Height = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4) //This has an alpha channel. 
        {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3)//This is the standard RGB
        {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }

        HZ_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);
        //If this is not specified it will probably show just black
        //TODO: GL_LINEAR, GL_NEAREST needs to be open in the API to be able to define which type of filtering we use
        //GL_LINEAR interpolates the pixels. while linear grabs the closest pixel to decide the color
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //Texture rendered on geometry smaller than the texture. This is good for images
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//Texture rendered on geometry larger than the texture. 

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
        //Level 0, GL_RGBA Internal, format we want to use, not the format we supply 
        //GL_RGBA Format in which we supply the data, GL_UNSIGNED_BYTE Type of that supplied, m_LocalBuffer= Actual data

        //Level 0 because this is the index. We define one level above.
        // 0, 0 This is an offset. We could use a texture that we already uploaded.
        //GL_RGB  could be validated with channels above.

        //	This can be kept if necessary
        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        glBindTextureUnit(slot, m_RendererID);
    }
}
