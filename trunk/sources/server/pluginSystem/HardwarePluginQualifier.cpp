#include "stdafx.h"
#include "HardwarePluginQualifier.h"
#include <shared/Exceptions/EmptyResultException.hpp>


// 7 ==> Tolerance of 1 crash per week
const int CHardwarePluginQualifier::m_SafetyThreshold = 7;


CHardwarePluginQualifier::CHardwarePluginQualifier(boost::shared_ptr<IHardwareEventLoggerRequester> eventLoggerDatabase)
   :m_eventLoggerDatabase(eventLoggerDatabase)
{
}

CHardwarePluginQualifier::~CHardwarePluginQualifier()
{
}

void CHardwarePluginQualifier::signalLoad(const boost::shared_ptr<const shared::plugin::IInformation> pluginInformation)
{
   // Insert event in database
   AddEventToDatabase(pluginInformation, CHardwareEventLogger::kLoad);

   // Since a new event is recorded, cache of quality indicator is obsolete
   obsoleteQualityIndicatorCache(CPluginIdentity(pluginInformation));
}

void CHardwarePluginQualifier::signalUnload(const boost::shared_ptr<const shared::plugin::IInformation> pluginInformation)
{
   // Insert event in database
   AddEventToDatabase(pluginInformation, CHardwareEventLogger::kUnload);

   // Since a new event is recorded, cache of quality indicator is obsolete
   obsoleteQualityIndicatorCache(CPluginIdentity(pluginInformation));
}

void CHardwarePluginQualifier::signalCrash(const boost::shared_ptr<const shared::plugin::IInformation> pluginInformation, const std::string& reason)
{
   // Insert event in database
   AddEventToDatabase(pluginInformation, CHardwareEventLogger::kCrash, reason);

   // Since a new event is recorded, cache of quality indicator is obsolete
   obsoleteQualityIndicatorCache(CPluginIdentity(pluginInformation));
}

void CHardwarePluginQualifier::AddEventToDatabase(const boost::shared_ptr<const shared::plugin::IInformation> pluginInformation, CHardwareEventLogger::EEventType eventType, const std::string& reason)
{
   try
   {
      m_eventLoggerDatabase->addEvent(
         pluginInformation->getName(),
         pluginInformation->getVersion(),
         pluginInformation->getReleaseType(),
         eventType,
         reason);
   }
   catch (CEmptyResultException& e)
   {
      // Just log the error
      YADOMS_LOG(error) << "Error when adding plugin " << eventType << " event in database : " << e.what();
   }
}

bool CHardwarePluginQualifier::isSafe(const boost::shared_ptr<const shared::plugin::IInformation> pluginInformation)
{
   int quality = getQualityLevel(pluginInformation);
   return quality == kNoEnoughData || quality >= m_SafetyThreshold;
}

int CHardwarePluginQualifier::getQualityLevel(const boost::shared_ptr<const shared::plugin::IInformation> pluginInformation)
{
   CPluginIdentity identity(pluginInformation);

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

void CHardwarePluginQualifier::obsoleteQualityIndicatorCache(const CPluginIdentity& identity)
{
   boost::lock_guard<boost::mutex> lock(m_qualityIndicatorsCacheMutex);

   QualityIndicatorsCache::iterator cacheIterator = m_qualityIndicatorsCache.find(identity);
   if (cacheIterator == m_qualityIndicatorsCache.end())
      return;  // Not found in cache, nothing to do

   m_qualityIndicatorsCache.erase(cacheIterator);
}

int CHardwarePluginQualifier::computeQuality(const CPluginIdentity& identity) const
{
   // First, compute total running time and crashes number, on a 90-days period
   int crashsNb = 0;
   boost::posix_time::time_duration runningDuration;
   boost::posix_time::ptime lastLoadTime(boost::posix_time::not_a_date_time);

   boost::gregorian::date fromDate = boost::gregorian::day_clock::universal_day() - boost::gregorian::days(90);
   std::vector<boost::shared_ptr<CHardwareEventLogger> > pluginEvents = m_eventLoggerDatabase->getHardwareEvents(identity.getName(), identity.getVersion(), identity.getReleaseType(), boost::posix_time::ptime(fromDate));
   for (std::vector<boost::shared_ptr<CHardwareEventLogger> >::const_iterator it = pluginEvents.begin() ; it != pluginEvents.end() ; it++)
   {
      switch((*it)->getEventType())
      {
      case CHardwareEventLogger::kLoad:
         {
            lastLoadTime = (*it)->getEventDate();
            break;
         }
      case CHardwareEventLogger::kUnload:
         {
            if(lastLoadTime != boost::posix_time::not_a_date_time)
               runningDuration += (*it)->getEventDate() - lastLoadTime;

            lastLoadTime = boost::posix_time::not_a_date_time;
            break;
         }
      case CHardwareEventLogger::kCrash:
         {
            crashsNb++;
            break;
         }
      default:
         {
            YADOMS_LOG(error) << "Compute plugin quality : unknown event type " << (*it)->getEventType();
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
      return 100;       // Nice plugin !

   int quality = runningDuration.hours() * 100 / crashsNb / (90*24);

   if (quality > 99)
      return 99;        // Never returns 100 if there is at least one crash

   return quality;
}