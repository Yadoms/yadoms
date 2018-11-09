#include "stdafx.h"
#include "UV.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CUV::CUV(boost::shared_ptr<yApi::IYPluginApi> api,
            const RBUF& rbuf,
            size_t rbufSize)
      : m_uv(boost::make_shared<yApi::historization::CUv>("uv")),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_uv , m_temperature , m_batteryLevel , m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeUV,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(UV),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.UV.subtype;

      m_id = rbuf.UV.id1 | (rbuf.UV.id2 << 8);

      m_uv->set(rbuf.UV.uv / 10);

      if (m_subType == sTypeUV3)
      {
         m_temperature = boost::make_shared<yApi::historization::CTemperature>("temperature");
         m_keywords.push_back(m_temperature);

         m_temperature->set(NormalizeTemperature(rbuf.UV.temperatureh, rbuf.UV.temperaturel, rbuf.UV.tempsign == 1));
      }

      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.UV.battery_level));
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.UV.rssi));

      buildDeviceModel();
      buildDeviceName();
   }

   CUV::~CUV()
   {
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CUV::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("UV is a read-only message, can not be encoded");
   }

   void CUV::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   void CUV::filter() const
   {
   }

   void CUV::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      shared::CDataContainer details;
      details.set("type", pTypeUV);
      details.set("subType", m_subType);
      details.set("id", m_id);
      api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, details);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
      details.printToLog(YADOMS_LOG(information));
   }

   const std::string& CUV::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CUV::keywords()
   {
      return m_keywords;
   }

   void CUV::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CUV::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeUV1: ssModel << "UVN128, UV138";
         break;
      case sTypeUV2: ssModel << "UVN800";
         break;
      case sTypeUV3: ssModel << "TFA";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }
} // namespace rfxcomMessages
