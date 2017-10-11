#include "stdafx.h"
#include "CartelectronicLinky.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CCartelectronicLinky::CCartelectronicLinky(const RBUF& rbuf,
                                                  size_t rbufSize)
      : m_id(0),
        m_run(boost::make_shared<yApi::historization::CCounter>("run")),
        m_prod(boost::make_shared<yApi::historization::CCounter>("prod")),
        m_voltage(boost::make_shared<yApi::historization::CVoltage>("voltage")),
        m_power(boost::make_shared<yApi::historization::CPower>("power")),
        m_keywords({m_run, m_prod, m_voltage, m_power})
   {
      CheckReceivedMessage(rbuf, rbufSize, pTypeCARTELECTRONIC, sTypeCEencoder, GET_RBUF_STRUCT_SIZE(LINKY), DONT_CHECK_SEQUENCE_NUMBER);

      m_run->set((rbuf.LINKY.runidx_0 << 24) + (rbuf.LINKY.runidx_1 << 16) + (rbuf.LINKY.runidx_2 << 8) + (rbuf.LINKY.runidx_3));
      m_prod->set((rbuf.LINKY.prodidx1_0 << 24) + (rbuf.LINKY.prodidx1_1 << 16) + (rbuf.LINKY.prodidx1_2 << 8) + (rbuf.LINKY.prodidx1_3));
      m_voltage->set(rbuf.LINKY.av_voltage);
      m_power->set(rbuf.LINKY.power_H << 8 + rbuf.LINKY.power_L);
   }

   CCartelectronicLinky::~CCartelectronicLinky()
   {
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CCartelectronicLinky::keywords() const
   {
      return m_keywords;
   }

   std::string CCartelectronicLinky::idFromProtocol(const RBUF& rbuf) const
   {
      std::stringstream s_id;

      auto i_id = static_cast<unsigned long>(rbuf.LINKY.id1 << 24) + (rbuf.LINKY.id2 << 16) + (rbuf.LINKY.id3 << 8) + rbuf.LINKY.id4;
      s_id << static_cast<unsigned long>(i_id);

      return s_id.str();
   }

   char CCartelectronicLinky::BatteryLevelFromProtocol(const RBUF& rbuf)
   {
      return rbuf.LINKY.battery_level;
   }

   char CCartelectronicLinky::signalPowerFromProtocol(const RBUF& rbuf)
   {
      return rbuf.LINKY.rssi;
   }

   std::string CCartelectronicLinky::getModel() const
   {
      return "Encoder Module";
   }
} // namespace rfxcomMessages
