#pragma once
#include "ColorType.h"
#include "shared/DataContainer.h"

class CColorTypeHelper
{
public:

   CColorTypeHelper() = default;
   virtual ~CColorTypeHelper() = default;

   static EColorType getColorType(boost::shared_ptr<shared::CDataContainer>& response);

private:
   static EColorType getGamutOrLegacyColorType(boost::shared_ptr<shared::CDataContainer>& response, bool hasCt);
   static const std::set<std::string> GamutBTypes;
   static const std::set<std::string> GamutCTypes;
   static const std::set<std::string> GamutATypes;
   static const std::string OnOffLight;
   static const std::string OnOffPluginInUnit;
   static const std::string DimmableLight;
   static const std::string DimmablePluginInUnit;
   static const std::string ColorTemperatureLight;
   static const std::string ColorLight;
   static const std::string ExtendedColorLight;
};
