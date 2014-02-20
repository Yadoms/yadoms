//
// IHardwarePluginQualifier.h
//
// yadoms-plugin qualifier interface
//
#pragma once

#include <shared/plugin/Information/Information.h>
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
   /// \param[in]  pluginInformation     Plugin information (name, version...)
   //--------------------------------------------------------------
   virtual void signalLoad(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation) = 0;

   //--------------------------------------------------------------
   /// \brief			         Signal that plugin was unloaded
   /// \param[in]  pluginInformation     Plugin information (name, version...)
   //--------------------------------------------------------------
   virtual void signalUnload(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation) = 0;

   //--------------------------------------------------------------
   /// \brief			         Signal that plugin has crashed
   /// \param[in]  pluginInformation     Plugin information (name, version...)
   /// \param[in]  reason     Crash cause (exception...)
   //--------------------------------------------------------------
   virtual void signalCrash(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation, const std::string& reason) = 0;

   //--------------------------------------------------------------
   /// \brief			         Check if plugin is safe
   /// \param[in]  pluginInformation     Plugin information (name, version...)
   /// \return                true if safe (don't crash too much)
   //--------------------------------------------------------------
   virtual bool isSafe(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation) = 0;

   //--------------------------------------------------------------
   /// \brief			         Get The quality level
   /// \param[in]  pluginInformation     Plugin information (name, version...)
   /// \return                Plugin quality level, value from 0 (very bad) to 100 (perfect, never crashed).
   //                         Can be kNoEnoughData if evaluation is not significative (plugin doesn't run for enough time)
   //--------------------------------------------------------------
   enum { kNoEnoughData = -1 };
   virtual int getQualityLevel(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation) = 0;
};
