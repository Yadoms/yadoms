#include "stdafx.h"
#include "SpeedTestEventData.h"


CSpeedTestEventData::CSpeedTestEventData(int returnCode,
                                         const std::string& error,
                                         const std::string& result)
   : m_returnCode(returnCode),
     m_error(error),
     m_result(result)
{
}

CSpeedTestEventData::~CSpeedTestEventData()
{
}

bool CSpeedTestEventData::success() const
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

const std::string& CSpeedTestEventData::result() const
{
   return m_result;
}
