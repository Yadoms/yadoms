//
// Instance.h
//
// yadoms-plugin instance handler
//
#pragma once

#include <shared/ThreadBase.h>
#include <shared/event/EventHandler.hpp>
#include "Factory.h"
#include "ManagerEvent.h"
#include "IQualifier.h"
#include "YadomsApiImplementation.h"
#include "database/entities/Entities.h"
#include "database/IDeviceRequester.h"
#include "database/IKeywordRequester.h"
#include "database/IAcquisitionRequester.h"
#include "communication/command/DeviceCommand.h"

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
      /// \param [in]	plugin                  the plugin used for this instance
      /// \param [in]   pluginData              the database entity
      /// \param [in]   deviceRequester         the device requester
      /// \param [in]   keywordRequester        the keyword requester
      /// \param [in]   acquisitionRequester    the acquisition requester
      /// \param [in]   qualifier               the plugin qualifier
      /// \param [in]   supervisor              the supervisor event handler
      /// \param [in]   pluginManagerEventId    The ID to use to send events to supervisor
      //--------------------------------------------------------------
      CInstance(const boost::shared_ptr<const CFactory> plugin, const boost::shared_ptr<database::entities::CPlugin> pluginData,
         boost::shared_ptr<database::IDeviceRequester> deviceRequester, boost::shared_ptr<database::IKeywordRequester> keywordRequester,
         boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester, const boost::shared_ptr<IQualifier> qualifier,
         shared::event::CEventHandler& supervisor, int pluginManagerEventId);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CInstance();

      //--------------------------------------------------------------
      /// \brief			            Post a command to the plugin
      /// \param  message           Command to post
      //--------------------------------------------------------------
      virtual void postCommand(const communication::command::CDeviceCommand & message) const;

      //--------------------------------------------------------------
      /// \brief			            Notify the plugin about its configuration changed
      /// \param  newConfiguration  The new configuration to apply
      //--------------------------------------------------------------
      virtual void updateConfiguration(const std::string& newConfiguration) const;

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
      const boost::shared_ptr<const CFactory> m_pPlugin;

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
