#include "stdafx.h"
#include "Cartelectronic.h"
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/Log.h>
#include "CartelectronicTIC.h"
#include "CartelectronicEncoder.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{

CCartelectronic::CCartelectronic(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider):
        	m_rssi(new yApi::historization::CRssi ("rssi")), 
	        m_batteryLevel(new yApi::historization::CBatteryLevel("battery"))
{
   createSubType(rbuf.TIC.subtype, rbuf, rbufSize);
   m_id = m_subTypeManager->idFromProtocol( rbuf );
   m_batteryLevel->set(NormalizeBatteryLevel(m_subTypeManager->BatteryLevelFromProtocol(rbuf)));
   m_rssi->set(NormalizeRssiLevel(m_subTypeManager->RssiFromProtocol(rbuf)));
   declare(context);
}

CCartelectronic::~CCartelectronic()
{}

void CCartelectronic::createSubType( unsigned char subType, const RBUF& rbuf, size_t rbufSize )
{
   m_subType = subType;

   switch(m_subType)
   {
   case sTypeTIC       : m_subTypeManager.reset(new CCartelectronicTIC(rbuf, rbufSize)); break;
   case sTypeCEencoder : m_subTypeManager.reset(new CCartelectronicEncoder(rbuf, rbufSize)); break;
   default:
      throw shared::exception::COutOfRange("Manually device creation : subType is not supported");
   }
}

void CCartelectronic::declare(boost::shared_ptr<yApi::IYPluginApi> context)
{
   if (!m_subTypeManager)
      throw shared::exception::CException("m_subTypeManager must be initialized");

   // Create device and keywords if needed
   if (!context->deviceExists(m_id))
   {
      shared::CDataContainer details;
      details.set("type", pTypeCARTELECTRONIC);
      details.set("subType", m_subType);
      details.set("id", m_id);

      context->declareDevice(m_id, getModel(), details);

      m_subTypeManager->declare(context, m_id);
	  context->declareKeyword(m_id, *m_batteryLevel);
	  context->declareKeyword(m_id, *m_rssi);
   }
}

std::string CCartelectronic::getModel() const
{
   return "Cartelectronic Module";
}

boost::shared_ptr<std::queue<shared::communication::CByteBuffer> > CCartelectronic::encode(boost::shared_ptr<ISequenceNumberProvider> seqNumberProvider) const
{
   throw shared::exception::CInvalidParameter("Cartelectronic is a read-only message, can not be encoded");
}

void CCartelectronic::historizeData(boost::shared_ptr<yApi::IYPluginApi> context) const
{
   std::vector<boost::shared_ptr<yApi::historization::IHistorizable> > KeywordList;

   KeywordList.push_back ( m_batteryLevel );
   KeywordList.push_back ( m_rssi );

   m_subTypeManager->historize( KeywordList );

   context->historizeData(m_id, KeywordList);
}

const std::string& CCartelectronic::getDeviceName() const
{
   return m_id;
}

} // namespace rfxcomMessages