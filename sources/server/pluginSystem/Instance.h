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
      /// \param [in]	plugin         the plugin used for this instance
      /// \param [in]   pluginData     the database entity
      /// \param [in]   qualifier      the plugin qualifier
      /// \param [in]   supervisor     the supervisor event handler
      /// \param [in]   pluginManagerEventId    The ID to use to send events to supervisor
      //--------------------------------------------------------------
      CInstance(const boost::shared_ptr<const CFactory> plugin, const boost::shared_ptr<database::entities::CPlugin> pluginData,
         const boost::shared_ptr<IQualifier> qualifier, shared::event::CEventHandler& supervisor,
         int pluginManagerEventId, boost::asio::io_service& pluginIOService);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CInstance();

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
      /// \brief			Plugin IOService (common for all plugin instances)
      //--------------------------------------------------------------
      boost::shared_ptr<CYadomsApiImplementation> m_api;
   };

} // namespace pluginSystem
