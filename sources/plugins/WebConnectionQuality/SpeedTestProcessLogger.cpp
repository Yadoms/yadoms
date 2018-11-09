#include "stdafx.h"
#include "SpeedTestProcessLogger.h"
#include <shared/Log.h>
#include <shared/exception/NotImplemented.hpp>


CSpeedTestProcessLogger::CSpeedTestProcessLogger(const std::string& logPrefix)
   : m_logPrefix(logPrefix)
{
}

CSpeedTestProcessLogger::~CSpeedTestProcessLogger()
{
}

void CSpeedTestProcessLogger::init()
{
}

void CSpeedTestProcessLogger::information(const std::string& line)
{
   YADOMS_LOG(information) << m_logPrefix << line;
   m_stdoutContent = line;
}

void CSpeedTestProcessLogger::error(const std::string& line)
{
   YADOMS_LOG(error) << m_logPrefix << line;
}

void CSpeedTestProcessLogger::purgeLogFile()
{
   throw shared::exception::CNotImplemented("CSpeedTestProcessLogger::purgeLogFile");
}

const std::string& CSpeedTestProcessLogger::stdoutContent() const
{
   return m_stdoutContent;
}
