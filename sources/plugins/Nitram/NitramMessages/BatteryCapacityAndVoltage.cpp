#include "stdafx.h"
#include "BatteryCapacityAndVoltage.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include "MessagesDefinition.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace nitramMessages
{
   CBatteryCapacityAndVoltage::CBatteryCapacityAndVoltage(std::string deviceName)
      : m_deviceName(deviceName),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("Level")),
        m_batteryVoltage(boost::make_shared<yApi::historization::CVoltage>("Voltage")),
        m_keywords({ m_batteryVoltage , m_batteryLevel})
   {}

   void CBatteryCapacityAndVoltage::decode(boost::shared_ptr<yApi::IYPluginApi> api,
                                           const shared::communication::CByteBuffer& buffer)
   {
      // battery Level
      m_batteryLevel->set(buffer[2]);

      //battery Voltage
      m_batteryVoltage->set(double(buffer[3] + buffer[4] * 256) / 10);

      YADOMS_LOG(information) << "batteryLevel : " << m_batteryLevel->get();
      YADOMS_LOG(information) << "batteryVoltage : " << m_batteryVoltage->get();
   }

   CBatteryCapacityAndVoltage::~CBatteryCapacityAndVoltage()
   {}

   boost::shared_ptr<shared::communication::CByteBuffer> CBatteryCapacityAndVoltage::encode() const
   {
      shared::communication::CByteBuffer message(1);

      // The ID of the message
      message[0] = nitramMessages::ENitramMessageID::BatteryCapacityAndVoltage;
      return boost::make_shared<shared::communication::CByteBuffer>(message);
   }

   void CBatteryCapacityAndVoltage::historizeData(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CBatteryCapacityAndVoltage::keywords() const
   {
      return m_keywords;
   }

   bool CBatteryCapacityAndVoltage::onlyAck() const
   {
      return false;
   }
} // namespace nitramMessages