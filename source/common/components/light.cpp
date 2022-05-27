#include "light.hpp"
#include "../deserialize-utils.hpp"
#include <iostream>

namespace our
{

  
  // Reads Light parameters from the given json object
  void LightComponent::deserialize(const nlohmann::json &data)
  {
    if (!data.is_object())
      return;
    std::string lightTypeStr = data.value("lightType", "DIRECTIONAL");
    
    if (lightTypeStr == "DIRECTIONAL")
    {
      lightType = LightType::DIRECTIONAL;
    }
    else if (lightTypeStr == "POINT")
    {
      lightType = LightType::POINT;
      attenuation = data.value("attenuation", attenuation);
    }
    else if (lightTypeStr == "SPOT")
    {
      lightType = LightType::SPOT;
      cone_angles = data.value("cone_angles", cone_angles);
    }

    diffuse = data.value("diffuse", diffuse);
    specular = data.value("specular", specular);
    position= data.value("position",position);
    direction= data.value("direction",direction);

  }
}