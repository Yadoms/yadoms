//
// CHardwarePluginQualifier.h
//
// Yadoms-plugin qualifier
//
#pragma once

#include "IHardwarePluginQualifier.h"
#include "database/IHardwareEventLoggerRequester.h"
#include "HardwarePluginIdentityForQualifier.h"

//--------------------------------------------------------------
/// \brief	this class is used to qualify a plugin
//--------------------------------------------------------------
class CHardwarePluginQualifier : public IHardwarePluginQualifier
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in] eventLoggerDatabase    Event logger database
   //--------------------------------------------------------------
   CHardwarePluginQualifier(boost::shared_ptr<IHardwareEventLoggerRequester> eventLoggerDatabase);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginQualifier();

   // IHardwarePluginQualifier implementation
   virtual void signalLoad(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation);
   virtual void signalUnload(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation);
   virtual void signalCrash(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation, const std::string& reason);
   virtual bool isSafe(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation);
   virtual int getQualityLevel(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation);
   // [END] IHardwarePluginQualifier implementation

private:
   //--------------------------------------------------------------
   /// \brief	               Add an event to database
   /// \param[in]  pluginInformation     Plugin information (name, version...)
   /// \param[in]  eventType  Event type (load, unload, crash...)
   /// \param[in]  reason     Crash cause (exception...)
   //--------------------------------------------------------------
   void AddEventToDatabase(const boost::shared_ptr<const IHardwarePluginInformation> pluginInformation,
      CHardwareEventLogger::EEventType eventType, const std::string& reason = CStringExtension::EmptyString);

   //--------------------------------------------------------------
   /// \brief	               Make corresponding quality indicator obsolete in cache
   /// \param[in] identity    Plugin identity
   //--------------------------------------------------------------
   void obsoleteQualityIndicatorCache(const CPluginIdentity& identity);

   //--------------------------------------------------------------
   /// \brief	               compute quality indicator of a plugin
   /// \param[in] identity    Plugin identity
   /// \return                Computed quality, from 0(lower) to 100(best)
   //--------------------------------------------------------------
   int computeQuality(const CPluginIdentity& identity) const;

private:
   //--------------------------------------------------------------
   /// \brief	Safety threshold : a plugin with a quality upper or equal to
   ///         this threshold, is considered as safe.
   //--------------------------------------------------------------
   static const int m_SafetyThreshold;

   //--------------------------------------------------------------
   /// \brief	Cache of quality indicators
   ///         Key is plugin identity
   ///         Value is quality indicator
   ///         Need specific comparator
   //--------------------------------------------------------------
   typedef std::map<CPluginIdentity, int, CPluginIdentityCompare> QualityIndicatorsCache;
   QualityIndicatorsCache m_qualityIndicatorsCache;

   //--------------------------------------------------------------
   /// \brief	Quality indicators Cache mutex
   //--------------------------------------------------------------
   boost::mutex m_qualityIndicatorsCacheMutex;

   //--------------------------------------------------------------
   /// \brief	database access
   //--------------------------------------------------------------
   boost::shared_ptr<IHardwareEventLoggerRequester> m_eventLoggerDatabase;
};
