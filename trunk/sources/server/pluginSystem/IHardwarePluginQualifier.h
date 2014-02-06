//
// IHardwarePluginQualifier.h
//
// yadoms-plugin qualifier interface
//
#pragma once

#include <shared/HardwarePlugin/Information/Information.h>
#include "HardwarePluginFactory.h"

//--------------------------------------------------------------
/// \brief	this class is used to qualify a plugin
//--------------------------------------------------------------
class IHardwarePluginQualifier
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IHardwarePluginQualifier() {}

   //--------------------------------------------------------------
   /// \brief			         Signal that plugin was loaded
   /// \param[in]  pluginName       Plugin name
   /// \param[in]  pluginVersion    Plugin version
   /// \param[in]  pluginReleaseType Plugin release type (beta, release...)
   //--------------------------------------------------------------
   virtual void signalLoad(const boost::shared_ptr<const CHardwarePluginFactory> plugin) = 0;

   //--------------------------------------------------------------
   /// \brief			         Signal that plugin was unloaded
   /// \param[in]  pluginName       Plugin name
   /// \param[in]  pluginVersion    Plugin version
   /// \param[in]  pluginReleaseType Plugin release type (beta, release...)
   //--------------------------------------------------------------
   virtual void signalUnload(const boost::shared_ptr<const CHardwarePluginFactory> plugin) = 0;

   //--------------------------------------------------------------
   /// \brief			         Signal that plugin has crashed
   /// \param[in]  plugin     Plugin
   /// \param[in]  reason     Crash cause (exception...)
   //--------------------------------------------------------------
   virtual void signalCrash(const boost::shared_ptr<const CHardwarePluginFactory> plugin, const std::string& exceptionName) = 0;

   //--------------------------------------------------------------
   /// \brief			         Check if plugin is safe
   /// \param[in]  plugin     Plugin
   /// \return                true if safe (don't crash too much)
   //--------------------------------------------------------------
   virtual bool isSafe(const boost::shared_ptr<const CHardwarePluginFactory> plugin) = 0;//TODO pas encore appelé

   //--------------------------------------------------------------
   /// \brief			         Get The quality level
   /// \param[in]  plugin     Plugin
   /// \return                Plugin quality level, value from 0 (very bad) to 100 (perfect, never crashed)
   //--------------------------------------------------------------
   virtual int getQualityLevel(const boost::shared_ptr<const CHardwarePluginFactory> plugin) = 0;//TODO pas encore appelé
};
