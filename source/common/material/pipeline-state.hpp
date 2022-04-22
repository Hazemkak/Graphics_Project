#pragma once

#include <glad/gl.h>
#include <glm/vec4.hpp>
#include <json/json.hpp>

namespace our {
    // There are some options in the render pipeline that we cannot control via shaders
    // such as blending, depth testing and so on
    // Since each material could require different options (e.g. transparent materials usually use blending),
    // we will encapsulate all these options into a single structure that will also be responsible for configuring OpenGL's pipeline
    struct PipelineState {
        // This set of pipeline options specifies whether face culling will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum culledFace = GL_BACK;
            GLenum frontFace = GL_CCW;
        } faceCulling;

        // This set of pipeline options specifies whether depth testing will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum function = GL_LEQUAL;
        } depthTesting;

        // This set of pipeline options specifies whether blending will be used or not and how it will be configured
        struct {
            bool enabled = false;
            GLenum equation = GL_FUNC_ADD;
            GLenum sourceFactor = GL_SRC_ALPHA;
            GLenum destinationFactor = GL_ONE_MINUS_SRC_ALPHA;
            glm::vec4 constantColor = {0, 0, 0, 0};
        } blending;

        // These options specify the color and depth mask which can be used to
        // prevent the rendering/clearing from modifying certain channels of certain targets in the framebuffer
        glm::bvec4 colorMask = {true, true, true, true}; // To know how to use it, check glColorMask
        bool depthMask = true; // To know how to use it, check glDepthMask


        // This function should set the OpenGL options to the values specified by this structure
        // For example, if faceCulling.enabled is true, you should call glEnable(GL_CULL_FACE), otherwise, you should call glDisable(GL_CULL_FACE)
        void setup() const {
            //TODO: (Req 3) Write this function


            glDepthMask(depthMask);
            glColorMask(colorMask[0],colorMask[1],colorMask[2],colorMask[3]);
            // checking blending
            // blending gives us the ability to render semi-transparent images with different levels of transparency
            /*
            Blending in OpenGL happens with the following equation:

                                C_result=C_source∗F_source+C_destination∗F_destination(1)

            C_source: the source color vector. This is the color output of the fragment shader.
            C_destination: the destination color vector. This is the color vector that is currently stored in the color buffer.
            F_source: the source factor value. Sets the impact of the alpha value on the source color.
            F_destination: the destination factor value. Sets the impact of the alpha value on the destination color
            */
            if(blending.enabled){
                glEnable(GL_BLEND); // not sure if disabled by default
                // glBlendEquation is used to pass the equation we want to use between the source & destination
                glBlendEquation(blending.equation);
                // glBlendFunc we pass the alpha of the source & the destination
                glBlendFunc(blending.sourceFactor, blending.destinationFactor);
                // glBendColor must be set using this func
                glBlendColor(blending.constantColor[0],blending.constantColor[1],blending.constantColor[2],blending.constantColor[3]);
            }else{
                glDisable(GL_BLEND);
            }

            // checking depth_testing 
            if(depthTesting.enabled){
                glEnable(GL_DEPTH_TEST); // disabled by default
                // we should clear depth buffer every frame to be updated always with the new values in our buffer
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                // setting the comparison operator of the depth test func
                glDepthFunc(depthTesting.function); 
            }

            // checking face_cull 
            if(faceCulling.enabled){
                glEnable(GL_CULL_FACE); // disabled by default
                // from now on all faces that aren't front faces are discarded which increase 50% of performance
                // in func glCullFace we choose which face to cull'discard' Front/Back or both "default value GL_BACK"
                glCullFace(faceCulling.culledFace);
                // in func glFrontFace we choose that we would prefer clockwise faces than counter-clockwise faces "default value GL_CCW"
                glFrontFace(faceCulling.frontFace);
            }

            //

        }

        // Given a json object, this function deserializes a PipelineState structure
        void deserialize(const nlohmann::json& data);
    };

}