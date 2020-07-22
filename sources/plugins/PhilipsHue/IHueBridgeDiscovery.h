#pragma once
#include "HueInformations.h"

class IHueBridgeDiscovery
{
public:
  
   virtual ~IHueBridgeDiscovery() = default;

   virtual std::vector<HueInformations> FindBridges() = 0;

   virtual HueInformations getHueInformations() = 0;

};
