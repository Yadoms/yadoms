#include "stdafx.h"
#include "X8DExtension.h"
#include <shared/DataContainer.h>
#include "../specificHistorizers/inputOutput.h"

static const std::string Model("X-8D");

namespace extensions
{

   CX8DExtension::CX8DExtension(boost::shared_ptr<yApi::IYPluginApi> api,
                                const std::string& device,
                                const int& position):
      m_deviceName(device),
      m_slotNumber(position)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;

      shared::CDataContainer details;
      details.set("provider", "IPX800");
      details.set("shortProvider", "ipx");
      details.set("type", "X8-D");

      // Relay Configuration
      for (int counter = 0; counter<X8D_DI_QTY; ++counter)
      {
         std::stringstream name, hardwareName;
         name << "D" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         hardwareName << "D" << boost::lexical_cast<int>(position * 8 + counter + 1);
         boost::shared_ptr<specificHistorizers::CInputOuput> temp = boost::make_shared<specificHistorizers::CInputOuput>(name.str(),
                                                                                                                         hardwareName.str(),
                                                                                                                         yApi::EKeywordAccessMode::kGet);
         keywordsToDeclare.push_back(temp);
      }

      //Déclaration of all IOs
      api->declareDevice(device, Model, keywordsToDeclare, details);
   }

   std::string CX8DExtension::getDeviceName() const
   {
      return m_deviceName;
   }

   int CX8DExtension::getSlot() const
   {
      return m_slotNumber;
   }

   CX8DExtension::~CX8DExtension()
   {}
}// namespace extensions