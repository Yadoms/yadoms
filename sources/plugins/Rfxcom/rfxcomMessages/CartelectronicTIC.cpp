#include "stdafx.h"
#include "CartelectronicTIC.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CCartelectronicTIC::CCartelectronicTIC( const RBUF& rbuf, size_t rbufSize )
{
   std::string NameCounter1="";
   std::string NameCounter2="";

   CheckReceivedMessage(rbuf, rbufSize, pTypeCARTELECTRONIC, sTypeTIC, GET_RBUF_STRUCT_SIZE(TIC), DONT_CHECK_SEQUENCE_NUMBER);

   m_SubscribeContract = (Contract) (rbuf.TIC.contract_type >> 4);

   //TODO : Ajouter La période tarifaire en cours et l'historiser
   //TODO : Récupérer le Tag DEMAIN et l'historiser

   switch ( m_SubscribeContract )
   {
      case OP_BASE:   NameCounter1 = "BASE";
					  break;
	  case OP_CREUSE: NameCounter1 = "LowCostHour";
		              NameCounter2 = "NormalHour";
					  break;
	  case OP_EJP:    NameCounter1 = "EJPHn";
		              NameCounter2 = "EJPPM";
					break;
	  case OP_TEMPO:
		              // Counter dependant of the Period
		              switch ( rbuf.TIC.contract_type & 0x0f)
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
		              break;
   }

   m_Counter1.reset(new yApi::historization::CEnergy ( NameCounter1 ));
   m_Counter2.reset(new yApi::historization::CEnergy ( NameCounter2 ));

   m_Counter1->set( (rbuf.TIC.counter1_3<<24) + (rbuf.TIC.counter1_2<<16) + (rbuf.TIC.counter1_1<<8) + (rbuf.TIC.counter1_0) );
   m_Counter2->set( (rbuf.TIC.counter2_3<<24) + (rbuf.TIC.counter2_2<<16) + (rbuf.TIC.counter2_1<<8) + (rbuf.TIC.counter2_0) );
}

CCartelectronicTIC::~CCartelectronicTIC()
{}

void CCartelectronicTIC::declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   // Create keywords if needed
   context->declareKeyword(deviceName, *m_Counter1);

   if (m_SubscribeContract != OP_BASE)
	 context->declareKeyword(deviceName, *m_Counter2);
}

void CCartelectronicTIC::historize(std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > KeywordList) const
{
   KeywordList.push_back ( m_Counter1 );

   if (m_SubscribeContract != OP_BASE)
      KeywordList.push_back ( m_Counter2 );
}

// TODO : Not working
const std::string& CCartelectronicTIC::idFromProtocol( const RBUF& rbuf )
{
	unsigned long long i_id;
	std::stringstream s_id;

	i_id = (unsigned long long)(rbuf.TIC.id1 << 32) + (rbuf.TIC.id2 << 24) + (rbuf.TIC.id3 << 16) + (rbuf.TIC.id4 << 8) + (rbuf.TIC.id5);
	s_id << static_cast<unsigned long long>(i_id);

	return s_id.str();
}

const char CCartelectronicTIC::BatteryLevelFromProtocol( const RBUF& rbuf )
{
	return rbuf.TIC.battery_level;
}

const char CCartelectronicTIC::RssiFromProtocol( const RBUF& rbuf )
{
	return rbuf.TIC.rssi;
}

} // namespace rfxcomMessages