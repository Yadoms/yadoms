#include "stdafx.h"
#include "RareDeviceIdFilter.h"
#include <shared/currentTime/Provider.h>


CRareDeviceIdFilter::CRareDeviceIdFilter(const int minTimesPerPeriod,
                                         const boost::posix_time::time_duration& period)
   : m_minTimesPerPeriod(minTimesPerPeriod),
     m_period(period)
{
}

CRareDeviceIdFilter::~CRareDeviceIdFilter()
{
}

bool CRareDeviceIdFilter::isValid(const std::string& deviceName)
{
   // Add the new device
   m_recentlySeenDevices.push_back(std::pair<boost::posix_time::ptime, std::string>(shared::currentTime::Provider().now(), deviceName));

   // Remove too old elements
   auto tooOldThreshold = shared::currentTime::Provider().now() - m_period;
   {
      m_recentlySeenDevices.erase(std::remove_if(m_recentlySeenDevices.begin(),
                                                 m_recentlySeenDevices.end(),
                                                 [&](std::pair<boost::posix_time::ptime, std::string> value)
                                                 {
                                                    return value.first < tooOldThreshold;
                                                 }),
                                  m_recentlySeenDevices.end());
   }

   // Count occurrences of this device
   auto nbOccurrences = 0;
   auto isValid = false;
   std::for_each(m_recentlySeenDevices.begin(),
                 m_recentlySeenDevices.end(),
                 [&nbOccurrences, &isValid, &deviceName, this](std::pair<boost::posix_time::ptime, std::string> value)
                 {
                    if (value.second == deviceName)
                    {
                       ++nbOccurrences;
                       if (nbOccurrences >= m_minTimesPerPeriod)
                          isValid = true;
                    }
                 });

   // If found, remove elements corresponding to deviceName before return
   if (isValid)
   {
      m_recentlySeenDevices.erase(std::remove_if(m_recentlySeenDevices.begin(),
                                                 m_recentlySeenDevices.end(),
                                                 [&](std::pair<boost::posix_time::ptime, std::string> value)
                                                 {
                                                    return value.second == deviceName;
                                                 }),
                                  m_recentlySeenDevices.end());
   }

   return isValid;
}

