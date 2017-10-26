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
        m_todayColor(boost::make_shared<teleInfo::specificHistorizers::CColor>("Today color")),
        m_forecastColor(boost::make_shared<teleInfo::specificHistorizers::CColor>("Forecast color")),
        m_keywords({m_teleInfoStatus, m_voltage, m_power, m_todayColor, m_forecastColor})
   {
      CheckReceivedMessage(rbuf, rbufSize, pTypeCARTELECTRONIC, sTypeLinky, GET_RBUF_STRUCT_SIZE(LINKY), DONT_CHECK_SEQUENCE_NUMBER);

      static const BYTE TeleInfoPresentMask = 0x04;
      if (rbuf.LINKY.state & TeleInfoPresentMask)
      {
         // TeleInfo reading error
         m_teleInfoStatus->set(teleInfo::specificHistorizers::EStatus::kError);
         return;
      }

      // TeleInfo reading ok
      m_teleInfoStatus->set(teleInfo::specificHistorizers::EStatus::kOk);

      static const BYTE ProductionCounterDisableMask = 0x02;
      if (~rbuf.LINKY.state & ProductionCounterDisableMask)
      {
         // Prod counter is enabled
         auto counter = boost::make_shared<yApi::historization::CEnergy>("Production");
         counter->set(((rbuf.LINKY.prodidx1_0 << 24) + (rbuf.LINKY.prodidx1_1 << 16) + (rbuf.LINKY.prodidx1_2 << 8) + (rbuf.LINKY.prodidx1_3) ) * 1000);
         m_keywords.push_back(counter);
      }

      // Current counter
      auto counter = boost::make_shared<yApi::historization::CEnergy>("Counter" + std::to_string(rbuf.LINKY.currentidx));
      counter->set(((rbuf.LINKY.runidx_0 << 24) + (rbuf.LINKY.runidx_1 << 16) + (rbuf.LINKY.runidx_2 << 8) + (rbuf.LINKY.runidx_3) ) * 1000);
      m_keywords.push_back(counter);

      // Voltage
      m_voltage->set(rbuf.LINKY.av_voltage + 200);

      // Power
      m_power->set((rbuf.LINKY.power_H << 8) + rbuf.LINKY.power_L);

      // Forecast
      static const BYTE TodayColorMask = 0x60;
      static const BYTE TodayColorShift = 5;
      static const BYTE ForecastColorMask = 0x18;
      static const BYTE ForecastColorShift = 3;
      enum
         {
            undefined = 0,
            white = 1,
            blue = 2,
            red = 3
         };
      switch ((rbuf.LINKY.state & TodayColorMask) >> TodayColorShift)
      {
      case white: m_todayColor->set(teleInfo::specificHistorizers::EColor::kWHITE);
         break;
      case blue: m_todayColor->set(teleInfo::specificHistorizers::EColor::kBLUE);
         break;
      case red: m_todayColor->set(teleInfo::specificHistorizers::EColor::kRED);
         break;
      default: m_todayColor->set(teleInfo::specificHistorizers::EColor::kNOTDEFINED);
         break;
      }
      switch ((rbuf.LINKY.state & ForecastColorMask) >> ForecastColorShift)
      {
      case white: m_forecastColor->set(teleInfo::specificHistorizers::EColor::kWHITE);
         break;
      case blue: m_forecastColor->set(teleInfo::specificHistorizers::EColor::kBLUE);
         break;
      case red: m_forecastColor->set(teleInfo::specificHistorizers::EColor::kRED);
         break;
      default: m_forecastColor->set(teleInfo::specificHistorizers::EColor::kNOTDEFINED);
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
      return "Linky Module";
   }
} // namespace rfxcomMessages


