#include "stdafx.h"
#include "IndicatorQualifier.h"

namespace pluginSystem
{
   // 7 ==> Tolerance of 1 crash per week
   const int CIndicatorQualifier::m_SafetyThreshold = 7;


   CIndicatorQualifier::CIndicatorQualifier(boost::shared_ptr<database::IPluginEventLoggerRequester> pluginLogger,
                                            boost::shared_ptr<dataAccessLayer::IEventLogger> mainLogger)
      : m_basicQualifier(pluginLogger, mainLogger),
        m_pluginLogger(pluginLogger)
   {
   }

   CIndicatorQualifier::~CIndicatorQualifier()
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
      int quality = getQualityLevel(pluginInformation);
      return quality == kNoEnoughData || quality >= m_SafetyThreshold;
   }

   int CIndicatorQualifier::getQualityLevel(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   {
      CIdentityForQualifier identity(pluginInformation);

      boost::lock_guard<boost::mutex> lock(m_qualityIndicatorsCacheMutex);

      // Return value in cache if found
      QualityIndicatorsCache::const_iterator cacheIterator = m_qualityIndicatorsCache.find(identity);
      if (cacheIterator != m_qualityIndicatorsCache.end())
         return cacheIterator->second;

      // If not found in cache, compute it
      int qualityLevel = computeQuality(identity);
      // And store it in cache
      m_qualityIndicatorsCache[identity] = qualityLevel;

      return qualityLevel;
   }

   void CIndicatorQualifier::obsoleteQualityIndicatorCache(const CIdentityForQualifier& identity)
   {
      boost::lock_guard<boost::mutex> lock(m_qualityIndicatorsCacheMutex);

      QualityIndicatorsCache::iterator cacheIterator = m_qualityIndicatorsCache.find(identity);
      if (cacheIterator == m_qualityIndicatorsCache.end())
         return; // Not found in cache, nothing to do

      m_qualityIndicatorsCache.erase(cacheIterator);
   }

   int CIndicatorQualifier::computeQuality(const CIdentityForQualifier& identity) const
   {
      // First, compute total running time and crashes number, on a 90-days period
      auto crashsNb = 0;
      boost::posix_time::time_duration runningDuration;
      boost::posix_time::ptime lastLoadTime(boost::posix_time::not_a_date_time);

      auto fromDate = boost::gregorian::day_clock::universal_day() - boost::gregorian::days(90);
      auto pluginEvents = m_pluginLogger->getPluginEvents(identity.getType(), identity.getVersion().toString(), boost::posix_time::ptime(fromDate));
      for (auto it = pluginEvents.begin(); it != pluginEvents.end(); ++it)
      {
         switch ((*it)->EventType())
         {
         case database::entities::EEventType::kLoadValue:
            {
               lastLoadTime = (*it)->EventDate();
               break;
            }
         case database::entities::EEventType::kUnloadValue:
            {
               if (lastLoadTime != boost::posix_time::not_a_date_time)
                  runningDuration += (*it)->EventDate() - lastLoadTime;

               lastLoadTime = boost::posix_time::not_a_date_time;
               break;
            }
         case database::entities::EEventType::kCrashValue:
            {
               crashsNb++;
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
      if (crashsNb >= 20 && runningDuration < boost::posix_time::hours(96))
         return 0;

      // If running time is less than 1 hour, data are not significant
      if (runningDuration < boost::posix_time::hours(1))
         return kNoEnoughData;

      // If no crash during the first 96 hours, data are considered as not significant
      if (crashsNb == 0 && runningDuration < boost::posix_time::hours(96))
         return kNoEnoughData;

      // Now, compute the indicator
      // 100 is the best quality (plugin never crashed on the full period)
      // 0 is the worst quality
      // If only one crash occurred, 99 can be obtained when running for 90 days.
      // The indicator represents the mean duration between two crashes.

      if (crashsNb == 0)
         return 100; // Nice plugin !

      auto quality = runningDuration.hours() * 100 / crashsNb / (90 * 24);

      if (quality > 99)
         return 99; // Never returns 100 if there is at least one crash
      if (quality < 0)
         return 0; // ensure int46 will not be badly casted into int

      return static_cast<int>(quality);
   }
} // namespace pluginSystem


