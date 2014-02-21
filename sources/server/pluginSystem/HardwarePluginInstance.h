//
// HardwarePluginInstance.h
//
// yadoms-plugin instance handler
//
#pragma once

#include <shared/ThreadBase.h>
#include <shared/event/EventHandler.hpp>
#include "HardwarePluginFactory.h"
#include "HardwarePluginManagerEvent.h"
#include "database/entities/Hardware.h"
#include "IHardwarePluginQualifier.h"

//--------------------------------------------------------------
/// \brief	this class is used to manage a plugin instance. 
//--------------------------------------------------------------
class CHardwarePluginInstance : public shared::CThreadBase
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param [in]	plugin         the plugin used for this instance
   /// \param [in]   context        the database entity
   /// \param [in]   qualifier      the plugin qualifier
   /// \param [in]   supervisor     the supervisor event handler
   /// \param [in]   pluginManagerEventId    The ID to use to send events to supervisor
   //--------------------------------------------------------------
   CHardwarePluginInstance(const boost::shared_ptr<const CHardwarePluginFactory> plugin, const boost::shared_ptr<server::database::entities::CHardware> context,
      const boost::shared_ptr<IHardwarePluginQualifier> qualifier, shared::event::CEventHandler& supervisor,
      int pluginManagerEventId, boost::asio::io_service * pluginIOService);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginInstance();

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
   const boost::shared_ptr<const CHardwarePluginFactory> m_pPlugin;

   //--------------------------------------------------------------
   /// \brief			The plugin instance
   //--------------------------------------------------------------
   boost::shared_ptr<IHardwarePlugin> m_pPluginInstance;

   //--------------------------------------------------------------
   /// \brief			The database accessor
   //--------------------------------------------------------------
   const boost::shared_ptr<server::database::entities::CHardware> m_context;

   //--------------------------------------------------------------
   /// \brief			The plugin qualifier
   //--------------------------------------------------------------
   const boost::shared_ptr<IHardwarePluginQualifier> m_qualifier;

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
   boost::asio::io_service * m_pluginIOService;
};
