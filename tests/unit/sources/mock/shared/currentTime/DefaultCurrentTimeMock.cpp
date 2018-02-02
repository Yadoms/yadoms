#include "stdafx.h"
#include "DefaultCurrentTimeMock.h"
#include "../../../../sources/shared/shared/currentTime/Provider.h"


CDefaultCurrentTimeMock::CDefaultCurrentTimeMock(const std::string& refTime)
   : m_initialRefTime(boost::posix_time::time_from_string(refTime)),
     m_refTime(m_initialRefTime)
{
}

CDefaultCurrentTimeMock::~CDefaultCurrentTimeMock()
{
}

// shared::currentTime::ICurrentTime implementation
boost::posix_time::ptime CDefaultCurrentTimeMock::now() const
{
   return m_refTime;
}

// [END] shared::currentTime::ICurrentTime implementation

void CDefaultCurrentTimeMock::reset(const std::string& refTime)
{
   m_initialRefTime = boost::posix_time::time_from_string(refTime);
   m_refTime = m_initialRefTime;
}

// Move the refTime of a relative duration
// After call : m_refTime = last_m_refTime + duration
void CDefaultCurrentTimeMock::sleep(const boost::posix_time::time_duration& duration)
{
   m_refTime += duration;
}

// Move the refTime of an absolute duration
// After call : m_refTime = m_initialRefTime + duration
void CDefaultCurrentTimeMock::move(const boost::posix_time::time_duration& duration)
{
   m_refTime = m_initialRefTime + duration;
}


boost::shared_ptr<CDefaultCurrentTimeMock> useTimeMock()
{
   auto timeProviderMock = boost::make_shared<CDefaultCurrentTimeMock>();
   shared::currentTime::Provider().setProvider(timeProviderMock);
   return timeProviderMock;
}