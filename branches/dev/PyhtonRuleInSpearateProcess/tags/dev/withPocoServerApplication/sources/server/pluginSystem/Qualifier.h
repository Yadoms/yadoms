//
// Qualifier.h
//
// Yadoms-plugin qualifier
//
#pragma once

#include "IQualifier.h"
#include "database/IPluginEventLoggerRequester.h"
#include "database/IEventLoggerRequester.h"
#include "IdentityForQualifier.h"

namespace pluginSystem
{

   //--------------------------------------------------------------
   /// \brief	this class is used to quality a plugin
   //--------------------------------------------------------------
   class CQualifier : public IQualifier
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in] pluginLogger   Main logger database requester
      /// \param[in] mainLogger     Plugin logger database requester
      //--------------------------------------------------------------
      CQualifier(boost::shared_ptr<database::IPluginEventLoggerRequester> pluginLogger,
         boost::shared_ptr<database::IEventLoggerRequester> mainLogger);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CQualifier();

      // IQualifier implementation
      virtual void signalLoad(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation);
      virtual void signalUnload(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation);
      virtual void signalCrash(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation, const std::string& reason);
      virtual bool isSafe(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation);
      virtual int getQualityLevel(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation);
      // [END] IQualifier implementation

   private:
      //--------------------------------------------------------------
      /// \brief	               Add an event to database
      /// \param[in]  pluginInformation     Plugin information (name, version...)
      /// \param[in]  eventType  Event type (load, unload, crash...)
      /// \param[in]  reason     Crash cause (exception...)
      //--------------------------------------------------------------
      void AddEventToDatabase(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
         database::entities::EEventType eventType, const std::string& reason = shared::CStringExtension::EmptyString);

      //--------------------------------------------------------------
      /// \brief	               Make corresponding quality indicator obsolete in cache
      /// \param[in] identity    Plugin identity
      //--------------------------------------------------------------
      void obsoleteQualityIndicatorCache(const CIdentityForQualifier& identity);

      //--------------------------------------------------------------
      /// \brief	               compute quality indicator of a plugin
      /// \param[in] identity    Plugin identity
      /// \return                Computed quality, from 0(lower) to 100(best)
      //--------------------------------------------------------------
      int computeQuality(const CIdentityForQualifier& identity) const;

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
      typedef std::map<CIdentityForQualifier, int, CPluginIdentityCompare> QualityIndicatorsCache;
      QualityIndicatorsCache m_qualityIndicatorsCache;

      //--------------------------------------------------------------
      /// \brief	Quality indicators Cache mutex
      //--------------------------------------------------------------
      boost::mutex m_qualityIndicatorsCacheMutex;

      //--------------------------------------------------------------
      /// \brief	Plugin logger access
      //--------------------------------------------------------------
      boost::shared_ptr<database::IPluginEventLoggerRequester> m_pluginLogger;

      //--------------------------------------------------------------
      /// \brief	Main logger access
      //--------------------------------------------------------------
      boost::shared_ptr<database::IEventLoggerRequester> m_mainLogger;
   };

} // namespace pluginSystem
