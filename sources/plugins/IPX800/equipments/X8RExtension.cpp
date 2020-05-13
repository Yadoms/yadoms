#include "stdafx.h"
#include "X8RExtension.h"
#include <shared/DataContainer.h>
#include "../specificHistorizers/inputOutput.h"
#include "noInformationException.hpp"
#include <shared/Log.h>

namespace equipments
{
   CX8RExtension::CX8RExtension(boost::shared_ptr<yApi::IYPluginApi> api,
                                const std::string& device,
                                const int position):
      m_deviceName(device),
      m_position(position)
   {
      shared::CDataContainerSharedPtr details = new_CDataContainerSharedPtr();
      details->set("provider", "IPX800");
      details->set("shortProvider", "ipx");
      details->set("type", deviceType());
      details->set("position", boost::lexical_cast<std::string>(position));

      // Relay Configuration
      for (int counter = 0; counter < X8R_RELAY_QTY; ++counter)
      {
         std::stringstream name, hardwareName;
         name << "R" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         hardwareName << "R" << boost::lexical_cast<int>(position * 8 + counter + 1);
         auto temp = boost::make_shared<specificHistorizers::CInputOuput>(name.str(),
                                                                          hardwareName.str(),
                                                                          yApi::EKeywordAccessMode::kGetSet);
         m_keywordList.push_back(temp);
      }

      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> keywordsToDeclare;
      keywordsToDeclare.insert(keywordsToDeclare.end(), m_keywordList.begin(), m_keywordList.end());

      //Declaration of all IOs
      api->updateDeviceDetails(device, details);
      api->declareKeywords(device, keywordsToDeclare);

      YADOMS_LOG(trace) << "creation of the device " << device << " of type " << deviceType() << " at position " << position;
   }

   std::string CX8RExtension::getDeviceName() const
   {
      return m_deviceName;
   }

   std::string CX8RExtension::getDeviceType() const
   {
      return deviceType();
   }

   int CX8RExtension::getSlot() const
   {
      return m_position;
   }

   void CX8RExtension::updateFromDevice(const std::string& type,
                                        boost::shared_ptr<yApi::IYPluginApi> api,
                                        shared::CDataContainerSharedPtr& values,
                                        bool forceHistorization)
   {
      if (type == "R")
      {
         std::vector<boost::shared_ptr<specificHistorizers::CInputOuput>>::const_iterator diIterator;
         std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> keywordsToHistorize;
         std::string productRevision("");

         try
         {
            productRevision = values->getWithDefault<std::string>("product", "");
            for (diIterator = m_keywordList.begin(); diIterator != m_keywordList.end(); ++diIterator)
            {
               bool newValue = values->get<bool>((*diIterator)->getHardwareName());

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

   shared::CDataContainerSharedPtr CX8RExtension::buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                                                              shared::CDataContainerSharedPtr& parametersToCopy,
                                                              boost::shared_ptr<const yApi::IDeviceCommand> command)
   {
      auto parameters = parametersToCopy->copy();

      std::string keywordName = command->getKeyword();

      std::vector<boost::shared_ptr<specificHistorizers::CInputOuput>>::const_iterator diIterator;

      try
      {
         for (diIterator = m_keywordList.begin(); diIterator != m_keywordList.end(); ++diIterator)
         {
            // // Keyword found
            if ((*diIterator)->getKeyword() == keywordName)
            {
               //separation of letters and digits
               boost::regex reg("([a-zA-Z]+)(\\d+)");
               boost::smatch match;

               //Set parameters
               if (boost::regex_search(keywordName, match, reg))
               {
                  if (boost::lexical_cast<bool>(command->getBody()))
                     parameters->set("Set" + (*diIterator)->getHardwareName(), match[2].str());
                  else
                     parameters->set("Clear" + (*diIterator)->getHardwareName(), match[2].str());

                  m_pendingHistorizer = (*diIterator);
               }
            }
         }
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(information) << "error retrieve value :" << e.what() ;
      }

      return parameters;
   }

   void CX8RExtension::historizePendingCommand(boost::shared_ptr<yApi::IYPluginApi> api, boost::shared_ptr<const yApi::IDeviceCommand> command)
   {
      if (m_pendingHistorizer)
      {
         bool newValue = boost::lexical_cast<bool>(command->getBody());
         if (m_pendingHistorizer->get() != newValue)
         {
            m_pendingHistorizer->set(newValue);
            api->historizeData(m_deviceName, m_pendingHistorizer);
         }

         m_pendingHistorizer.reset();
      }
   }

   void CX8RExtension::resetPendingCommand()
   {
      m_pendingHistorizer.reset();
   }

   void CX8RExtension::setNewConfiguration(const shared::CDataContainerSharedPtr& newConfiguration)
   {
      std::vector<boost::shared_ptr<specificHistorizers::CInputOuput>>::const_iterator iterator;

      m_position = newConfiguration->get<int>("Position");
      int counter = 0;

      // change all hardware names
      for (iterator = m_keywordList.begin(); iterator != m_keywordList.end(); ++iterator)
      {
         (*iterator)->setNewHardwareName(std::string("D") + boost::lexical_cast<std::string>(m_position * 8 + counter + 1));
         ++counter;
      }

      YADOMS_LOG(information) << "equipment " << m_deviceName << " configuration is updated";
   }

   const std::string& CX8RExtension::deviceType()
   {
      static const std::string deviceType("X-8R");
      return deviceType;
   }

   CX8RExtension::~CX8RExtension()
   {
   }
}// namespace equipments


