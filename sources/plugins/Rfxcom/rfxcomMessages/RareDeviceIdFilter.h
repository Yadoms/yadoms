#pragma once
#include "../IUnsecuredProtocolFilter.h"

//--------------------------------------------------------------
/// \brief	The unsecured protocols filter
//--------------------------------------------------------------
class CRareDeviceIdFilter : public IUnsecuredProtocolFilter
{
public:
   CRareDeviceIdFilter(const int minTimesPerPeriod,
                       const boost::posix_time::time_duration& period);
   virtual ~CRareDeviceIdFilter();

   // IUnsecuredProtocolFilter Implementation
   bool isValid(const std::string& deviceName) override;
   // [END] IUnsecuredProtocolFilter Implementation

private:
   std::vector<std::pair<boost::posix_time::ptime, std::string>> m_recentlySeenDevices;
   const int m_minTimesPerPeriod;
   const boost::posix_time::time_duration m_period;
};

