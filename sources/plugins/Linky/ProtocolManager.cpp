#include "stdafx.h"
#include "ProtocolManager.h"
#include <shared/Log.h>

CProtocolManager::CProtocolManager():
   m_firstRun(true),
   m_protocoleSelected(Standard)
{}

void CProtocolManager::validateProtocol()
{
   m_firstRun = false;
}

bool CProtocolManager::isValidated()
{
   return !m_firstRun;
}

void CProtocolManager::changeProtocol()
{
   if (m_firstRun)
   {
      if (m_protocoleSelected == Standard)
         m_protocoleSelected = Historic;
      else
         m_protocoleSelected = Standard;
   }
}

EProtocolType CProtocolManager::getProtocol()
{
   return m_protocoleSelected;
}

CProtocolManager::~CProtocolManager()
{}

bool CProtocolManager::end()
{
   if (!m_firstRun){
      YADOMS_LOG(information) << "!firstRun";
      return true;
   }

   if (m_protocoleSelected == Historic)
      return true;
   else
      return false;
}