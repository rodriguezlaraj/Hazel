#pragma once

#include <string>


namespace Hazel {

	class Shader
	{
	public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        //This is a temporary solution while a material class is created. 
        //We have to offer the create method becuase we cannot use 'new' as it is an abstract class.
        static Shader* Create(const std::string& vertexSrc, const std::string& fragmentSrc);

	};

}
