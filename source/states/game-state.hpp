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
class Gamestate : public our::State
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

        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene
        logic(&world, deltaTime);

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

    void checkCollision(our::Entity* obj,our::Entity* energy,our::Entity* car , our::Entity* camera){

    }

    // the logic of the game should implemented here
    void logic(our::World *world, double deltaTime)
    {
        
        const double MAX_SCALE = 7;
        double wallCollision = 2.5;
        double gasCollision = 1.5;
        double stepForward = (rand() % 100) + 70;
        

        our::Entity *car = nullptr;
        our::Entity *energy = nullptr;
        our::Entity *camera = nullptr;

        // Making the logic of collision
        for (const auto i : world->getEntities())
        {
            // first getting the cam position to check it with wall/gas
            if (i->name == "car")
            {
                car = i;
                continue;
            }
            if (i->name == "energy")
            {
                energy = i;
                continue;
            }
            if (i->name == "camera")
            {
                camera = i;
                continue;
            }
        }
        our::MovementComponent *movement = camera->getComponent<our::MovementComponent>();
        double zDepth = movement->linearVelocity.z/-200.0;

        glm::vec3 carPos = glm::vec3(car->getLocalToWorldMatrix() * glm::vec4(car->localTransform.position.x, car->localTransform.position.y, car->localTransform.position.z, 1));
        // std::cout<<carPos.z<<"     kkkk"<<std::endl;

        // decrement the car energy with time
        energy->localTransform.scale.x -= deltaTime / 10.0;

        for (const auto i : world->getEntities())
        {
            if (i->name.substr(0, 3) == "gas")
            {

                glm::vec3 objPos = i->localTransform.position;
                // check for position related to car
                //  TODO: check for the total car length collision
                if (abs(abs(objPos.z) - abs(carPos.z)) < zDepth)
                {

                    if (abs(carPos.x - objPos.x) <= (car->localTransform.scale.x + i->localTransform.scale.x) + 0.5)
                    {
                        // TODO: increase the energy bar of car
                        i->localTransform.position.z -= stepForward; // to avoid multiple collision
                        energy->localTransform.scale.x += gasCollision;
                        std::cout << "collision gas" << std::endl;
                    }
                }
                else if (abs(objPos.z) < abs(carPos.z) - 10)
                {

                    i->localTransform.position.x = (rand() % 51) - 25; // to change x-axis pos
                    i->localTransform.position.z -= stepForward;       // to avoid multiple collision
                }
            }
            else if (i->name.substr(0, 4) == "wall")
            {
                glm::vec3 carPos = glm::vec3(car->getLocalToWorldMatrix() * glm::vec4(car->localTransform.position.x, car->localTransform.position.y, car->localTransform.position.z, 1));
                glm::vec3 objPos = i->localTransform.position;

                if (abs(abs(objPos.z) - abs(carPos.z)) < zDepth)
                {

                    if (abs(carPos.x - objPos.x) <= (car->localTransform.scale.x + i->localTransform.scale.x))
                    {
                        // TODO: increase the energy bar of car
                        i->localTransform.position.z -= stepForward; // to avoid multiple collision
                        energy->localTransform.scale.x -= wallCollision;
                        std::cout << "collision wall" << std::endl;
                    }
                }
                else if (abs(objPos.z) < abs(carPos.z) - 10)
                {

                    i->localTransform.position.x = (rand() % 51) - 25; // to change x-axis pos
                    i->localTransform.position.z -= stepForward;       // to avoid multiple collision
                }
            }
            else if (i->name.substr(0, 4) == "bump")
            {
                glm::vec3 carPos = glm::vec3(car->getLocalToWorldMatrix() * glm::vec4(car->localTransform.position.x, car->localTransform.position.y, car->localTransform.position.z, 1));
                glm::vec3 objPos = glm::vec3(i->getLocalToWorldMatrix() * glm::vec4(i->localTransform.position.x, i->localTransform.position.y, i->localTransform.position.z, 1));

                if (abs(abs(objPos.z) - abs(carPos.z)) < zDepth)
                {
                    std::cout << "hhh" << std::endl;

                    if (abs(carPos.x - objPos.x) <= (car->localTransform.scale.x + i->localTransform.scale.y))
                    {
                        // TODO: increase the energy bar of car
                        i->localTransform.position.z -= stepForward; // to avoid multiple collision
                        our::MovementComponent *movement = camera->getComponent<our::MovementComponent>();
                        movement->linearVelocity.z /= 1.3;
                        std::cout << "bump collision" << std::endl;
                    }
                }
                else if (abs(objPos.z) < abs(carPos.z) - 10)
                {

                    i->localTransform.position.x = (rand() % 51) - 25; // to change x-axis pos
                    i->localTransform.position.z -= stepForward;       // to avoid multiple collision
                }
            }
            else if (i->name.substr(0, 3) == "can")
            {

                glm::vec3 objPos = i->localTransform.position;
                // check for position related to car
                //  TODO: check for the total car length collision
                if (abs(abs(objPos.z) - abs(carPos.z)) < zDepth)
                {

                    if (abs(carPos.x - objPos.x) <= (car->localTransform.scale.x + i->localTransform.scale.x) + 0.5)
                    {
                        // TODO: increase the energy bar of car
                        i->localTransform.position.z -= stepForward; // to avoid multiple collision
                        our::MovementComponent *movement = camera->getComponent<our::MovementComponent>();
                        movement->linearVelocity.z *= 1.3;
                        std::cout << "can collision" << std::endl;
                    }
                }
                else if (abs(objPos.z) < abs(carPos.z) - 10)
                {

                    i->localTransform.position.x = (rand() % 51) - 25; // to change x-axis pos
                    i->localTransform.position.z -= stepForward;       // to avoid multiple collision
                }
            }
        }

        if (energy->localTransform.scale.x <= 0)
        {
            energy->localTransform.scale.x = 0;
            // TODO: go to game over
            getApp()->changeState("end");
        }

        if (energy->localTransform.scale.x > MAX_SCALE)
        {
            energy->localTransform.scale.x = MAX_SCALE;
        }

        if (getApp()->getKeyboard().justPressed(GLFW_KEY_ESCAPE))
        {
            getApp()->changeState("end");
            // onDestroy();
            // exit(0);
        }
    }
};
