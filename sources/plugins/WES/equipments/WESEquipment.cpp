#include "stdafx.h"
#include "WESEquipment.h"
#include "masterdeviceConfiguration.h"
#include <shared/DataContainer.h>
#include "noInformationException.hpp"
#include <shared/Log.h>

namespace equipments
{
   CWESEquipment::CWESEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                                const std::string& device,
                                const shared::CDataContainer& deviceConfiguration):
      m_deviceName(device),
      m_deviceType("WES")
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToDeclare;
      shared::CDataContainer details;
      details.set("provider", "WES");
      details.set("type", m_deviceType);

      deviceConfiguration.printToLog(YADOMS_LOG(information));

      m_configuration.initializeWith(deviceConfiguration);

      // Relay Configuration
      for (int counter = 0; counter<WES_RELAY_QTY; ++counter)
      {
         std::stringstream name;
         name << "R" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<yApi::historization::CSwitch> temp = boost::make_shared<yApi::historization::CSwitch>(name.str(),
                                                                                                                 yApi::EKeywordAccessMode::kGetSet);
         m_relaysList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // TIC Counters Configuration
      for (int counter = 0; counter<WES_TIC_QTY; ++counter)
      {
         std::stringstream name;
         name << "TIC" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<yApi::historization::CEnergy> temp = boost::make_shared<yApi::historization::CEnergy>(name.str(),
                                                                                                                 yApi::EKeywordAccessMode::kGet);
         m_counterTICList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      // Pulse Counters Configuration
      for (int counter = 0; counter<WES_PULSE_QTY; ++counter)
      {
         std::stringstream name;
         name << "IMP" << std::setfill('0') << std::setw(2) << boost::lexical_cast<int>(counter + 1);
         boost::shared_ptr<yApi::historization::CCounter> temp = boost::make_shared<yApi::historization::CCounter>(name.str(),
                                                                                                                 yApi::EKeywordAccessMode::kGet);
         m_PulseCounterList.push_back(temp);
         keywordsToDeclare.push_back(temp);
      }

      //Déclaration of all IOs
      api->declareDevice(device, m_deviceType, keywordsToDeclare, details);
   }

   std::string CWESEquipment::getDeviceName() const
   {
      return m_deviceName;
   }

   bool CWESEquipment::isMasterDevice() const
   {
      return true;
   }

   std::string CWESEquipment::getDeviceType() const
   {
      return m_deviceType;
   }

   void CWESEquipment::updateFromDevice( boost::shared_ptr<yApi::IYPluginApi> api,
                                         bool forceHistorization)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > keywordsToHistorize;

      //TODO : create the call to the function here

      api->historizeData(m_deviceName, keywordsToHistorize);
   }

   shared::CDataContainer CWESEquipment::buildMessageToDevice(boost::shared_ptr<yApi::IYPluginApi> api, 
                                                                 shared::CDataContainer& parameters, 
                                                                 boost::shared_ptr<const yApi::IDeviceCommand> command)
   {
      std::string keywordName = command->getKeyword();

      //separation of letters and digits
      boost::regex reg("([a-zA-Z]+)(\\d+)");
      boost::smatch match;
/*
      //Set parameters
      if (boost::regex_search(keywordName, match, reg))
      {
         if (match[1] == "R") setParameter(keywordName, m_relaysList, command, match[2], parameters);
         else if (match[1] == "C") setParameter(keywordName, m_countersList, command, command->getBody(), parameters);
         else throw shared::exception::CException("Invalid or Read-Only Keyword:" + keywordName);

         // No command should be received for DI and Analog Values
      }*/

      return parameters;
   }

   CWESEquipment::~CWESEquipment()
   {}
}// namespace equipments