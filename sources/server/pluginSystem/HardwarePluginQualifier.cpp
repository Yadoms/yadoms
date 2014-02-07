#include "stdafx.h"
#include "HardwarePluginQualifier.h"


const int CHardwarePluginQualifier::m_SafetyThreshold = 80;


CHardwarePluginQualifier::CHardwarePluginQualifier()
{
}

CHardwarePluginQualifier::~CHardwarePluginQualifier()
{
}

void CHardwarePluginQualifier::signalLoad(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation)
{
   // Insert event in database
   //TODO

   // Since a new event is recorded, cache of quality indicator is obsolete
   obsoleteQualityIndicatorCache(CPluginIdentity(pluginInformation));
}

void CHardwarePluginQualifier::signalUnload(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation)
{
   // Insert event in database
   //TODO

   // Since a new event is recorded, cache of quality indicator is obsolete
   obsoleteQualityIndicatorCache(CPluginIdentity(pluginInformation));
}

void CHardwarePluginQualifier::signalCrash(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation, const std::string& exceptionName)
{
   // Insert event in database
   //TODO

   // Since a new event is recorded, cache of quality indicator is obsolete
   obsoleteQualityIndicatorCache(CPluginIdentity(pluginInformation));
}

bool CHardwarePluginQualifier::isSafe(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation)
{
   return getQualityLevel(pluginInformation) >= m_SafetyThreshold;
}

int CHardwarePluginQualifier::getQualityLevel(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation)
{
   CPluginIdentity identity(pluginInformation);

   // Return value in cache if found
   QualityIndicatorsCache::const_iterator cacheIterator = m_qualityIndicatorsCache.find(identity);
   if (cacheIterator != m_qualityIndicatorsCache.end())
      return cacheIterator->second;

   // If not found in cache, compute it
   m_qualityIndicatorsCache[identity] = computeQuality(identity);

   return 100;
}

void CHardwarePluginQualifier::obsoleteQualityIndicatorCache(const CPluginIdentity& identity)
{
   QualityIndicatorsCache::const_iterator cacheIterator = m_qualityIndicatorsCache.find(identity);
   if (cacheIterator == m_qualityIndicatorsCache.end())
      return;

   //TODO
}

int CHardwarePluginQualifier::computeQuality(const CPluginIdentity& identity) const
{
   //TODO
   return 85;
}