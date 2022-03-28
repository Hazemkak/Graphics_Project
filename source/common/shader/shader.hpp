#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle
        GLuint program;

    public:
        void create();
        void destroy();

        ShaderProgram(){ program = 0; }
        ~ShaderProgram(){ destroy(); }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            //TODO: call opengl to use the program identified by this->program

            /*glUseProgram --> Specify which program to use when draw 
             parameter: the program to use 
             */
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            //TODO: call opengl to get the uniform location for the uniform defined by name from this->program


            /*glGetUniformLocation --> to get the location of the uniform variable that will be sent from the main to the fragment shader (pointer to this uniform)
            1st parameter: the program used in drowing 
            2nd parameter: the name of the uniform variable 
            */
            return glGetUniformLocation(this->program,name.c_str());
        }

        void set(const std::string &uniform, GLfloat value) {
            //TODO: call opengl to set the value to the uniform defined by name


            /*glUniform1f --> to set the value to the uniform in case that its type is float 
            1st parameter: Location to that uniform
            2nd parameter: pointer to it 
            */
            glUniform1f(getUniformLocation(uniform),(float)value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            //TODO: call opengl to set the value to the uniform defined by name

            /*glUniform2fv --> to set the value to the uniform in case that its type is vec2 of floats 
            1st parameter: Location to that uniform
            2nd parameter: pointer to the start position of the vector 
            */
            glUniform2fv(getUniformLocation(uniform),1,&value[0]);
        }

        void set(const std::string &uniform, glm::vec3 value) {
            //TODO: call opengl to set the value to the uniform defined by name


            /*glUniform3fv --> to set the value to the uniform in case that its type is vec3 of floats  
            1st parameter: Location to that uniform
            2nd parameter: pointer to the start position of the vector  
            */
            glUniform3fv(getUniformLocation(uniform),1,&value[0]);
        }

        void set(const std::string &uniform, glm::vec4 value) {
            //TODO: call opengl to set the value to the uniform defined by name

            /*glUniform4fv --> to set the value to the uniform in case that its type is vec4 of floats  
            1st parameter: Location to that uniform
            2nd parameter: pointer to the start position of the vector  
            */
            glUniform4fv(getUniformLocation(uniform),1,&value[0]);
        }


        //TODO: Delete the copy constructor and assignment operator
        //Question: Why do we do this? Hint: Look at the deconstructor
        /* the existance of the copy constructor is wrong : as it will give the openGL objet 
        a new name (another pointer to this object) not copy the object value itself 
        so when the deconstructor called for each pointer the first one will be destroied 
        but the second one won't find an object to destroy .
        also we should never make a copy for the shader it should be a unique source.
        */
    };

}

#endif