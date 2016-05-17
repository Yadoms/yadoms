#include "stdafx.h"
#include "CartelectronicTIC.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CCartelectronicTIC::CCartelectronicTIC(const RBUF& rbuf,
                                          size_t rbufSize)
      : m_id(0),
        m_apparentePower(boost::make_shared<yApi::historization::CApparentPower>("ApparentPower")),
        m_keywords({m_apparentePower})
   {
      std::string NameCounter1;
      std::string NameCounter2;

      CheckReceivedMessage(rbuf, rbufSize, pTypeCARTELECTRONIC, sTypeTIC, GET_RBUF_STRUCT_SIZE(TIC), DONT_CHECK_SEQUENCE_NUMBER);

      m_SubscribeContract = static_cast<Contract>(rbuf.TIC.contract_type >> 4);

      //TODO : Ajouter La période tarifaire en cours et l'historiser
      //TODO : Récupérer le Tag DEMAIN et l'historiser

      switch (m_SubscribeContract)
      {
      case OP_BASE: NameCounter1 = "BASE";
         break;
      case OP_CREUSE: NameCounter1 = "LowCostHour";
         NameCounter2 = "NormalHour";
         break;
      case OP_EJP: NameCounter1 = "EJPHn";
         NameCounter2 = "EJPPM";
         break;
      case OP_TEMPO:
         // Counter dependant of the Period
         switch (rbuf.TIC.contract_type & 0x0f)
         {
         case LowCostBlueDays:
         case NormalCostBlueDays:
            NameCounter1 = "LowCostBLUE";
            NameCounter2 = "NormalCostBLUE";
            break;
         case LowCostWhiteDays:
         case NormalCostWhiteDays:
            NameCounter1 = "LowCostWHITE";
            NameCounter2 = "NormalCostWHITE";
            break;
         case LowCostRedDays:
         case NormalCostRedDays:
            NameCounter1 = "LowCostRED";
            NameCounter2 = "NormalCostRED";
            break;
         }
         break;
      default:
         throw shared::exception::COutOfRange("Cartelectronic : SubscribeContract is not supported");
      }

      m_counter1 = boost::make_shared<yApi::historization::CEnergy>(NameCounter1);
      m_keywords.push_back(m_counter1);
      if (!NameCounter2.empty())
      {
         m_counter2 = boost::make_shared<yApi::historization::CEnergy>(NameCounter2);
         m_keywords.push_back(m_counter2);
      }

      m_counter1->set((rbuf.TIC.counter1_0 << 24) + (rbuf.TIC.counter1_1 << 16) + (rbuf.TIC.counter1_2 << 8) + (rbuf.TIC.counter1_3));
      m_counter2->set((rbuf.TIC.counter2_0 << 24) + (rbuf.TIC.counter2_1 << 16) + (rbuf.TIC.counter2_2 << 8) + (rbuf.TIC.counter2_3));

      m_apparentePower->set((rbuf.TIC.power_H << 8) + rbuf.TIC.power_L);
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

      i_id = (static_cast<unsigned long long>(rbuf.TIC.id1) << 32) + (rbuf.TIC.id2 << 24) + (rbuf.TIC.id3 << 16) + (rbuf.TIC.id4 << 8) + (rbuf.TIC.id5);

	// TODO : voir comment compléter à 12 caractères, pour le 0 devant
      s_id << static_cast<unsigned long long>(i_id);

      return s_id.str();
   }

   char CCartelectronicTIC::BatteryLevelFromProtocol(const RBUF& rbuf)
   {
      return rbuf.TIC.battery_level;
   }

   char CCartelectronicTIC::RssiFromProtocol(const RBUF& rbuf)
   {
      return rbuf.TIC.rssi;
   }

   std::string CCartelectronicTIC::getModel() const
   {
      return "TeleInfo Module";
   }
} // namespace rfxcomMessages


