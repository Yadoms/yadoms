#include "stdafx.h"
#include "X8RExtension.h"
#include <shared/DataContainer.h>
#include "../specificHistorizers/inputOutput.h"

namespace equipments
{

   CX8RExtension::CX8RExtension(boost::shared_ptr<yApi::IYPluginApi> api,
                                const std::string& device,
                                const int& position):
      m_deviceName(device),
      m_deviceType("X-8R")
   {
      shared::CDataContainer details;
      details.set("provider", "IPX800");
      details.set("shortProvider", "ipx");
      details.set("type", m_deviceType);
      details.set("position", boost::lexical_cast<std::string>(position));

      // Relay Configuration
      for (int counter = 0; counter<X8R_RELAY_QTY; ++counter)
      {
         std::stringstream name, hardwareName;
         name << "R" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         hardwareName << "R" << boost::lexical_cast<int>(position * 8 + counter + 1);
         boost::shared_ptr<specificHistorizers::CInputOuput> temp = boost::make_shared<specificHistorizers::CInputOuput>(name.str(),
                                                                                                                         hardwareName.str(),
                                                                                                                         yApi::EKeywordAccessMode::kGetSet);
         m_keywordList.push_back(temp);
      }

      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;
      keywordsToDeclare.insert(keywordsToDeclare.end(), m_keywordList.begin(), m_keywordList.end());

      //Déclaration of all IOs
      api->declareDevice(device, m_deviceType, keywordsToDeclare, details);
   }

   std::string CX8RExtension::getDeviceName() const
   {
      return m_deviceName;
   }

   std::string CX8RExtension::getDeviceType() const
   {
      return m_deviceType;
   }

   void CX8RExtension::updateFromDevice(const std::string& type,
                                        boost::shared_ptr<yApi::IYPluginApi> api,
                                        shared::CDataContainer& values,
                                        bool forceHistorization)
   {
      if (type == "R")
      {
         std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> >::const_iterator diIterator;
         std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToHistorize;

         try {
            for (diIterator = m_keywordList.begin(); diIterator != m_keywordList.end(); ++diIterator)
            {
               bool newValue = values.get<bool>((*diIterator)->getHardwareName());

               //historize only for new value
               if ((*diIterator)->get() != newValue || forceHistorization)
               {
                  std::cout << "Set IO : " << (*diIterator)->getHardwareName() << std::endl;
                  (*diIterator)->set(newValue);
                  keywordsToHistorize.push_back((*diIterator));
               }
            }
         }
         catch (shared::exception::CException& e)
         {
            std::cout << "error retrieve value :" << e.what() << std::endl;
         }

         api->historizeData(m_deviceName, keywordsToHistorize);
      }
   }

   shared::CDataContainer CX8RExtension::buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api, 
                                                              shared::CDataContainer& parameters, 
                                                              boost::shared_ptr<const yApi::IDeviceCommand> command)
   {
      std::string keywordName = command->getKeyword();

      std::vector<boost::shared_ptr<specificHistorizers::CInputOuput> >::const_iterator diIterator;

      try {
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
                     parameters.set("Set" + (*diIterator)->getHardwareName(), match[2]);
                  else
                     parameters.set("Clear" + (*diIterator)->getHardwareName(), match[2]);

                  m_pendingHistorizer = (*diIterator);
               }
            }
         }
      }
      catch (shared::exception::CException& e)
      {
         std::cout << "error retrieve value :" << e.what() << std::endl;
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

   CX8RExtension::~CX8RExtension()
   {}
}// namespace equipments