#pragma once

#include "IHardwarePluginConfigurationProvider.h"


//--------------------------------------------------------------
/// \class General hardware plugin interface
//--------------------------------------------------------------
class IHardwarePlugin
{
public:
   virtual ~IHardwarePlugin() {}

   //--------------------------------------------------------------
   /// \brief        Main plugin function
   /// \par config   Configuration provider
   /// \note         Do the work. This function runs in specific-thread context (yadoms main-thread independent).
   //                Implementation should loop until end is asked by yadoms.
   //                Be careful to put some sleeps to prevent using too much system ressources.
   //                Use boost::this_thread::sleep is a watch stop point (a point where thread is able to stop).
   //--------------------------------------------------------------
   virtual void doWork(const IHardwarePluginConfigurationProvider& config) = 0;
};

