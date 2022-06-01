#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include <ecs/entity.hpp>
#include <iostream>

// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State
{

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;

    void onInitialize() override
    {
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets"))
        {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world"))
        {
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override
    {

        logic(&world);
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene

        renderer.render(&world);
    }

    void onDestroy() override
    {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }

    // the logic of the game should implemented here
    void logic(our::World *world)
    {
        our::Entity *car = nullptr;
        our::Entity *energy = nullptr;
        // Making the logic of collision
        for (const auto i : world->getEntities())
        {
            // first getting the cam position to check it with wall/gas
            if (i->name == "car")
            {
                car = i;
            }
            if (i->name == "energy")
            {
                energy = i;
            }
        }

        for (const auto i : world->getEntities())
        {
            if (i->name.substr(0, 3) == "gas")
            {
                glm::vec3 carPos=glm::vec3(car->getLocalToWorldMatrix()* glm::vec4(car->localTransform.position.x,car->localTransform.position.y,car->localTransform.position.z,1));
                glm::vec3 objPos=i->localTransform.position;
                //check for position related to car
                // std::cout<<i->name<<"  ";
                // std::cout<<abs(abs(i->localTransform.position.z) - abs(car->localTransform.position.z))<<"  "<<g<<"   "<<abs(abs(i->localTransform.position.z))<<std::endl;

                if(abs(abs(objPos.z) - abs(carPos.z)) < 0.1 ){
                    double xObj=objPos.x;
                    double xCar=carPos.x;
                    double leftCar=abs(xCar)-1.5;
                    double rightCar=abs(xCar)+1.5;
                    double leftObj=abs(xObj)-0.8;
                    double rightObj=abs(xObj)+0.8;

                    std::cout<<leftCar<<"  "<<rightCar<<"  "<<leftObj<<"  "<<rightObj<<std::endl;

                    if((leftObj<rightCar && leftObj>leftCar) || (rightObj<rightCar && rightObj>leftCar)){
                        // TODO: increase the energy bar of car
                        std::cout<<"collision gas"<<std::endl;
                    }
                        
                }
            }
            if (i->name.substr(0, 4) == "wall")
            {
                glm::vec3 carPos=glm::vec3(car->getLocalToWorldMatrix()* glm::vec4(car->localTransform.position.x,car->localTransform.position.y,car->localTransform.position.z,1));
                glm::vec3 objPos=i->localTransform.position;
                //check for position related to car
                // std::cout<<i->name<<"  ";
                // std::cout<<abs(abs(i->localTransform.position.z) - abs(car->localTransform.position.z))<<"  "<<g<<"   "<<abs(abs(i->localTransform.position.z))<<std::endl;

                if(abs(abs(objPos.z) - abs(carPos.z)) < 0.1 ){
                    double xObj=objPos.x;
                    double xCar=carPos.x;
                    double leftCar=abs(xCar)-1.5;
                    double rightCar=abs(xCar)+1.5;
                    double leftObj=abs(xObj)-3.8;
                    double rightObj=abs(xObj)+3.8;

                    std::cout<<leftCar<<"  "<<rightCar<<"  "<<leftObj<<"  "<<rightObj<<std::endl;

                    if((leftObj<rightCar && leftObj>leftCar) || (rightObj<rightCar && rightObj>leftCar)){
                        // TODO: decrement the energy bar of car
                        std::cout<<"collision wall"<<std::endl;
                    }
                        
                }
            }
        }

        if (getApp()->getKeyboard().justPressed(GLFW_KEY_ESCAPE))
        {
            onDestroy();
            exit(0);
        }
    }
};
