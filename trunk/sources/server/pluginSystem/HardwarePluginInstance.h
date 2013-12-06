//
// HardwarePluginInstance.h
//
// yadoms-plugin instance handler
//
#pragma once

#include "tools/ThreadBase.h"
#include "HardwarePluginFactory.h"
#include "database/entities/Hardware.h"

//--------------------------------------------------------------
/// \brief	this class is used to manage a plugin instance. 
//--------------------------------------------------------------
class CHardwarePluginInstance : public CThreadBase
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param [in]	plugin         the plugin used for this instance
   /// \param [in]   context        the database accessor
   //--------------------------------------------------------------
   CHardwarePluginInstance(boost::shared_ptr<const CHardwarePluginFactory> plugin, const CHardware& context);

   //--------------------------------------------------------------
   /// \brief	Desstructor
   //--------------------------------------------------------------
   virtual ~CHardwarePluginInstance();

   //--------------------------------------------------------------
   /// \brief			Notify the plugin about its configuration changed
   //--------------------------------------------------------------
   virtual void updateConfiguration() const;

protected:
   //--------------------------------------------------------------
   /// \brief			The main plugin work
   //--------------------------------------------------------------
   virtual void doWork();

private:
   //--------------------------------------------------------------
   /// \brief			The plugin used for this instance
   //--------------------------------------------------------------
   boost::shared_ptr<const CHardwarePluginFactory> m_pPlugin;

   //--------------------------------------------------------------
   /// \brief			The plugin instance
   //--------------------------------------------------------------
   boost::shared_ptr<IHardwarePlugin> m_pPluginInstance;//TODO : rendre thread-safe

   //--------------------------------------------------------------
   /// \brief			The database accessor
   //--------------------------------------------------------------
   const CHardware m_context;
};
