#include "stdafx.h"
#include "CartelectronicEncoder.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CCartelectronicEncoder::CCartelectronicEncoder( const RBUF& rbuf, size_t rbufSize )
   :m_Counter1(new yApi::historization::CCounter ("counter1")), 
    m_Counter2(new yApi::historization::CCounter ("counter2"))
{
   CheckReceivedMessage(rbuf, rbufSize, pTypeCARTELECTRONIC, sTypeCEencoder, GET_RBUF_STRUCT_SIZE(CEENCODER), DONT_CHECK_SEQUENCE_NUMBER);

   m_Counter1->set( (rbuf.CEENCODER.counter1_0<<24) + (rbuf.CEENCODER.counter1_1<<16) + (rbuf.CEENCODER.counter1_2<<8) + (rbuf.CEENCODER.counter1_3) );
   m_Counter2->set( (rbuf.CEENCODER.counter2_0<<24) + (rbuf.CEENCODER.counter2_1<<16) + (rbuf.CEENCODER.counter2_2<<8) + (rbuf.CEENCODER.counter2_3) );
}

CCartelectronicEncoder::~CCartelectronicEncoder()
{}

void CCartelectronicEncoder::declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   // Create device and keywords if needed
   context->declareKeyword(deviceName, *m_Counter1);
   context->declareKeyword(deviceName, *m_Counter2);
}

void CCartelectronicEncoder::historize(std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > &KeywordList) const
{
   KeywordList.push_back ( m_Counter1 );
   KeywordList.push_back ( m_Counter2 );
}

std::string CCartelectronicEncoder::idFromProtocol( const RBUF& rbuf ) const
{
	unsigned long i_id;
	std::stringstream s_id;

	i_id = (unsigned long long)(rbuf.CEENCODER.id1 << 24) + (rbuf.TIC.id2 << 16) + (rbuf.TIC.id3 << 8) + rbuf.TIC.id4;
	s_id << static_cast<unsigned long>(i_id);

	return s_id.str();
}

const char CCartelectronicEncoder::BatteryLevelFromProtocol( const RBUF& rbuf )
{
	return rbuf.CEENCODER.battery_level;
}

const char CCartelectronicEncoder::RssiFromProtocol( const RBUF& rbuf )
{
	return rbuf.CEENCODER.rssi;
}

std::string CCartelectronicEncoder::getModel() const
{
	return "Encoder Module";
}

} // namespace rfxcomMessages