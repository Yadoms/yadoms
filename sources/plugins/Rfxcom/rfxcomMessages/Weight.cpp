#include "stdafx.h"
#include "Weight.h"
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CWeight::CWeight(boost::shared_ptr<yApi::IYPluginApi> api,
                    const RBUF& rbuf,
                    size_t rbufSize)
      : m_weight(boost::make_shared<yApi::historization::CWeight>("weight")),
      m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
      m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
      m_keywords({ m_weight , m_batteryLevel , m_signalPower })
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeWEIGHT,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(WEIGHT),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.WEIGHT.subtype;

      m_id = rbuf.WEIGHT.id1 | (rbuf.WEIGHT.id2 << 8);

      m_weight->set((rbuf.WEIGHT.weighthigh << 8 | rbuf.WEIGHT.weightlow) / 10.0);

      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.WEIGHT.filler)); // In SDK specification battery_level is at filler location
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.WEIGHT.rssi));

      Init(api);
   }

   CWeight::~CWeight()
   {
   }

   void CWeight::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeWEIGHT);
         details.set("subType", m_subType);
         details.set("id", m_id);
         api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, details);
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CWeight::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("Weight is a read-only message, can not be encoded");
   }

   void CWeight::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CWeight::getDeviceName() const
   {
      return m_deviceName;
   }

   void CWeight::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CWeight::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeWEIGHT1: ssModel << "BWR101/102";
         break;
      case sTypeWEIGHT2: ssModel << "GR101";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }
} // namespace rfxcomMessages


