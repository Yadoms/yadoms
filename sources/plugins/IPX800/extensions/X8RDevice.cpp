#include "stdafx.h"
#include "X8RDevice.h"
#include <shared/DataContainer.h>
#include "../specificHistorizers/inputOutput.h"

static const std::string Model("X-8R");

namespace extensions
{

   CX8RDevice::CX8RDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                          const std::string& device,
                          const int& position):
      m_deviceName(device),
      m_slotNumber(position)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;

      shared::CDataContainer details;
      details.set("provider", "IPX800");
      details.set("shortProvider", "ipx");
      details.set("type", "X24-D");

      // Relay Configuration
      for (int counter = 0; counter<X8R_RELAY_QTY; ++counter)
      {
         std::stringstream name, hardwareName;
         name << "R" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         hardwareName << boost::lexical_cast<int>(position * 8 + counter + 1);
         boost::shared_ptr<specificHistorizers::CInputOuput> temp = boost::make_shared<specificHistorizers::CInputOuput>(name.str(),
                                                                                                                         hardwareName.str(),
                                                                                                                         yApi::EKeywordAccessMode::kGetSet);
         keywordsToDeclare.push_back(temp);
      }

      //Déclaration of all IOs
      api->declareDevice(device, Model, keywordsToDeclare, details);
   }

   std::string CX8RDevice::getDeviceName() const
   {
      return m_deviceName;
   }

   int CX8RDevice::getSlot() const
   {
      return m_slotNumber;
   }

   CX8RDevice::~CX8RDevice()
   {}
}// namespace extensions