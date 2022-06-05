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
        renderer.render(&world);
        logic(&world, deltaTime);
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

    void renderObjectAgain(int stepForward, our::Entity *obj)
    {
        obj->localTransform.position.z -= stepForward;
    }

    void checkCollision(our::Entity *obj, our::Entity *energy, our::Entity *car, our::Entity *camera)
    {
        double wallCollision = 1.5;
        double gasCollision = 1.5;
        double stepForward = (rand() % 100) + 120;

        glm::vec3 objPos = obj->localTransform.position;
        our::MovementComponent *movement = camera->getComponent<our::MovementComponent>();
        double zDepth = movement->linearVelocity.z / -50.0;

        glm::vec3 carPos = glm::vec3(car->getLocalToWorldMatrix() * glm::vec4(car->localTransform.position, 1));
        // check for position related to car
        if (abs(abs(objPos.z) - abs(carPos.z - 10)) < zDepth)
        {

            if (abs(carPos.x - objPos.x) <= (car->localTransform.scale.x + obj->localTransform.scale.x + 0.5))
            {
                // Collision detected
                if (obj->name.substr(0, 3) == "gas") //----------- GAS ------------//
                {
                    renderObjectAgain(stepForward+500, obj);
                    energy->localTransform.scale.x += gasCollision;
                    std::cout << "collision gas" << std::endl;
                }
                else if (obj->name.substr(0, 3) == "wal") //----------- Wall ------------//
                {
                    renderObjectAgain(stepForward, obj);
                    energy->localTransform.scale.x -= wallCollision;
                    std::cout << "collision wall" << std::endl;
                }
                else if (obj->name.substr(0, 3) == "bum") //----------- Bump ------------//
                {
                    renderObjectAgain(stepForward, obj);
                    movement->linearVelocity.z /= 1.3;
                    std::cout << "bump collision" << std::endl;
                }
                else if (obj->name.substr(0, 3) == "can") //----------- Can ------------//
                {
                    renderObjectAgain(stepForward+200, obj);
                    movement->linearVelocity.z *= 1.3;
                    std::cout << "can collision" << std::endl;
                }
            }
        }
        else if (abs(objPos.z) < abs(carPos.z) - 10)
        {

            if (obj->name.substr(0, 3) != "lam")
            {
                renderObjectAgain(stepForward, obj);
            }
            else if (obj->name.substr(0, 4) == "lamp" && (abs(objPos.z) < abs(carPos.z) - 100))
            {
                renderObjectAgain(200, obj);
            }
            else if (obj->name.substr(0, 4) == "lamm" && (abs(objPos.z) < abs(carPos.z) - 100))
            {
                renderObjectAgain(200, obj);
            }
        }
    }

    // the logic of the game should implemented here
    void logic(our::World *world, double deltaTime)
    {

        const double MAX_SCALE = 7;

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

        // decrement the car energy with time
        energy->localTransform.scale.x -= deltaTime / 10.0;

        for (const auto i : world->getEntities())
        {
            if (i->name.substr(0, 3) == "bum" || i->name.substr(0, 3) == "wal" || i->name.substr(0, 3) == "gas" || i->name.substr(0, 3) == "can" || i->name.substr(0, 3) == "lam")
                checkCollision(i, energy, car, camera);
            if (i->name.substr(0, 4) == "road" && ((abs(i->localTransform.position.z) + i->localTransform.scale.x) < abs(glm::vec3(car->getLocalToWorldMatrix() * glm::vec4(car->localTransform.position, 1)).z) - 50))
                i->localTransform.position.z -= 2000; // render the road again in front
        }

        if (energy->localTransform.scale.x <= 0)
        {
            energy->localTransform.scale.x = 0;
            // go to game over
            world->clear();
            getApp()->changeState("end");
        }
        else if (energy->localTransform.scale.x > MAX_SCALE)
        {
            energy->localTransform.scale.x = MAX_SCALE;
        }

        if (getApp()->getKeyboard().justPressed(GLFW_KEY_ESCAPE))
        {
            world->clear();
            getApp()->changeState("end");
        }
    }
};
