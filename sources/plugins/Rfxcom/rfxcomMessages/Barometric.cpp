#include "stdafx.h"
#include "Barometric.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CBarometric::CBarometric(boost::shared_ptr<yApi::IYPluginApi> api,
                            const RBUF& rbuf,
                            size_t rbufSize)
      : m_pressure(boost::make_shared<yApi::historization::CPressure>("pressure")),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_pressure , m_batteryLevel , m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeBARO,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(BARO),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.BARO.subtype;

      m_id = rbuf.BARO.id1 | (rbuf.BARO.id2 << 8);

      m_pressure->set(rbuf.BARO.baro1 << 8 | (rbuf.BARO.baro2));

      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.BARO.battery_level));
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.BARO.rssi));

      Init(api);
   }

   CBarometric::~CBarometric()
   {
   }

   void CBarometric::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeBARO);
         details.set("subType", m_subType);
         details.set("id", m_id);
         api->declareDevice(m_deviceName,
                            "barometric",
                            m_deviceModel,
                            m_keywords,
                            details);
         YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
         details.printToLog(YADOMS_LOG(information));
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CBarometric::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("Barometric is a read-only message, can not be encoded");
   }

   void CBarometric::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CBarometric::filter() const
   {
   }

   void CBarometric::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CBarometric::keywords()
   {
      return m_keywords;
   }

   const std::string& CBarometric::getDeviceName() const
   {
      return m_deviceName;
   }

   void CBarometric::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CBarometric::buildDeviceModel()
   {
      m_deviceModel = "Barometric sensor";
   }
} // namespace rfxcomMessages
