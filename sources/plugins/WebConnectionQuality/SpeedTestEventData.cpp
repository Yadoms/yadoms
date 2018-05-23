#include "stdafx.h"
#include "SpeedTestEventData.h"


CSpeedTestEventData::CSpeedTestEventData(int returnCode,
                                         const std::string& error)
   : m_returnCode(returnCode),
     m_error(error)
{
}

CSpeedTestEventData::~CSpeedTestEventData()
{
}

bool CSpeedTestEventData::succes() const
{
   return m_returnCode == 0;
}

int CSpeedTestEventData::returnCode() const
{
   return m_returnCode;
}

const std::string& CSpeedTestEventData::error() const
{
   return m_error;
}

