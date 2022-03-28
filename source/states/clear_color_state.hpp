#pragma once

#include <application.hpp>
#include <json/json.hpp>

class ClearColorState: public our::State {
    // onInitialize() function is called once before the state starts
    void onInitialize() override {
        //TODO: Read the color from the configuration file and use it to set the clear color for the window
        //HINT: you can the configuration for the whole application using "getApp()->getConfig()"
        //To see how the clear color is written in the json files, see "config/blue-screen.json"
        //To know how read data from a nlohmann::json object, 
        //look at the following documentation: https://json.nlohmann.me/features/element_access/

        nlohmann::json app_config=getApp()->getConfig();

        float r_channel = app_config["scene"]["clear-color"]["r"].get<float>();
        float g_channel = app_config["scene"]["clear-color"]["g"].get<float>();
        float b_channel = app_config["scene"]["clear-color"]["b"].get<float>();
        float a_channel = app_config["scene"]["clear-color"]["a"].get<float>();
        
        glClearColor(r_channel,g_channel,b_channel,a_channel);
    }

    // onDraw(deltaTime) function is called every frame 
    void onDraw(double deltaTime) override {
        //At the start of frame we want to clear the screen. Otherwise we would still see the results from the previous frame.
        glClear(GL_COLOR_BUFFER_BIT);
    }
};