#include "stdafx.h"
#include "CartelectronicLinky.h"
#include "boost/lexical_cast.hpp"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CCartelectronicLinky::CCartelectronicLinky(const RBUF& rbuf,
                                              size_t rbufSize)
      : m_id(0),
      m_teleInfoStatus(boost::make_shared<teleInfo::specificHistorizers::CTeleInfoStatus>("TeleInfoStatus")),
      m_keywords({ m_teleInfoStatus })
   {
      std::string NameCounter1;
      std::string NameCounter2;

      CheckReceivedMessage(rbuf, rbufSize, pTypeCARTELECTRONIC, sTypeTIC, GET_RBUF_STRUCT_SIZE(TIC), DONT_CHECK_SEQUENCE_NUMBER);

      if ((rbuf.TIC.state & 0x04) != 0)
      {
         //TeleInfo reading error
         m_teleInfoStatus->set(teleInfo::specificHistorizers::EStatus::kError);
      }
      else
      {
         // TeleInfo reading ok
         m_teleInfoStatus->set(teleInfo::specificHistorizers::EStatus::kOk);

         // Forecast information
         m_forecast = boost::make_shared<teleInfo::specificHistorizers::CColor>("ForecastTomorrow");
         m_keywords.push_back(m_forecast);
         m_forecast->set(teleInfo::specificHistorizers::EColor((rbuf.TIC.state & 0x18) >> 3));

         // Counter 1
         m_counter1 = boost::make_shared<yApi::historization::CEnergy>(NameCounter1);
         m_keywords.push_back(m_counter1);
         m_counter1->set((rbuf.TIC.counter1_0 << 24) + (rbuf.TIC.counter1_1 << 16) + (rbuf.TIC.counter1_2 << 8) + (rbuf.TIC.counter1_3));
         //----------

         // Counter 2
         if (!NameCounter2.empty())
         {
            m_counter2 = boost::make_shared<yApi::historization::CEnergy>(NameCounter2);
            m_keywords.push_back(m_counter2);
            m_counter2->set((rbuf.TIC.counter2_0 << 24) + (rbuf.TIC.counter2_1 << 16) + (rbuf.TIC.counter2_2 << 8) + (rbuf.TIC.counter2_3));
         }
         //---------

         // Apparent power handling
         // Creating and feeding this keyword only when this value is valid
         if ((rbuf.TIC.state & 0x02) != 0)
         {
            m_apparentePower = boost::make_shared<yApi::historization::CApparentPower>("ApparentPower");
            m_keywords.push_back(m_apparentePower);
            m_apparentePower->set(boost::lexical_cast<double>((rbuf.TIC.power_H << 8) + rbuf.TIC.power_L));
         }
         //---------
      }
   }

   CCartelectronicLinky::~CCartelectronicLinky()
   {
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CCartelectronicLinky::keywords() const
   {
      return m_keywords;
   }

   std::string CCartelectronicLinky::idFromProtocol(const RBUF& rbuf) const
   {
      unsigned long counterId;
      unsigned int counterYear;
      unsigned int manufacturerId;
      unsigned int counterType;
      std::stringstream s_id;

      counterId = (static_cast<unsigned long>(rbuf.TIC.id2 & 0x0f) << 16) +
             (static_cast<unsigned long>(rbuf.TIC.id3) << 8) + 
             static_cast<unsigned long>(rbuf.TIC.id4);

      s_id << std::setfill('0') << std::setw(6) << counterId;

      return s_id.str();
   }

   char CCartelectronicLinky::BatteryLevelFromProtocol(const RBUF& rbuf)
   {
      return rbuf.TIC.battery_level;
   }

   char CCartelectronicLinky::signalPowerFromProtocol(const RBUF& rbuf)
   {
      return rbuf.TIC.rssi;
   }

   std::string CCartelectronicLinky::getModel() const
   {
      return "Linky Module";
   }
} // namespace rfxcomMessages