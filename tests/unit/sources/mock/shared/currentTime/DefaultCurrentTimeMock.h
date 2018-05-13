#pragma once

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/currentTime/ICurrentTime.h"

class CDefaultCurrentTimeMock : public shared::currentTime::ICurrentTime
{
public:
   explicit CDefaultCurrentTimeMock(const std::string& refTime = "2015-10-05 08:30:00.000");

   virtual ~CDefaultCurrentTimeMock();

   // shared::currentTime::ICurrentTime implementation
   boost::posix_time::ptime now() const override;
   // [END] shared::currentTime::ICurrentTime implementation

   void reset(const std::string& refTime = "2015-10-05 08:30:00.000");

   // Move the refTime of a relative duration
   // After call : m_refTime = last_m_refTime + duration
   void sleep(const boost::posix_time::time_duration& duration);

   // Move the refTime of an absolute duration
   // After call : m_refTime = m_initialRefTime + duration
   void move(const boost::posix_time::time_duration& duration);

private:
   boost::posix_time::ptime m_initialRefTime;
   boost::posix_time::ptime m_refTime;
};

extern boost::shared_ptr<CDefaultCurrentTimeMock> useTimeMock();
