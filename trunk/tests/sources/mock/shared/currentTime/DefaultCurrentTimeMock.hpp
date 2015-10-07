

// Includes needed to compile tested classes
#include "../../../../sources/shared/shared/currentTime/ICurrentTime.h"

class CDefaultCurrentTimeMock : public shared::currentTime::ICurrentTime
{
public:
   CDefaultCurrentTimeMock(const std::string& refTime = "2015-10-05 08:30:00.000") : m_initialRefTime(boost::posix_time::time_from_string(refTime)), m_refTime(m_initialRefTime) {}
   virtual ~CDefaultCurrentTimeMock() {}
   // shared::currentTime::ICurrentTime implementation
   virtual boost::posix_time::ptime now() const { return m_refTime; }
   // [END] shared::currentTime::ICurrentTime implementation

   // Move the refTime of a relative duration
   // After call : m_refTime = last_m_refTime + duration
   void sleep(const boost::posix_time::time_duration& duration) { m_refTime += duration; }

   // Move the refTime of an absolute duration
   // After call : m_refTime = m_initialRefTime + duration
   void move(const boost::posix_time::time_duration& duration) { m_refTime = m_initialRefTime + duration; }

private:
   const boost::posix_time::ptime m_initialRefTime;
   boost::posix_time::ptime m_refTime;
};