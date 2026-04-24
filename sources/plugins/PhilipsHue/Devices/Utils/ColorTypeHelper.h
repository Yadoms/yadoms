#pragma once
#include <set>
#include <string>
#include "ColorType.h"
#include "shared/DataContainer.h"
#include "LightType.h"

class CColorTypeHelper
{
public:

   CColorTypeHelper() = default;
   virtual ~CColorTypeHelper() = default;

   static EColorType getColorType(boost::shared_ptr<shared::CDataContainer>& response, std::string& type, int& lightCounter);
   static ELightType getLightType(std::string& response);

private:
   static EColorType getGamutOrLegacyColorType(boost::shared_ptr<shared::CDataContainer>& response, bool hasCt, int& lightCounter);
   static const std::set<std::string> GamutBTypes;
   static const std::set<std::string> GamutCTypes;
   static const std::set<std::string> GamutATypes;
 
};
