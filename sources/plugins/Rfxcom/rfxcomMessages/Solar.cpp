#include "stdafx.h"
#include "Solar.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CSolar::CSolar(boost::shared_ptr<yApi::IYPluginApi> api,
                  const RBUF& rbuf,
                  size_t rbufSize)
      : m_illumination(boost::make_shared<yApi::historization::CIlluminationWm2>("illumination")),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({ m_illumination, m_batteryLevel, m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeSOLAR,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(SOLAR),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.SOLAR.subtype;

      m_id = rbuf.SOLAR.id1 | (rbuf.SOLAR.id2 << 8);

      m_illumination->set( (rbuf.SOLAR.solarlow | (rbuf.SOLAR.solarhigh << 8)) / 100.0);

      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.SOLAR.battery_level));
      m_signalPower->set(NormalizeSignalPowerLevel(rbuf.SOLAR.rssi));

      buildDeviceModel();
      buildDeviceName();
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CSolar::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("SOLAR is a read-only message, can not be encoded");
   }

   void CSolar::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CSolar::filter() const
   {
   }

   void CSolar::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      shared::CDataContainer details;
      details.set("type", pTypeSOLAR);
      details.set("subType", m_subType);
      details.set("id", m_id);
      api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, details);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
      details.printToLog(YADOMS_LOG(information));
   }

   const std::string& CSolar::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CSolar::keywords()
   {
      return m_keywords;
   }

   void CSolar::buildDeviceName()
   {
      std::ostringstream deviceName;
      deviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = deviceName.str();
   }

   void CSolar::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeSOLAR1: ssModel << "Davis";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }
} // namespace rfxcomMessages
