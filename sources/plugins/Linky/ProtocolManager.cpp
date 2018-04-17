#include "stdafx.h"
#include "ProtocolManager.h"

CProtocolManager::CProtocolManager():
   m_firstRun(true),
   m_protocoleSelected(Standard)
{}

void CProtocolManager::validateProtocol()
{
   m_firstRun = false;
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