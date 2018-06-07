#include "stdafx.h"
#include "DefaultCurrentTimeMock.h"
#include "../../../../sources/shared/shared/currentTime/Provider.h"


//--------------------------------------------------------------
// ! ! ! BE CAREFUL ! ! !
//
// Never test using CEventHandler.waitForEvents function with a timeout
// This function uses some system functions relating on real time (not depending on our time provider)
// So test result is dependant of the machine (performance, load...), and test consums time.
//
// Instead, prefer this method to test that an event come in a certain time :
//
// In this example, an event comes 1 second after test start
//
// // Mock time
// auto timeProviderMock = useTimeMock();
// shared::event::CEventHandler evtHandler;
//
// ... Create here your event and start the 1 second operation ...
//
// // Move forward time of 998 ms later
// timeProviderMock->sleep(boost::posix_time::milliseconds(998));
// // Check that no event arrives
// BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), shared::event::kNoEvent);
//
// // Move forward time of 2 ms later
// timeProviderMock->sleep(boost::posix_time::milliseconds(2));
// // Check that event was fired
// BOOST_REQUIRE_EQUAL(evtHandler.waitForEvents(boost::date_time::min_date_time), evtId);
//
//--------------------------------------------------------------


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