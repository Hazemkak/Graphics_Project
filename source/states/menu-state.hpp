#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>

// This state shows how to use the ECS framework and deserialization.
class MenuState : public our::State
{

    our::World world;
    our::ForwardRenderer renderer;
    our::MovementSystem movementSystem;

    void onInitialize() override
    {
        // First of all, we get the scene configuration from the app config
        auto &config = getApp()->getConfig()["menu-scene"];

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

        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override
    {

        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);

        logic(&world);

        // And finally we use the renderer system to draw the scene
        renderer.render(&world);
    }

    void onDestroy() override
    {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }

    void logic(our::World *world)
    {
        our::Entity *start = nullptr;
        our::Entity *quit = nullptr;

        float smallScale=1;
        float largeScale=2; 

        // Making the logic of collision
        for (const auto i : world->getEntities())
        {
            // first getting the cam position to check it with wall/gas
            if (i->name.substr(0,5) == "start")
            {
                start = i;
                continue;
            }
            if (i->name.substr(0,4) == "quit")
            {
                quit = i;
                continue;
            }
        }

        if (getApp()->getKeyboard().justPressed(GLFW_KEY_ENTER))
        {
            if(quit->localTransform.scale.x==largeScale){
                onDestroy();
                exit(0);
            }else{
                world->clear();
                getApp()->changeState("game");
            }
        }
        else if(getApp()->getKeyboard().justPressed(GLFW_KEY_RIGHT) && quit->localTransform.scale.x==smallScale){
            quit->localTransform.scale.x=largeScale;
            quit->localTransform.scale.y=largeScale;
            start->localTransform.scale.x=smallScale;
            start->localTransform.scale.y=smallScale;
        }else if(getApp()->getKeyboard().justPressed(GLFW_KEY_LEFT) && start->localTransform.scale.x==smallScale){
            start->localTransform.scale.x=largeScale;
            start->localTransform.scale.y=largeScale;
            quit->localTransform.scale.x=smallScale;
            quit->localTransform.scale.y=smallScale;
        }
        else if (getApp()->getKeyboard().justPressed(GLFW_KEY_ESCAPE))
        {
            onDestroy();
            exit(0);
        }
    }
};