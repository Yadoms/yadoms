#include "stdafx.h"
#include "Power.h"
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CPower::CPower(boost::shared_ptr<yApi::IYPluginApi> api,
                  const RBUF& rbuf,
                  size_t rbufSize)
      : m_voltage(boost::make_shared<yApi::historization::CVoltage>("voltage")),
        m_current(boost::make_shared<yApi::historization::CCurrent>("current")),
        m_instantPower(boost::make_shared<yApi::historization::CPower>("instant")),
        m_totalPower(boost::make_shared<yApi::historization::CPower>("total")),
        m_powerFactor(boost::make_shared<yApi::historization::CPowerFactor>("powerFactor")),
        m_frequency(boost::make_shared<yApi::historization::CFrequency>("frequency")),
        m_signalPower(boost::make_shared<yApi::historization::CSignalPower>("signalPower")),
        m_keywords({m_voltage , m_current , m_instantPower , m_totalPower , m_powerFactor , m_frequency , m_signalPower})
   {
      CheckReceivedMessage(rbuf,
                           rbufSize,
                           pTypePOWER,
                           DONT_CHECK_SUBTYPE,
                           GET_RBUF_STRUCT_SIZE(POWER),
                           DONT_CHECK_SEQUENCE_NUMBER);

      m_subType = rbuf.POWER.subtype;

      m_id = rbuf.POWER.id1 | (rbuf.POWER.id2 << 8);

      m_voltage->set(rbuf.POWER.voltage);

      m_current->set((rbuf.POWER.currentH << 8 | rbuf.POWER.currentL) / 100.0);

      m_instantPower->set((rbuf.POWER.powerH << 8 | rbuf.POWER.powerL) / 10.0);

      m_totalPower->set((rbuf.POWER.energyH << 8 | rbuf.POWER.energyL) / 100.0);

      m_powerFactor->set(rbuf.POWER.pf / 100.0);

      m_frequency->set(rbuf.POWER.freq);

      m_signalPower->set(NormalizesignalPowerLevel(rbuf.POWER.rssi));

      Init(api);
   }

   CPower::~CPower()
   {
   }

   void CPower::Init(boost::shared_ptr<yApi::IYPluginApi> api)
   {
      // Build device description
      buildDeviceModel();
      buildDeviceName();

      // Create device and keywords if needed
      if (!api->deviceExists(m_deviceName))
      {
         shared::CDataContainer details;
         details.set("type", pTypePOWER);
         details.set("subType", m_subType);
         details.set("id", m_id);
         api->declareDevice(m_deviceName, m_deviceModel, m_deviceModel, m_keywords, details);
         YADOMS_LOG(information) << "New device : " << m_deviceName << " (" << m_deviceModel << ")";
         details.printToLog(YADOMS_LOG(information));
      }
   }

   boost::shared_ptr<std::queue<shared::communication::CByteBuffer>> CPower::encode(boost::shared_ptr<ISequenceNumber> seqNumberProvider) const
   {
      throw shared::exception::CInvalidParameter("Power is a read-only message, can not be encoded");
   }

   void CPower::historizeData(boost::shared_ptr<yApi::IYPluginApi> api) const
   {
      api->historizeData(m_deviceName, m_keywords);
   }

   const std::string& CPower::getDeviceName() const
   {
      return m_deviceName;
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CPower::keywords()
   {
      return m_keywords;
   }

   void CPower::buildDeviceName()
   {
      std::ostringstream ssdeviceName;
      ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
      m_deviceName = ssdeviceName.str();
   }

   void CPower::buildDeviceModel()
   {
      std::ostringstream ssModel;

      switch (m_subType)
      {
      case sTypeELEC5: ssModel << "Revolt";
         break;
      default: ssModel << boost::lexical_cast<std::string>(m_subType);
         break;
      }

      m_deviceModel = ssModel.str();
   }
} // namespace rfxcomMessages
