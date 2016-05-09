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

CCartelectronic::CCartelectronic(boost::shared_ptr<yApi::IYPluginApi> context, const RBUF& rbuf, size_t rbufSize, boost::shared_ptr<const ISequenceNumberProvider> seqNumberProvider)
{
   createSubType(rbuf.TIC.subtype, rbuf, rbufSize);
   //m_id = m_subTypeManager->idFromProtocol(rbuf.CHIME.id1, rbuf.CHIME.id2, rbuf.CHIME.sound);
   //m_subTypeManager->setFromProtocolState(rbuf.CHIME.sound);
   declare(context);
}

CCartelectronic::~CCartelectronic()
{
}

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

   // Build device description
   buildDeviceName();

   // Create device and keywords if needed
   if (!context->deviceExists(m_deviceName))
   {
      shared::CDataContainer details;
      details.set("type", pTypeCARTELECTRONIC);
      details.set("subType", m_subType);
      details.set("id", m_id);
      context->declareDevice(m_deviceName, getModel(), details);

      m_subTypeManager->declare(context, m_deviceName);
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
   m_subTypeManager->historize(context, m_deviceName);
}

const std::string& CCartelectronic::getDeviceName() const
{
   return m_deviceName;
}                     

void CCartelectronic::buildDeviceName()
{
   std::ostringstream ssdeviceName;
   //TODO : JMB -> A modifier
   ssdeviceName << static_cast<unsigned int>(m_subType) << "." << static_cast<unsigned int>(m_id);
   m_deviceName = ssdeviceName.str();
}

} // namespace rfxcomMessages