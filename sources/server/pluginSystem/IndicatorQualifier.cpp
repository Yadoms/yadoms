#include "stdafx.h"
#include "IndicatorQualifier.h"

#include <utility>

namespace pluginSystem
{
   // 7 ==> Tolerance of 1 crash per week
   const int CIndicatorQualifier::m_SafetyThreshold = 7;


   CIndicatorQualifier::CIndicatorQualifier(boost::shared_ptr<database::IPluginEventLoggerRequester> pluginLogger,
                                            boost::shared_ptr<dataAccessLayer::IEventLogger> mainLogger)
      : m_basicQualifier(pluginLogger, std::move(mainLogger)),
        m_pluginLogger(pluginLogger)
   {
   }

   void CIndicatorQualifier::signalLoad(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   {
      m_basicQualifier.signalLoad(pluginInformation);

      // Since a new event is recorded, cache of quality indicator is obsolete
      obsoleteQualityIndicatorCache(CIdentityForQualifier(pluginInformation));
   }

   void CIndicatorQualifier::signalUnload(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   {
      m_basicQualifier.signalUnload(pluginInformation);

      // Since a new event is recorded, cache of quality indicator is obsolete
      obsoleteQualityIndicatorCache(CIdentityForQualifier(pluginInformation));
   }

   void CIndicatorQualifier::signalCrash(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                         const std::string& reason)
   {
      m_basicQualifier.signalCrash(pluginInformation, reason);

      // Since a new event is recorded, cache of quality indicator is obsolete
      obsoleteQualityIndicatorCache(CIdentityForQualifier(pluginInformation));
   }

   bool CIndicatorQualifier::isSafe(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   {
      const auto quality = getQualityLevel(pluginInformation);
      return quality == kNoEnoughData || quality >= m_SafetyThreshold;
   }

   int CIndicatorQualifier::getQualityLevel(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   {
      const CIdentityForQualifier identity(pluginInformation);

      boost::lock_guard<boost::mutex> lock(m_qualityIndicatorsCacheMutex);

      // Return value in cache if found
      const QualityIndicatorsCache::const_iterator cacheIterator = m_qualityIndicatorsCache.find(identity);
      if (cacheIterator != m_qualityIndicatorsCache.end())
         return cacheIterator->second;

      // If not found in cache, compute it
      const auto qualityLevel = computeQuality(identity);
      // And store it in cache
      m_qualityIndicatorsCache[identity] = qualityLevel;

      return qualityLevel;
   }

   void CIndicatorQualifier::obsoleteQualityIndicatorCache(const CIdentityForQualifier& identity)
   {
      boost::lock_guard<boost::mutex> lock(m_qualityIndicatorsCacheMutex);

      const auto cacheIterator = m_qualityIndicatorsCache.find(identity);
      if (cacheIterator == m_qualityIndicatorsCache.end())
         return; // Not found in cache, nothing to do

      m_qualityIndicatorsCache.erase(cacheIterator);
   }

   int CIndicatorQualifier::computeQuality(const CIdentityForQualifier& identity) const
   {
      // First, compute total running time and crashes number, on a 90-days period
      auto crashNb = 0;
      boost::posix_time::time_duration runningDuration;
      boost::posix_time::ptime lastLoadTime(boost::posix_time::not_a_date_time);

      const auto fromDate = boost::gregorian::day_clock::universal_day() - boost::gregorian::days(90);
      auto pluginEvents = m_pluginLogger->getPluginEvents(identity.getType(), 
         boost::posix_time::ptime(fromDate));
      for (auto& pluginEvent : pluginEvents)
      {
         switch (pluginEvent->EventType())
         {
         case database::entities::EEventType::kLoadValue:
            {
               lastLoadTime = pluginEvent->EventDate();
               break;
            }
         case database::entities::EEventType::kUnloadValue:
            {
               if (lastLoadTime != boost::posix_time::not_a_date_time)
                  runningDuration += pluginEvent->EventDate() - lastLoadTime;

               lastLoadTime = boost::posix_time::not_a_date_time;
               break;
            }
         case database::entities::EEventType::kCrashValue:
            {
               crashNb++;
               break;
            }
         default:
            {
               // Ignore other values
               break;
            }
         }
      }

      // If too many crashes while the first 96 hours, return worst quality
      if (crashNb >= 20 && runningDuration < boost::posix_time::hours(96))
         return 0;

      // If running time is less than 1 hour, data are not significant
      if (runningDuration < boost::posix_time::hours(1))
         return kNoEnoughData;

      // If no crash during the first 96 hours, data are considered as not significant
      if (crashNb == 0 && runningDuration < boost::posix_time::hours(96))
         return kNoEnoughData;

      // Now, compute the indicator
      // 100 is the best quality (plugin never crashed on the full period)
      // 0 is the worst quality
      // If only one crash occurred, 99 can be obtained when running for 90 days.
      // The indicator represents the mean duration between two crashes.

      if (crashNb == 0)
         return 100; // Nice plugin !

      const auto quality = runningDuration.hours() * 100 / crashNb / (90 * 24);

      if (quality > 99)
         return 99; // Never returns 100 if there is at least one crash

      return static_cast<int>(quality);
   }
} // namespace pluginSystem


