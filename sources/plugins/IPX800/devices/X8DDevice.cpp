#include "stdafx.h"
#include "X8DDevice.h"
#include <shared/DataContainer.h>

static const std::string Model("X-8D");

namespace devices
{

   CX8DDevice::CX8DDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& device)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;

      shared::CDataContainer details;
      details.set("provider", "X8-D");
      details.set("shortProvider", "x8d");

      // Relay Configuration
      for (int counter = 0; counter<X8D_DI_QTY; ++counter)
      {
         std::stringstream name;
         name << "D" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(name.str(), yApi::EKeywordAccessMode::kGet);
         keywordsToDeclare.push_back(temp);
      }

      //Déclaration of all IOs
      api->declareDevice(device, Model, keywordsToDeclare, details);
   }

   CX8DDevice::~CX8DDevice()
   {}
}// namespace devices