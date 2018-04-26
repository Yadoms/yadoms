#include "stdafx.h"
#include "Bbq.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CBbq::CBbq(boost::shared_ptr<yApi::IYPluginApi> api,
              const RBUF& rbuf,
              size_t rbufSize)
      : m_foodTemperature(boost::make_shared<yApi::historization::CTemperature>("food")),
        m_bbqTemperature(boost::make_shared<yApi::historization::CTemperature>("bbq")),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_foodTemperature , m_bbqTemperature , m_batteryLevel , m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeBBQ,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(BBQ),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.BBQ.subtype;

      m_id = rbuf.BBQ.id1 | (rbuf.BBQ.id2 << 8);

      // Don't use NormalizeTemperature, as temperature is given in °C, not in °C/10
      m_foodTemperature->set(static_cast<double>((rbuf.BBQ.sensor1h << 8) | rbuf.BBQ.sensor1l));
      m_bbqTemperature->set(static_cast<double>((rbuf.BBQ.sensor2h << 8) | rbuf.BBQ.sensor2l));
      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.BBQ.battery_level));
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.BBQ.rssi));

      Init(api);
   }

   CBbq::~CBbq()
   {
   }

   void CBbq::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeBBQ);
         details.set("subType", m_subType);
         details.set("id", m_id);
         api->declareDevice(m_deviceName,
                            "maverickET732",
                            m_deviceModel,
                            m_keywords,
                            details);
         YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
         details.printToLog(YADOMS_LOG(information));
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CBbq::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("Temp is a read-only message, can not be encoded");
   }

   void CBbq::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CBbq::filter() const
   {
   }

   void CBbq::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
   }

   const std::string& CBbq::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CBbq::keywords()
   {
      return m_keywords;
   }

   void CBbq::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << m_id;
      m_deviceName = ssdeviceName.str();
   }

   void CBbq::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeBBQ1: ssModel << "Maverick ET-732";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }
} // namespace rfxcomMessages
