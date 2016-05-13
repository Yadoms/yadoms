#include "stdafx.h"
#include "CartelectronicTIC.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   CCartelectronicTIC::CCartelectronicTIC(const RBUF& rbuf,
                                          size_t rbufSize)
      : m_id(0),
        m_ApparentePower(boost::make_shared<yApi::historization::CApparentPower>("ApparentPower"))
   {
      std::string NameCounter1 = "";
      std::string NameCounter2 = "";

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

      m_Counter1 = boost::make_shared<yApi::historization::CEnergy>(NameCounter1);
      m_Counter2 = boost::make_shared<yApi::historization::CEnergy>(NameCounter2);

      m_Counter1->set((rbuf.TIC.counter1_0 << 24) + (rbuf.TIC.counter1_1 << 16) + (rbuf.TIC.counter1_2 << 8) + (rbuf.TIC.counter1_3));
      m_Counter2->set((rbuf.TIC.counter2_0 << 24) + (rbuf.TIC.counter2_1 << 16) + (rbuf.TIC.counter2_2 << 8) + (rbuf.TIC.counter2_3));

      m_ApparentePower->set((rbuf.TIC.power_H << 8) + rbuf.TIC.power_L);
   }

   CCartelectronicTIC::~CCartelectronicTIC()
   {
   }

   void CCartelectronicTIC::declare(boost::shared_ptr<yApi::IYPluginApi> api,
                                    const std::string& deviceName) const
   {
      // Create keywords if needed
      api->declareKeyword(deviceName, *m_Counter1);
      api->declareKeyword(deviceName, *m_ApparentePower);

      if (m_SubscribeContract != OP_BASE)
         api->declareKeyword(deviceName, *m_Counter2);
   }

   void CCartelectronicTIC::historize(std::vector<boost::shared_ptr<yApi::historization::IHistorizable>>& KeywordList) const
   {
      KeywordList.push_back(m_Counter1);
      KeywordList.push_back(m_ApparentePower);

      if (m_SubscribeContract != OP_BASE)
         KeywordList.push_back(m_Counter2);
   }

   std::string CCartelectronicTIC::idFromProtocol(const RBUF& rbuf) const
   {
      unsigned long long i_id;
      std::stringstream s_id;

      i_id = (static_cast<unsigned long long>(rbuf.TIC.id1) << 32) + (rbuf.TIC.id2 << 24) + (rbuf.TIC.id3 << 16) + (rbuf.TIC.id4 << 8) + (rbuf.TIC.id5);
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


