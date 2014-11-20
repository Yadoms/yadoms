//
// Instance.h
//
// yadoms-plugin instance handler
//
#pragma once

#include <shared/ThreadBase.h>
#include <shared/event/EventHandler.hpp>
#include <shared/plugin/yadomsApi/IManuallyDeviceCreationRequest.h>
#include <shared/plugin/yadomsApi/ICustomQueryListRequest.h>
#include "Factory.h"
#include "ManagerEvent.h"
#include "IQualifier.h"
#include "YadomsApiImplementation.h"
#include "database/entities/Entities.h"
#include "database/IPluginEventLoggerRequester.h"
#include "database/IDeviceRequester.h"
#include "database/IKeywordRequester.h"
#include "database/IAcquisitionRequester.h"
#include "dataAccessLayer/IAcquisitionHistorizer.h"
#include "dataAccessLayer/IDeviceManager.h"

namespace pluginSystem
{

   //--------------------------------------------------------------
   /// \brief	this class is used to manage a plugin instance. 
   //--------------------------------------------------------------
   class CInstance : public shared::CThreadBase
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param [in]	plugin                     the plugin used for this instance
      /// \param [in]   pluginData                 the database entity
      /// \param [in]   pluginEventLoggerRequester the plugin event logger requester
      /// \param [in]   deviceRequester            the device requester
      /// \param [in]   keywordRequester           the keyword requester
      /// \param [in]   acquisitionRequester       the acquisition requester
      /// \param [in]   qualifier                  the plugin qualifier
      /// \param [in]   supervisor                 the supervisor event handler
      /// \param [in]   pluginManagerEventId       The ID to use to send events to supervisor
      //--------------------------------------------------------------
      CInstance(const boost::shared_ptr<const IFactory> plugin, const boost::shared_ptr<database::entities::CPlugin> pluginData,
         boost::shared_ptr<database::IPluginEventLoggerRequester> pluginEventLoggerRequester,
         boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
         boost::shared_ptr<database::IKeywordRequester> keywordRequester,
         boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
         boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer,
         const boost::shared_ptr<IQualifier> qualifier,
         shared::event::CEventHandler& supervisor, int pluginManagerEventId);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CInstance();

      //--------------------------------------------------------------
      /// \brief			            Post a command to the plugin
      /// \param  command           Command to post
      //--------------------------------------------------------------
      virtual void postCommand(boost::shared_ptr<const shared::plugin::yadomsApi::IDeviceCommand> command) const;

      //--------------------------------------------------------------
      /// \brief			            Post a manually device creation request to the plugin
      /// \param  request           Request data
      //--------------------------------------------------------------
      virtual void postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yadomsApi::IManuallyDeviceCreationRequest> & request) const;

		//--------------------------------------------------------------
		/// \brief                 Post a custom query request to a plugin
		/// \param [in] request    Request data
		//--------------------------------------------------------------
		virtual void postCustomQueryRequest(boost::shared_ptr<shared::plugin::yadomsApi::ICustomQueryListRequest> & request) const;

      //--------------------------------------------------------------
      /// \brief			            Notify the plugin about its configuration changed
      /// \param  newConfiguration  The new configuration to apply
      //--------------------------------------------------------------
      virtual void updateConfiguration(const shared::CDataContainer & newConfiguration) const;

      //--------------------------------------------------------------
      /// \brief			Get the plugin name of this instance
      //--------------------------------------------------------------
      const std::string getPluginName() const;

   protected:
      //--------------------------------------------------------------
      /// \brief			The main plugin work
      //--------------------------------------------------------------
      virtual void doWork();

   private:
      //--------------------------------------------------------------
      /// \brief			The plugin used for this instance
      //--------------------------------------------------------------
      const boost::shared_ptr<const IFactory> m_pPlugin;

      //--------------------------------------------------------------
      /// \brief			The plugin instance
      //--------------------------------------------------------------
      boost::shared_ptr<shared::plugin::IPlugin> m_pPluginInstance;

      //--------------------------------------------------------------
      /// \brief			The plugin qualifier
      //--------------------------------------------------------------
      const boost::shared_ptr<IQualifier> m_qualifier;

      //--------------------------------------------------------------
      /// \brief			The supervisor event handler
      //--------------------------------------------------------------
      shared::event::CEventHandler& m_supervisor;

      //--------------------------------------------------------------
      /// \brief			ID to use to send events to supervisor
      //--------------------------------------------------------------
      const int m_pluginManagerEventId;

      //--------------------------------------------------------------
      /// \brief			Plugin context (Yadoms API)
      //--------------------------------------------------------------
      boost::shared_ptr<CYadomsApiImplementation> m_context;
   };

} // namespace pluginSystem
