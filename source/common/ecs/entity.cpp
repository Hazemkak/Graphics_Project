#include "entity.hpp"
#include "../deserialize-utils.hpp"
#include "../components/component-deserializer.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function returns the transformation matrix from the entity's local space to the world space
    // Remember that you can get the transformation matrix from this entity to its parent from "localTransform"
    // To get the local to world matrix, you need to combine this entities matrix with its parent's matrix and
    // its parent's parent's matrix and so on till you reach the root.
    glm::mat4 Entity::getLocalToWorldMatrix() const {
        //TODO: (Req 7) Write this function
        glm::mat4 All_T=  localTransform.toMat4(); // All transfromation matrices from child to the root 
        Entity * next =parent;
        while (next!=nullptr)
        {
            All_T= next->localTransform.toMat4()*All_T; // multiply transfromation matrices till the root
            next =next->parent;
            
        }
        
        return All_T;
    }

    // Deserializes the entity data and components from a json object
    void Entity::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        name = data.value("name", name);
        // in case gas/obstacle we send it's type to random it's position on x-axis
        localTransform.deserialize(data,name.substr(0,3)=="gas" || name.substr(0,3)=="can" ? 'g': (name.substr(0,4)=="wall" || name.substr(0,4)=="bump")?'o':' ');

        if(data.contains("components")){
            if(const auto& components = data["components"]; components.is_array()){
                for(auto& component: components){
                    deserializeComponent(component, this);
                }
            }
        }
    }

}