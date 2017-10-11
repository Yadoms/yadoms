#include "stdafx.h"
#include "Current.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CCurrent::CCurrent(boost::shared_ptr<yApi::IYPluginApi> api,
                      const RBUF& rbuf,
                      size_t rbufSize)
      : m_current1(boost::make_shared<yApi::historization::CCurrent>("channel_1")),
        m_current2(boost::make_shared<yApi::historization::CCurrent>("channel_2")),
        m_current3(boost::make_shared<yApi::historization::CCurrent>("channel_3")),
        m_batteryLevel(boost::make_shared<yApi::historization::CBatteryLevel>("battery")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_current1 , m_current2, m_current3, m_batteryLevel , m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeCURRENT,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(CURRENT),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.CURRENT.subtype;

      m_id = rbuf.CURRENT.id1 | (rbuf.CURRENT.id2 << 8);

      m_current1->set(rbuf.CURRENT.ch1h << 8 | rbuf.CURRENT.ch1l);
      m_current2->set(rbuf.CURRENT.ch2h << 8 | rbuf.CURRENT.ch2l);
      m_current3->set(rbuf.CURRENT.ch3h << 8 | rbuf.CURRENT.ch3l);

      m_batteryLevel->set(NormalizeBatteryLevel(rbuf.CURRENT.battery_level));
      m_signalPower->set(NormalizesignalPowerLevel(rbuf.CURRENT.rssi));

      Init(api);
   }

   CCurrent::~CCurrent()
   {
   }

   void CCurrent::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypeCURRENT);
         details.set("subType", m_subType);
         details.set("id", m_id);
         api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, details);
         YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
         details.printToLog(YADOMS_LOG(information));
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CCurrent::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("Current is a read-only message, can not be encoded");
   }

   void CCurrent::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CCurrent::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CCurrent::keywords()
   {
      return m_keywords;
   }

   void CCurrent::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CCurrent::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeELEC1: ssModel << "CM113, Electrisave";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }
} // namespace rfxcomMessages
