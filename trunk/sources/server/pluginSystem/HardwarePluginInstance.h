//
// HardwarePluginInstance.h
//
// yadoms-plugin instance handler
//
#pragma once

#include "tools/ThreadBase.h"
#include "HardwarePluginFactory.h"

//--------------------------------------------------------------
/// \brief	this class is used to manage a plugin instance. 
//--------------------------------------------------------------
class CHardwarePluginInstance : public CThreadBase
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param [in]	pluginInstanceName   the plugin instance name
   /// \param [in]   pluginInstance       the plugin instance to manage
   //--------------------------------------------------------------
   CHardwarePluginInstance(const std::string& pluginInstanceName, boost::shared_ptr<IHardwarePlugin> pluginInstance);

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
   /// \brief			The plugin instance
   //--------------------------------------------------------------
   boost::shared_ptr<IHardwarePlugin> m_pPluginInstance;//TODO : rendre thread-safe

   //--------------------------------------------------------------
   /// \brief			get the plugin instance configuration (through database)
   //--------------------------------------------------------------
   std::string getPluginConfiguration() const;
};
