#include "stdafx.h"
#include "CartelectronicEncoder.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//TODO : Voir pour l'intégration de l'encoder dans la même classe

namespace rfxcomMessages
{

CCartelectronicEncoder::CCartelectronicEncoder( const RBUF& rbuf, size_t rbufSize )
   :m_Counter1("counter1"), m_Counter2("counter2"), m_rssi("rssi")
{
   m_Counter1.set(0);
   m_Counter2.set(0);

   CheckReceivedMessage(rbuf, rbufSize, pTypeCARTELECTRONIC, sTypeCEencoder, GET_RBUF_STRUCT_SIZE(CEENCODER), DONT_CHECK_SEQUENCE_NUMBER);

   //Mettre en place ici

   /*m_subType = rbuf.TIC.subtype;
   m_id = (rbuf.TIC.id1 << 16) | (rbuf.TIC.id2 << 8) | rbuf.TIC.id3;
   m_houseCode = rbuf.TIC.housecode;
   m_unitCode = rbuf.TIC.unitcode;
   m_state.set(fromProtocolState(rbuf.TIC.cmnd));*/
   m_rssi.set(NormalizeRssiLevel(rbuf.CEENCODER.rssi));
}

CCartelectronicEncoder::~CCartelectronicEncoder()
{}

void CCartelectronicEncoder::declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   // Create device and keywords if needed
   if (!context->deviceExists(deviceName))
   {
      shared::CDataContainer details;
      //details.set("type", pTypeHomeConfort);
      //details.set("subType", m_subType);
      //details.set("id", m_id);

      //context->declareDevice(deviceName, m_deviceModel, details);

      context->declareKeyword(deviceName, m_Counter1);
	  context->declareKeyword(deviceName, m_Counter2);
	  context->declareKeyword(deviceName, m_rssi);
   }
}

void CCartelectronicEncoder::historize(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const
{
   context->historizeData(deviceName, m_Counter1);
   context->historizeData(deviceName, m_Counter2);
   context->historizeData(deviceName, m_rssi);
}

unsigned int CCartelectronicEncoder::idFromProtocol(unsigned char id1, unsigned char id2, unsigned char sound)
{
	return 0;
}

void CCartelectronicEncoder::setFromProtocolState(unsigned char cmd)
{
/*
   switch(protocolState)
   {
   case HomeConfort_sOn:
   case HomeConfort_sGroupOn:
      return true;

   case HomeConfort_sOff: 
   case HomeConfort_sGroupOff:
      return false;

   default:
      throw shared::exception::CInvalidParameter("state, " + boost::lexical_cast<std::string>(protocolState));
   }*/
}

} // namespace rfxcomMessages