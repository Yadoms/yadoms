#include "stdafx.h"
#include "CartelectronicTIC.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CCartelectronicTIC::CCartelectronicTIC( const RBUF& rbuf, size_t rbufSize ) : 
	m_ApparentePower (new yApi::historization::CApparentPower("ApparentPower") )
{
   std::string NameCounter1="";
   std::string NameCounter2="";

   CheckReceivedMessage(rbuf, rbufSize, pTypeCARTELECTRONIC, sTypeTIC, GET_RBUF_STRUCT_SIZE(TIC), DONT_CHECK_SEQUENCE_NUMBER);

   m_SubscribeContract = (Contract) (rbuf.TIC.contract_type >> 4);

   m_Forecast.reset(new teleInfo::CColor("ForecastTomorrow"));

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
		              m_Forecast->set( (teleInfo::EColor) ((rbuf.TIC.state & 0x18 )>>3) );

		              // Counter dependant of the Period
		              switch ( (rbuf.TIC.contract_type & 0x0f) )
					  {
					  case teleInfo::EPeriod::kLowCostBlueDaysValue:
					  case teleInfo::EPeriod::kNormalCostBlueDaysValue:
						  NameCounter1 = "LowCostBLUE";
		                  NameCounter2 = "NormalCostBLUE";
						  break;
					  case teleInfo::EPeriod::kLowCostWhiteDaysValue:
					  case teleInfo::EPeriod::kNormalCostWhiteDaysValue:
						  NameCounter1 = "LowCostWHITE";
		                  NameCounter2 = "NormalCostWHITE";
						  break;
					  case teleInfo::EPeriod::kLowCostRedDaysValue:
					  case teleInfo::EPeriod::kNormalCostRedDaysValue:
						  NameCounter1 = "LowCostRED";
		                  NameCounter2 = "NormalCostRED";
						  break;
					  }
					break;
	  default:
		              throw shared::exception::COutOfRange("Cartelectronic : SubscribeContract is not supported");
		              break;
   }

   m_Counter1.reset (new yApi::historization::CEnergy ( NameCounter1 ));
   m_Counter2.reset (new yApi::historization::CEnergy ( NameCounter2 ));

   m_Period.reset   (new teleInfo::CPeriod("RunningPeriod") );

   m_Counter1->set( (rbuf.TIC.counter1_0<<24) + (rbuf.TIC.counter1_1<<16) + (rbuf.TIC.counter1_2<<8) + (rbuf.TIC.counter1_3) );
   m_Counter2->set( (rbuf.TIC.counter2_0<<24) + (rbuf.TIC.counter2_1<<16) + (rbuf.TIC.counter2_2<<8) + (rbuf.TIC.counter2_3) );

   m_ApparentePower->set ( (rbuf.TIC.power_H << 8) + rbuf.TIC.power_L );
   m_Period->set( (teleInfo::EPeriod) (rbuf.TIC.contract_type & 0x0F) );
}

CCartelectronicTIC::~CCartelectronicTIC()
{}

void CCartelectronicTIC::declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   // TODO : Create Keywords details

   // Create keywords if needed
   context->declareKeyword(deviceName, *m_Counter1);
   context->declareKeyword(deviceName, *m_ApparentePower);
   context->declareKeyword(deviceName, *m_Period);

   if (m_SubscribeContract != OP_BASE)
	  context->declareKeyword(deviceName, *m_Counter2);

   if (m_SubscribeContract == OP_TEMPO)
	  context->declareKeyword(deviceName, *m_Forecast);
}

void CCartelectronicTIC::historize(std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > &KeywordList) const
{
   KeywordList.push_back ( m_Counter1 );
   KeywordList.push_back ( m_ApparentePower );
   KeywordList.push_back ( m_Period );

   if (m_SubscribeContract != OP_BASE)
      KeywordList.push_back ( m_Counter2 );
}

std::string CCartelectronicTIC::idFromProtocol( const RBUF& rbuf ) const
{
	unsigned long long i_id;
	std::stringstream s_id;

	i_id = ((unsigned long long)(rbuf.TIC.id1) << 32) + (rbuf.TIC.id2 << 24) + (rbuf.TIC.id3 << 16) + (rbuf.TIC.id4 << 8) + (rbuf.TIC.id5);

	// TODO : voir comment compléter à 12 caractères, pour le 0 devant
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

std::string CCartelectronicTIC::getModel() const
{
	return "TeleInfo Module";
}

} // namespace rfxcomMessages