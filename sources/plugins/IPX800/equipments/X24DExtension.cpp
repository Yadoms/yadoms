#include "stdafx.h"
#include "X24DExtension.h"
#include <shared/DataContainer.h>
#include "../specificHistorizers/inputOutput.h"
#include "noInformationException.hpp"
#include <shared/Log.h>

namespace equipments
{

   CX24DExtension::CX24DExtension(boost::shared_ptr<yApi::IYPluginApi> api,
                                  const std::string& device,
                                  const int position):
      m_deviceName(device),
      m_deviceType("X-24D"),
      m_position(position)
   {
      shared::CDataContainer details;
      details.set("provider", "IPX800");
      details.set("shortProvider", "ipx");
      details.set("type", m_deviceType);
      details.set("position", boost::lexical_cast<std::string>(position));

      // Relay Configuration
      for (int counter = 0; counter<X24D_DI_QTY; ++counter)
      {
         std::stringstream name, hardwareName;
         name << "D" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         hardwareName << "D" << boost::lexical_cast<int>(position*24 + counter + 1);
         boost::shared_ptr<specificHistorizers::CInputOuput> temp = boost::make_shared<specificHistorizers::CInputOuput>(name.str(),
                                                                                                                         hardwareName.str(),
                                                                                                                         yApi::EKeywordAccessMode::kGet);
         m_keywordList.push_back(temp);
      }

      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;
      keywordsToDeclare.insert(keywordsToDeclare.end(), m_keywordList.begin(), m_keywordList.end());

      //Declaration of all IOs
      api->declareDevice(device, m_deviceType, keywordsToDeclare, details);
   }

   std::string CX24DExtension::getDeviceName() const
   {
      return m_deviceName;
   }

   std::string CX24DExtension::getDeviceType() const
   {
      return m_deviceType;
   }

   int CX24DExtension::getSlot() const
   {
      return m_position;
   }

   shared::CDataContainer CX24DExtension::buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api, 
                                                               shared::CDataContainer& parameters, 
                                                               boost::shared_ptr<const yApi::IDeviceCommand> command)
   {
      throw shared::exception::CException("Extension module X-24D have no keyword to set");
   }

   void CX24DExtension::updateFromDevice(const std::string& type, 
                                         boost::shared_ptr<yApi::IYPluginApi> api,
                                         shared::CDataContainer& values,
                                         bool forceHistorization)
   {
      if (type == "D")
      {
         std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> >::const_iterator diIterator;
         std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToHistorize;
         std::string productRevision("");

         try {
            productRevision = values.getWithDefault<std::string>("product", "");
            for (diIterator = m_keywordList.begin(); diIterator != m_keywordList.end(); ++diIterator)
            {
               bool newValue = values.get<bool>((*diIterator)->getHardwareName());

               //historize only for new value
               if ((*diIterator)->get() != newValue || forceHistorization)
               {
				  YADOMS_LOG(information) << "read IO : " << (*diIterator)->getHardwareName() << " : " << boost::lexical_cast<std::string>(newValue) ;
                  (*diIterator)->set(newValue);
                  keywordsToHistorize.push_back((*diIterator));
               }
            }
         }
         catch (shared::exception::CInvalidParameter&)
         {
            // If we could read the product revision, IOs have not been return : so it's an error of conception, or a wrong apikey
            if (productRevision == "IPX800_V4") throw CNoInformationException("Equipment connected, but no information received. Please check apikey");
         }

         api->historizeData(m_deviceName, keywordsToHistorize);
      }
   }

   void CX24DExtension::historizePendingCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                                                boost::shared_ptr<const yApi::IDeviceCommand> command)
   {
      throw shared::exception::CException("Extension module X-24D have no keyword to set");
   }

   void CX24DExtension::resetPendingCommand()
   {
      throw shared::exception::CException("Extension module X-24D have no pending operation");
   }

   CX24DExtension::~CX24DExtension()
   {}
}// namespace equipments