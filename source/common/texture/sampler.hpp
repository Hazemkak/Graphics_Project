#pragma once

#include <glad/gl.h>
#include <json/json.hpp>
#include <glm/vec4.hpp>

namespace our
{

    // This class defined an OpenGL sampler
    class Sampler
    {
        // The OpenGL object name of this sampler
        GLuint name;

    public:
        // This constructor creates an OpenGL sampler and saves its object name in the member variable "name"
        Sampler()
        {
            // TODO: (Req 5) Complete this function

            // Genrate 1 Sampler (OpenGL object) and assgin it to GLunit "name"
            glGenSamplers(1, &name);
        };

        // This deconstructor deletes the underlying OpenGL sampler
        ~Sampler()
        {
            // TODO: (Req 5) Complete this function

            // Deleting the Sampler object
            glDeleteSamplers(1, &name);
        }

        // This method binds this sampler to the given texture unit
        void bind(GLuint textureUnit) const
        {
            // TODO: (Req 5) Complete this function
            
            // Binding the texture unit to the sampler object
            glBindSampler(textureUnit, name);
        }

        // This static method ensures that no sampler is bound to the given texture unit
        static void unbind(GLuint textureUnit)
        {
            // TODO: (Req 5) Complete this function

            // Unbind the texture unit
            glBindSampler(textureUnit, 0);
        }

        // This function sets a sampler paramter where the value is of type "GLint"
        // This can be used to set the filtering and wrapping parameters
        void set(GLenum parameter, GLint value) const
        {
            // TODO: (Req 5) Complete this function

            // it sets the sampler paramters by passing the parameter name and value (Integer)
            glSamplerParameteri(name, parameter, value);
        }

        // This function sets a sampler paramter where the value is of type "GLfloat"
        // This can be used to set the "GL_TEXTURE_MAX_ANISOTROPY_EXT" parameter
        void set(GLenum parameter, GLfloat value) const
        {
            // TODO: (Req 5) Complete this function

            // it sets the sampler paramters by passing the parameter name and value (Float)
            glSamplerParameterf(name, parameter, value);
        }

        // This function sets a sampler paramter where the value is of type "GLfloat[4]"
        // This can be used to set the "GL_TEXTURE_BORDER_COLOR" parameter
        void set(GLenum parameter, glm::vec4 value) const
        {
            // it sets the sampler paramters by passing the parameter name and value (Float vector)
            glSamplerParameterfv(name, parameter, &(value.r));
        }

        // Given a json object, this function deserializes the sampler state
        void deserialize(const nlohmann::json &data);

        Sampler(const Sampler &) = delete;
        Sampler &operator=(const Sampler &) = delete;
    };

}