#pragma once
#include <glad/gl.h>
#include "vertex.hpp"

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
    public:

        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements)
        {
            //TODO: (Req 1) Write this function
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc

            //Setting elementCount that will be used later with size of elements vector.
            this->elementCount = elements.size();

            
            //Create arrays instead of vectors to be able to pass it to gl functions .
            Vertex * VertArr = new Vertex[vertices.size()];
            unsigned int* elemArr = new unsigned int[elementCount];
            for(int i= 0;i < vertices.size(); i++)
                VertArr[i] = vertices[i];
            
            for(int i= 0;i < elements.size(); i++)
                elemArr[i] = elements[i];


            //Binding vertex buffer object to set its data with vertices' data.
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), VertArr , GL_STATIC_DRAW);

            
            //Reading vertices' data from buffer to vertex array by reading:
            //Firstly, Positions of float values in the created array (VerArr) which are not normalized.

            glGenVertexArrays(1,&VAO);
            glBindVertexArray(VAO);
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            glVertexAttribPointer(ATTRIB_LOC_POSITION,3,GL_FLOAT,false,sizeof(Vertex),(void*) 0);
    

            //Secondly, Bytes of Colors in the created array (VerArr) which should be normalized as their values are 0-255,
            //but should be 0-1.0 
            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            glVertexAttribPointer(ATTRIB_LOC_COLOR,4, GL_UNSIGNED_BYTE ,true,sizeof(Vertex),(void*) offsetof(Vertex,color));

            
            //Generate Buffer for the element buffer object.
            glGenBuffers(1, &EBO);
            //Binding it to fill its data.
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
            //Setting indices of vertices that will be drawn in the Element Bufffer Object.
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size()*sizeof(unsigned int), elemArr , GL_STATIC_DRAW);


            //Texture
            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD,2, GL_FLOAT ,false ,sizeof(Vertex),(void*) offsetof(Vertex,tex_coord));

            //Normal
            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            glVertexAttribPointer(ATTRIB_LOC_NORMAL,3, GL_FLOAT ,false ,sizeof(Vertex),(void*) offsetof(Vertex,normal));

        }

        // this function should render the mesh
        void draw() 
        {
            // //TODO: (Req 1) Write this function
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, this->elementCount, GL_UNSIGNED_INT, (void*)0);
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            //TODO: (Req 1) Write this function
            glDeleteVertexArrays(1,&VAO);
            glDeleteBuffers(1,&VBO);
            glDeleteBuffers(1,&EBO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}