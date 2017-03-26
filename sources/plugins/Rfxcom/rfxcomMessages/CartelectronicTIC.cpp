#include "stdafx.h"
#include "CartelectronicTIC.h"
#include "boost/lexical_cast.hpp"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CCartelectronicTIC::CCartelectronicTIC(const RBUF& rbuf,
                                          size_t rbufSize)
      : m_id(0),
      m_teleInfoStatus(boost::make_shared<teleInfo::specificHistorizers::CTeleInfoStatus>("TeleInfoStatus")), // Read-only keyword
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
      else if ((rbuf.TIC.state & 0x40) != 0)
      {
         //TeleInfo counter desactivated
         m_teleInfoStatus->set(teleInfo::specificHistorizers::EStatus::kDesactivated);
      }
      else
      {
         // TeleInfo reading ok
         m_teleInfoStatus->set(teleInfo::specificHistorizers::EStatus::kOk);

         // Subscribe Contract
         m_subscribeContract = static_cast<Contract>(rbuf.TIC.contract_type >> 4);
         switch (m_subscribeContract)
         {
         case OP_BASE: NameCounter1 = "BASE";
            break;
         case OP_CREUSE: NameCounter1 = "LowCostHour";
            NameCounter2 = "NormalHour";
            break;
         case OP_EJP: NameCounter1 = "EJPPM";
            NameCounter2 = "EJPHn";
            break;
         case OP_TEMPO:

            // Forecast information
            m_forecast = boost::make_shared<teleInfo::specificHistorizers::CColor>("ForecastTomorrow");
            m_keywords.push_back(m_forecast);
            m_forecast->set(teleInfo::specificHistorizers::EColor((rbuf.TIC.state & 0x18) >> 3));
            //---------------------

            // Counter dependant of the Period
            switch (rbuf.TIC.contract_type & 0x0f)
            {
            case teleInfo::specificHistorizers::EPeriod::kLowCostBlueDaysValue:
            case teleInfo::specificHistorizers::EPeriod::kNormalCostBlueDaysValue:
               NameCounter1 = "LowCostBLUE";
               NameCounter2 = "NormalCostBLUE";
               break;
            case teleInfo::specificHistorizers::EPeriod::kLowCostWhiteDaysValue:
            case teleInfo::specificHistorizers::EPeriod::kNormalCostWhiteDaysValue:
               NameCounter1 = "LowCostWHITE";
               NameCounter2 = "NormalCostWHITE";
               break;
            case teleInfo::specificHistorizers::EPeriod::kLowCostRedDaysValue:
            case teleInfo::specificHistorizers::EPeriod::kNormalCostRedDaysValue:
               NameCounter1 = "LowCostRED";
               NameCounter2 = "NormalCostRED";
               break;
            }
            break;
         default:
            throw shared::exception::COutOfRange("Cartelectronic : SubscribeContract is not supported");
         }
         //----------

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

         //Period time
         m_period = boost::make_shared<teleInfo::specificHistorizers::CPeriod>("RunningPeriod");
         m_keywords.push_back(m_period);
         m_period->set(teleInfo::specificHistorizers::EPeriod(rbuf.TIC.contract_type & 0x0F));
         //---------
      }
   }

   CCartelectronicTIC::~CCartelectronicTIC()
   {
   }

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& CCartelectronicTIC::keywords() const
   {
      return m_keywords;
   }

   std::string CCartelectronicTIC::idFromProtocol(const RBUF& rbuf) const
   {
      unsigned long long i_id;
      std::stringstream s_id;

      i_id = (static_cast<unsigned long long>(rbuf.TIC.id1) << 32) + 
             (static_cast<unsigned long long>(rbuf.TIC.id2) << 24) + 
             (static_cast<unsigned long long>(rbuf.TIC.id3) << 16) + 
             (static_cast<unsigned long long>(rbuf.TIC.id4) << 8) + 
             static_cast<unsigned long long>(rbuf.TIC.id5);

      s_id << std::setfill('0') << std::setw(12) << i_id;

      return s_id.str();
   }

   char CCartelectronicTIC::BatteryLevelFromProtocol(const RBUF& rbuf)
   {
      return rbuf.TIC.battery_level;
   }

   char CCartelectronicTIC::signalPowerFromProtocol(const RBUF& rbuf)
   {
      return rbuf.TIC.signalPower;
   }

   std::string CCartelectronicTIC::getModel() const
   {
      return "TeleInfo Module";
   }
} // namespace rfxcomMessages