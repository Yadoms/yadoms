#include "stdafx.h"
#include "CartelectronicLinky.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CCartelectronicLinky::CCartelectronicLinky(const RBUF& rbuf,
                                              size_t rbufSize)
      : m_id(0),
        m_teleInfoStatus(boost::make_shared<teleInfo::specificHistorizers::CTeleInfoStatus>("TeleInfoStatus")), // Read-only keyword
        m_voltage(boost::make_shared<yApi::historization::CVoltage>("Voltage")),
        m_power(boost::make_shared<yApi::historization::CPower>("Power")),
        m_forecast(boost::make_shared<teleInfo::specificHistorizers::CColor>("Forecast")),
        m_keywords({m_teleInfoStatus, m_voltage, m_power, m_forecast})
   {
      CheckReceivedMessage(rbuf, rbufSize, pTypeCARTELECTRONIC, sTypeLinky, GET_RBUF_STRUCT_SIZE(LINKY), DONT_CHECK_SEQUENCE_NUMBER);

      static const BYTE TeleInfoPresentMask = 0x04;
      if (rbuf.LINKY.state & TeleInfoPresentMask)
      {
         // TeleInfo reading error
         m_teleInfoStatus->set(teleInfo::specificHistorizers::EStatus::kError);
         return;
      }

      static const BYTE ProductionCounterDisableMask = 0x40;
      if (rbuf.LINKY.state & ProductionCounterDisableMask)
      {
         // TeleInfo counter desactivated
         m_teleInfoStatus->set(teleInfo::specificHistorizers::EStatus::kDesactivated);
         return;
      }

      // TeleInfo reading ok
      m_teleInfoStatus->set(teleInfo::specificHistorizers::EStatus::kOk);

      // Current counter
      static const unsigned int NbMaxCounters = 10; // Linky max counters number
      if (rbuf.LINKY.currentidx >= NbMaxCounters)
      {
         // TeleInfo reading error
         m_teleInfoStatus->set(teleInfo::specificHistorizers::EStatus::kError);
         return;
      }
      auto counter = boost::make_shared<yApi::historization::CCounter>("Counter" + std::to_string(rbuf.LINKY.currentidx));
      counter->set((rbuf.LINKY.runidx_0 << 24) + (rbuf.LINKY.runidx_1 << 16) + (rbuf.LINKY.runidx_2 << 8) + (rbuf.LINKY.runidx_3));
      m_keywords.push_back(counter);

      // Voltage
      m_voltage->set(rbuf.LINKY.av_voltage);

      // Power
      m_power->set((rbuf.LINKY.power_H << 8) + rbuf.LINKY.power_L);

      // Forecast
      static const BYTE ForecastMask = 0x18;
      enum
         {
            undefined = 0x00,
            white = 0x08,
            blue = 0x10,
            red = 0x11
         };
      switch (rbuf.LINKY.state & ForecastMask)
      {
      case white: m_forecast->set(teleInfo::specificHistorizers::EColor::kWHITE);
         break;
      case blue: m_forecast->set(teleInfo::specificHistorizers::EColor::kBLUE);
         break;
      case red: m_forecast->set(teleInfo::specificHistorizers::EColor::kRED);
         break;
      default: m_forecast->set(teleInfo::specificHistorizers::EColor::kNOTDEFINED);
         break;
      }
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
      std::stringstream s;

      auto id = static_cast<unsigned long>(rbuf.LINKY.id1 << 24) + (rbuf.LINKY.id2 << 16) + (rbuf.LINKY.id3 << 8) + rbuf.LINKY.id4;
      s << id;

      return s.str();
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


