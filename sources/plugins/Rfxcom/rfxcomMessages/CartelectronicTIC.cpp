#include "stdafx.h"
#include "CartelectronicTIC.h"
#include <boost/lexical_cast.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CCartelectronicTIC::CCartelectronicTIC(const RBUF& rbuf,
                                          size_t rbufSize)
      : m_teleInfoStatus(boost::make_shared<teleInfo::specificHistorizers::CTeleInfoStatus>("TeleInfoStatus")), // Read-only keyword
        m_keywords({m_teleInfoStatus})
   {
      std::string nameCounter1;
      std::string nameCounter2;

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
         case OP_BASE: nameCounter1 = "BASE";
            break;
         case OP_CREUSE: nameCounter1 = "LowCostHour";
            nameCounter2 = "NormalHour";
            break;
         case OP_EJP: nameCounter1 = "EJPPM";
            nameCounter2 = "EJPHn";

            // Warning EJP information
            m_warningEJP = boost::make_shared<yApi::historization::CSwitch>("WarningEJP");
            m_keywords.push_back(m_warningEJP);

            if (teleInfo::specificHistorizers::EColor((rbuf.TIC.state & 0x18) >> 3) == teleInfo::specificHistorizers::EColor::kRED)
               m_warningEJP->set(true);
            else
               m_warningEJP->set(false);
            //---------------------
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
               nameCounter1 = "LowCostBLUE";
               nameCounter2 = "NormalCostBLUE";
               break;
            case teleInfo::specificHistorizers::EPeriod::kLowCostWhiteDaysValue:
            case teleInfo::specificHistorizers::EPeriod::kNormalCostWhiteDaysValue:
               nameCounter1 = "LowCostWHITE";
               nameCounter2 = "NormalCostWHITE";
               break;
            case teleInfo::specificHistorizers::EPeriod::kLowCostRedDaysValue:
            case teleInfo::specificHistorizers::EPeriod::kNormalCostRedDaysValue:
               nameCounter1 = "LowCostRED";
               nameCounter2 = "NormalCostRED";
               break;
            default:
               break;
            }
            break;
         default:
            throw shared::exception::COutOfRange("Cartelectronic : SubscribeContract is not supported");
         }
         //----------

         // Counter 1
         m_counter1 = boost::make_shared<yApi::historization::CEnergy>(nameCounter1);
         m_keywords.push_back(m_counter1);
         m_counter1->set((rbuf.TIC.counter1_0 << 24) + (rbuf.TIC.counter1_1 << 16) + (rbuf.TIC.counter1_2 << 8) + (rbuf.TIC.counter1_3));
         //----------

         // Counter 2
         if (!nameCounter2.empty())
         {
            m_counter2 = boost::make_shared<yApi::historization::CEnergy>(nameCounter2);
            m_keywords.push_back(m_counter2);
            m_counter2->set((rbuf.TIC.counter2_0 << 24) + (rbuf.TIC.counter2_1 << 16) + (rbuf.TIC.counter2_2 << 8) + (rbuf.TIC.counter2_3));
         }
         //---------

         // Apparent power handling
         // Creating and feeding this keyword only when this value is valid
         if ((rbuf.TIC.state & 0x02) != 0)
         {
            m_apparentPower = boost::make_shared<yApi::historization::CApparentPower>("ApparentPower");
            m_keywords.push_back(m_apparentPower);
            m_apparentPower->set(boost::lexical_cast<double>((rbuf.TIC.power_H << 8) + rbuf.TIC.power_L));
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

   const std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>>& CCartelectronicTIC::keywords(const shared::CDataContainerSharedPtr& deviceConfiguration)
   {
      return m_keywords;
   }

   std::string CCartelectronicTIC::idFromProtocol(const RBUF& rbuf) const
   {
      std::stringstream sId;

      const auto iId = (static_cast<unsigned long long>(rbuf.TIC.id1) << 32) +
         (static_cast<unsigned long long>(rbuf.TIC.id2) << 24) +
         (static_cast<unsigned long long>(rbuf.TIC.id3) << 16) +
         (static_cast<unsigned long long>(rbuf.TIC.id4) << 8) +
         static_cast<unsigned long long>(rbuf.TIC.id5);

      sId << std::setfill('0') << std::setw(12) << iId;

      return sId.str();
   }

   char CCartelectronicTIC::batteryLevelFromProtocol(const RBUF& rbuf)
   {
      return rbuf.TIC.battery_level;
   }

   char CCartelectronicTIC::signalPowerFromProtocol(const RBUF& rbuf)
   {
      return rbuf.TIC.rssi;
   }

   std::string CCartelectronicTIC::getModel() const
   {
      return "TeleInfo Module";
   }
} // namespace rfxcomMessages