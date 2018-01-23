#include "stdafx.h"
#include "DefaultEquipment.h"
#include <shared/DataContainer.h>

namespace equipments
{
   // TODO : Mettre en place keyword SignalLevel/snr/rssi

   CDefaultEquipment::CDefaultEquipment(const std::string& name,
                                        const std::string& devEUID) :
	   m_messageKeyword(boost::make_shared<yApi::historization::CText>("message",
                                                                      yApi::EKeywordAccessMode::kGetSet)),
      m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
      m_rssi(boost::make_shared<yApi::historization::CRssi>("rssi")),
      m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_snr(boost::make_shared<specificHistorizers::CSNR>("snr")),
	   m_name(name),
      m_devEUI(devEUID)
   {}

   CDefaultEquipment::CDefaultEquipment(const std::string& name,
                                        const std::string& devEUID,
                                        boost::shared_ptr<yApi::IYPluginApi> api) :
      m_messageKeyword(boost::make_shared<yApi::historization::CText>("message",
                                                                      yApi::EKeywordAccessMode::kGetSet)),
      m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
      m_rssi(boost::make_shared<yApi::historization::CRssi>("rssi")),
      m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_snr(boost::make_shared<specificHistorizers::CSNR>("snr")),
      m_name(name),
      m_devEUI(devEUID)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> keywordsToDeclare = {
         m_batteryLevel, 
         m_messageKeyword,
         m_rssi,
         m_signalPower,
         m_snr
      };

      // Save names into details
      shared::CDataContainer details;
      details.set("devEUI", m_devEUI);
      details.set("id", "");            // initial value -> no id defined

      api->declareDevice(m_name, "Orange Business", "Orange Business", keywordsToDeclare, details);
   }

   std::string CDefaultEquipment::getName() const
   {
      return m_name;
   }

   std::string CDefaultEquipment::getEUI() const
   {
      return m_devEUI;
   }

   CDefaultEquipment::~CDefaultEquipment()
   {}

   void CDefaultEquipment::updatelastMessageId(boost::shared_ptr<yApi::IYPluginApi> api,
                                               const std::string& id)
   {
      shared::CDataContainer details;
      details.set("devEUI", m_devEUI);
      details.set("id", id);

      api->updateDeviceDetails(m_name, details);
   }

   std::string CDefaultEquipment::getlastMessageId(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      return api->getDeviceDetails(m_name).get<std::string>("id");
   }

   void CDefaultEquipment::updateData(boost::shared_ptr<yApi::IYPluginApi> api,
                                      const std::string& data,
                                      const double& rssi,
                                      const int signalLevel,
                                      const double& snr)
   {
      std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> keywordsToHistorize = {
         m_messageKeyword,
         m_rssi,
         m_signalPower,
         m_snr
      };

      m_messageKeyword->set(data);
      m_rssi->set(rssi);
      m_signalPower->set(signalLevel);
      m_snr->set(snr);
      api->historizeData(m_name, keywordsToHistorize);
   }

   void CDefaultEquipment::updateBatteryLevel(boost::shared_ptr<yApi::IYPluginApi> api,
									                   int batteryLevel)
   {
      if (batteryLevel > 0 && batteryLevel < 255)
      {
         m_batteryLevel->set(batteryLevel);
         api->historizeData(m_name, m_batteryLevel);
      }
      else
      {
         if (batteryLevel == 0)
            YADOMS_LOG(trace) << "Equipment " << m_name << " is externally powered !";
         if (batteryLevel == 255)
            YADOMS_LOG(trace) << "This equipment " << m_name << " is not able to mesure the battery level !";
      }
   }

   void CDefaultEquipment::removeFromYadoms(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      YADOMS_LOG(information) << "Delete device " << m_name;
      api->removeDevice(m_name);
   }
}