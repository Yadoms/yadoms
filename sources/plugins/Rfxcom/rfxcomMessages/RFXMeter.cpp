#include "stdafx.h"
#include "RFXMeter.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CRFXMeter::CRFXMeter(boost::shared_ptr<yApi::IYPluginApi> api,
                        const RBUF& rbuf,
                        size_t rbufSize)
      : m_counter(boost::make_shared<yApi::historization::CCounter>("counter")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_counter , m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypeRFXMeter,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(RFXMETER),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.RFXMETER.subtype;

      m_id = rbuf.RFXMETER.id2 | (rbuf.RFXMETER.id1 << 8);

      m_counter->set(rbuf.RFXMETER.count4 | (rbuf.RFXMETER.count3 << 8) | (rbuf.RFXMETER.count2 << 16) | (rbuf.RFXMETER.count1 << 24));

      m_signalPower->set(NormalizeSignalPowerLevel(rbuf.RFXMETER.rssi));

      buildDeviceModel();
      buildDeviceName();
   }

   CRFXMeter::~CRFXMeter()
   {
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CRFXMeter::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("RFXMeter is a read-only message, can not be encoded");
   }

   void CRFXMeter::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      if (m_subType != sTypeRFXMeterCount)
      {
         YADOMS_LOG(information) << "RFXMeter subtype " << m_subType << " actually not supported. Please report to Yadoms development team if needed";
         return;
      }
      api->historizeData(m_deviceName, m_keywords);
   }

   void CRFXMeter::filter() const
   {
   }

   void CRFXMeter::declareDevice(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      boost::shared_ptr<shared::CDataContainer> details = shared::CDataContainer::make();
      details->set("type", pTypeRFXMeter);
      details->set("subType", m_subType);
      details->set("id", m_id);
      api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, details);
      YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
      details->printToLog(YADOMS_LOG(information));
   }

   const std::string& CRFXMeter::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CRFXMeter::keywords()
   {
      return m_keywords;
   }

   void CRFXMeter::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CRFXMeter::buildDeviceModel()
   {
      // RFXMeter subtype value doesn't really represent a device subtype.
      // So don't use it for model name.
      m_deviceModel = "RFXMeter counter";
   }
} // namespace rfxcomMessages
