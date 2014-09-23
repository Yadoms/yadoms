#include "stdafx.h"
#include "OpenZWaveNodeCapacity.h"
#include <shared/Log.h>
#include "FatalErrorException.h"
#include "ZWave.h"
#include "OpenZWaveCommandClass.h"
#include "CommandClasses.h"
#include "OpenZWaveNodeKeywordFactory.h"

COpenZWaveNodeCapacity::COpenZWaveNodeCapacity(const int homeId, const int nodeId, const ECommandClass & classIdentifier)
   :m_classIdentifier(classIdentifier), m_class(OpenZWave::CommandClasses::CreateCommandClass(classIdentifier, homeId, nodeId))
{
   
}

COpenZWaveNodeCapacity::~COpenZWaveNodeCapacity()
{
}

boost::shared_ptr<OpenZWave::CommandClass> COpenZWaveNodeCapacity::getCommandClass() 
{
   return m_class;
}

ECommandClass COpenZWaveNodeCapacity::getClassIdentifier()
{
   return m_classIdentifier;
}

bool COpenZWaveNodeCapacity::sendCommand(const std::string & keyword, const std::string & commandData)
{
   if (m_keywords.find(keyword) == m_keywords.end())
      throw shared::exception::CException("The keyword is not registered for this capacity");
   return m_keywords[keyword]->sendCommand(commandData);
}

std::string COpenZWaveNodeCapacity::getLastKeywordValue(const std::string & keyword)
{
   if (m_keywords.find(keyword) == m_keywords.end())
      throw shared::exception::CException("The keyword is not registered for this capacity");
   return m_keywords[keyword]->getLastKeywordValue();
}

void COpenZWaveNodeCapacity::registerKeyword(const std::string & keyword, OpenZWave::ValueID & value)
{
   m_keywords[keyword] = COpenZWaveNodeKeywordFactory::createKeyword(m_classIdentifier, value);
}


