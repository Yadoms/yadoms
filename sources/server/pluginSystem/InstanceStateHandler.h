#pragma once
#include <shared/plugin/yPluginApi/historization/PluginStateMessage.h>
#include <shared/process/IProcessObserver.h>
#include "IInstanceStartErrorObserver.h"
#include "IInstanceStateHandler.h"
#include "database/IPluginRequester.h"
#include "database/IPluginEventLoggerRequester.h"
#include "dataAccessLayer/IDeviceManager.h"
#include "dataAccessLayer/IAcquisitionHistorizer.h"
#include "dataAccessLayer/IKeywordManager.h"
#include "IQualifier.h"

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The instance error handler
   //-----------------------------------------------------
   class CInstanceStateHandler final : public IInstanceStartErrorObserver, public IInstanceStateHandler, public shared::process::IProcessObserver
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] instanceData             Plugin instance data
      ///\param[in] pluginInformation        Plugin information
      ///\param[in] pluginRequester          Database requester
      ///\param[in] qualifier                Event qualifier
      ///\param[in] pluginEventLoggerRequester Plugin event logger
      ///\param[in] acquisitionHistorizer    Acquisition recorder
      ///\param[in] onPluginsStoppedFct      The listener to call when an instance is stopped
      ///\param[in] deviceManager            Database device access layer
      ///\param[in] keywordManager           Database keyword access layer
      //-----------------------------------------------------
      CInstanceStateHandler(boost::shared_ptr<const database::entities::CPlugin> instanceData,
                            boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                            boost::shared_ptr<database::IPluginRequester> pluginRequester,
                            boost::shared_ptr<IQualifier> qualifier,
                            boost::shared_ptr<database::IPluginEventLoggerRequester> pluginEventLoggerRequester,
                            boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer,
                            boost::function1<void, int> onPluginsStoppedFct,
                            boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
                            boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordManager);

      CInstanceStateHandler(const CInstanceStateHandler&) = delete;
      CInstanceStateHandler(const CInstanceStateHandler&&) = delete;
      CInstanceStateHandler operator=(const CInstanceStateHandler&) = delete;
      CInstanceStateHandler operator=(const CInstanceStateHandler&&) = delete;

      ~CInstanceStateHandler() override = default;


      // IInstanceStartErrorObserver Implementation
      void signalStartError(const std::string& error) override;
      // [END] IInstanceStartErrorObserver Implementation

      // process::IProcessObserver Implementation
      void onStart() override;
      void onFinish(int returnCode, const std::string& error) override;
      // [END] process::IProcessObserver Implementation


   protected:
      // IInstanceStateHandler Implementation
      void setState(const shared::plugin::yPluginApi::historization::EPluginState& state,
                    const std::string& customMessageId = std::string(),
                    const std::string& customMessageData = std::string()) override;
      // [END] IInstanceStateHandler Implementation


      void signalNormalStop();
      void signalError(int returnCode,
                       const std::string& error);

      // Get the plugin state device/keywords (create if not exists)
      int pluginStateDeviceId() const;
      int pluginStateKeywordId();
      int pluginStateMessageIdKeywordId();

      enum PluginEventSeverity
      {
         kInfo,
         kError
      };

      //-----------------------------------------------------
      ///\brief Record a plugin major event (recorded in Yadoms database)
      ///\param    [in]    severity           The message severity
      ///\param    [in]    message            The message
      //-----------------------------------------------------
      void recordPluginEvent(PluginEventSeverity severity, const std::string& message);

   private:
      //-----------------------------------------------------
      ///\brief               The instance data
      //-----------------------------------------------------
      boost::shared_ptr<const database::entities::CPlugin> m_instanceData;

      //-----------------------------------------------------
      ///\brief               The plugin information
      //-----------------------------------------------------
      boost::shared_ptr<const shared::plugin::information::IInformation> m_pluginInformation;

      //-----------------------------------------------------
      ///\brief               The plugin data accessor
      //-----------------------------------------------------
      boost::shared_ptr<database::IPluginRequester> m_pluginRequester;

      //-----------------------------------------------------
      ///\brief               The plugin qualifier
      //-----------------------------------------------------
      boost::shared_ptr<IQualifier> m_qualifier;

      //--------------------------------------------------------------
      /// \brief			The plugin event logger requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IPluginEventLoggerRequester> m_pluginEventLoggerRequester;

      //--------------------------------------------------------------
      /// \brief			The Acquisition historizer
      //--------------------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> m_acquisitionHistorizer;

      //--------------------------------------------------------------
      /// \brief			The listener to call when an instance is stopped
      //--------------------------------------------------------------
      boost::function1<void, int> m_onPluginsStoppedFct;

      //--------------------------------------------------------------
      /// \brief			The device requester
      //--------------------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IDeviceManager> m_deviceManager;

      //--------------------------------------------------------------
      /// \brief			The keyword requester
      //--------------------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IKeywordManager> m_keywordDataAccessLayer;

      //--------------------------------------------------------------
      /// \brief	      The plugin state keyword
      /// \note         Use it by pluginState() method
      //--------------------------------------------------------------
      boost::shared_ptr<shared::plugin::yPluginApi::historization::CPluginState> m_pluginStateKeyword;
      int m_pluginStateKeywordId;
      boost::shared_ptr<shared::plugin::yPluginApi::historization::CPluginStateMessage> m_pluginStateMessage;
      int m_pluginStateMessageIdKeywordId;
   };
} // namespace pluginSystem
