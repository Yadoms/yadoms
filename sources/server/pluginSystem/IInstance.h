#pragma once

#include <shared/plugin/yPluginApi/IManuallyDeviceCreationRequest.h>
#include "IQualifier.h"

namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief A plugin instance
   //-----------------------------------------------------
   class IInstance
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IInstance() {}

      //--------------------------------------------------------------
      /// \brief			            Notify the plugin about its configuration changed
      /// \param  newConfiguration  The new configuration to apply
      //--------------------------------------------------------------
      virtual void updateConfiguration(const shared::CDataContainer & newConfiguration) const = 0;

      //-----------------------------------------------------
      ///\brief               Request to stop instance
      //-----------------------------------------------------
      virtual void requestStop() = 0;

      //-----------------------------------------------------
      ///\brief               Get the plugin associated with this instance
      ///\return              Associated plugin name
      //-----------------------------------------------------
      virtual std::string getPluginName() const = 0;
   };
	
} // namespace pluginSystem	
	
	