#include "stdafx.h"
#include "X8RDevice.h"
#include <shared/DataContainer.h>

static const std::string Model("X-8R");

namespace devices
{

   CX8RDevice::CX8RDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& device)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;

      shared::CDataContainer details;
      details.set("provider", "X8-R");
      details.set("shortProvider", "x8r");

      // Relay Configuration
      for (int counter = 0; counter<X8R_RELAY_QTY; ++counter)
      {
         std::stringstream name;
         name << "R" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(name.str(), yApi::EKeywordAccessMode::kGetSet);
         keywordsToDeclare.push_back(temp);
      }

      //Déclaration of all IOs
      api->declareDevice(device, Model, keywordsToDeclare, details);
   }

   CX8RDevice::~CX8RDevice()
   {}
}// namespace devices