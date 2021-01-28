//
// Qualifier.h
//
// Yadoms-plugin qualifier
//
#pragma once

#include "BasicQualifier.h"
#include "database/IPluginEventLoggerRequester.h"
#include "../dataAccessLayer/IEventLogger.h"
#include "IdentityForQualifier.h"

namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	This qualifier compute plugin quality
   //--------------------------------------------------------------
   class CIndicatorQualifier : public IQualifier
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in] pluginLogger   Plugin logger database requester
      /// \param[in] mainLogger     Main logger
      //--------------------------------------------------------------
      CIndicatorQualifier(boost::shared_ptr<database::IPluginEventLoggerRequester> pluginLogger,
                          boost::shared_ptr<dataAccessLayer::IEventLogger> mainLogger);

      virtual ~CIndicatorQualifier() = default;

      // IQualifier implementation
      void signalLoad(boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation) override;
      void signalUnload(boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation) override;
      void signalCrash(boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation, const std::string& reason) override;
      bool isSafe(boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation) override;
      int getQualityLevel(boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation) override;
      // [END] IQualifier implementation

   private:
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

      //--------------------------------------------------------------
      /// \brief	Base qualifier
      //--------------------------------------------------------------
      CBasicQualifier m_basicQualifier;

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
   };
} // namespace pluginSystem
