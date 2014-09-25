#include "stdafx.h"
#include "OpenZWaveNode.h"
#include <shared/Log.h>
#include "FatalErrorException.h"


COpenZWaveNode::COpenZWaveNode()
   :m_homeId(0), m_nodeId(0)
{
   throw shared::exception::CException("Should not happen");
}

COpenZWaveNode::COpenZWaveNode(const uint32 homeId, const uint8 nodeId)
   :m_homeId(homeId), m_nodeId(nodeId)
{
}

COpenZWaveNode::~COpenZWaveNode()
{
}

boost::shared_ptr<COpenZWaveNodeCapacity> COpenZWaveNode::getCapacity(ECommandClass classIdentifier)
{
   if (m_nodeCapacities.find(classIdentifier) == m_nodeCapacities.end())
      registerCapacity(classIdentifier);
   return m_nodeCapacities[classIdentifier];
}

void COpenZWaveNode::registerCapacity(ECommandClass classIdentifier)
{
   if (m_nodeCapacities.find(classIdentifier) == m_nodeCapacities.end())
      m_nodeCapacities[classIdentifier] = boost::shared_ptr<COpenZWaveNodeCapacity>(new COpenZWaveNodeCapacity(m_homeId, m_nodeId, classIdentifier));
}

void COpenZWaveNode::registerKeyword(ECommandClass classIdentifier, const std::string & keyword, OpenZWave::ValueID & value)
{
   getCapacity(classIdentifier)->registerKeyword(keyword, value);
}

//OpenZWave::ValueID& COpenZWaveNode::getValueId(ECommandClass classIdentifier, const std::string & keyword)
//{
//   return getCapacity(classIdentifier)->getValueId(keyword);
//}

bool COpenZWaveNode::sendCommand(ECommandClass classIdentifier, const std::string & keyword, const std::string & commandData)
{
   if (m_nodeCapacities.find(classIdentifier) == m_nodeCapacities.end())
      throw shared::exception::CException("The capacity is not registered for this zwave node");
   return m_nodeCapacities[classIdentifier]->sendCommand(keyword, commandData);
}

const shared::plugin::yadomsApi::commands::IHistorizable & COpenZWaveNode::getLastKeywordValue(ECommandClass classIdentifier, const std::string & keyword)
{
   if (m_nodeCapacities.find(classIdentifier) == m_nodeCapacities.end())
      throw shared::exception::CException("The capacity is not registered for this zwave node");
   return m_nodeCapacities[classIdentifier]->getLastKeywordValue(keyword);
}


const bool COpenZWaveNode::match(const uint32 homeId, const uint8 nodeId)
{
   return m_homeId == homeId && m_nodeId == nodeId;
}

const uint32 COpenZWaveNode::getHomeId()
{
   return m_homeId; 
}

const uint8	COpenZWaveNode::getNodeId() 
{ 
   return m_nodeId;
}


