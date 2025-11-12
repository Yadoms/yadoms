#pragma once
#include <vector>
#include "Entities/HueInformations.h"

class IHueBridgeDiscovery
{
public:
  
   virtual ~IHueBridgeDiscovery() = default;

   virtual std::vector<CHueInformations> FindBridges() = 0;

   virtual CHueInformations getHueInformations() = 0;

};
