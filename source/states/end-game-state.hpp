#pragma once
#include <application.hpp>
#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>

// This state shows how to use the ECS framework and deserialization.
class EndGameState: public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["gameover-scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);

        logic(&world);
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
    void logic(our::World *world)
    {
        our::Entity *EXIT = nullptr;
        our::Entity *REPLAY = nullptr;

        /* small scall means it is not selected */
        float smallScale=1;

         /* small scall means it is selected */
        float largeScale=2; 

        // Making the logic for the start menu
        for (const auto i : world->getEntities())
        {
            if (i->name.substr(0,4) == "EXIT")
            {
                EXIT = i;
                continue;
            }
            if (i->name.substr(0,4) == "REPL")
            {
                REPLAY = i;
                continue;
            }
        }
         
        /* Check if the Enter key is pressed */
        if (getApp()->getKeyboard().justPressed(GLFW_KEY_ENTER))
        {
            if(EXIT->localTransform.scale.x==largeScale){
                onDestroy();
                exit(0);
            }else{
                world->clear();
                getApp()->changeState("game");
            }
        }
        /* Check if the right key is pressed and Exit is not selected */
        else if(getApp()->getKeyboard().justPressed(GLFW_KEY_RIGHT) && EXIT->localTransform.scale.x==smallScale){
            EXIT->localTransform.scale.x=largeScale;
            EXIT->localTransform.scale.y=largeScale;
            REPLAY->localTransform.scale.x=smallScale;
            REPLAY->localTransform.scale.y=smallScale;
        }
        /* Check if the left key is pressed and Replay is not selected */
        else if(getApp()->getKeyboard().justPressed(GLFW_KEY_LEFT) && REPLAY->localTransform.scale.x==smallScale){
            REPLAY->localTransform.scale.x=largeScale;
            REPLAY->localTransform.scale.y=largeScale;
            EXIT->localTransform.scale.x=smallScale;
            EXIT->localTransform.scale.y=smallScale;
        }
        /* Check if the ESCAPE key is pressed  */
        else if (getApp()->getKeyboard().justPressed(GLFW_KEY_ESCAPE))
        {
            onDestroy();
            exit(0);
        }
    }


    
};