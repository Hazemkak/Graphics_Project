#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

#include<iostream>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 2) Write this function

       
        glm::mat4 T= glm::translate(
            glm::mat4(1.0f),
            position
        );


        glm::mat4 S=glm::scale(
            glm::mat4(1.0f),
            scale
        );

       glm::mat4 R= glm::yawPitchRoll( rotation[1],rotation[0],rotation[2] );
       glm::mat4 SRT =T * R * S;
     


        return SRT;
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}