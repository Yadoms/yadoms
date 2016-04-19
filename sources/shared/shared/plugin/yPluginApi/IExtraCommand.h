#pragma once
#include <shared/DataContainer.h>

namespace shared { namespace plugin { namespace yPluginApi
{
   //-----------------------------------------------------
   ///\brief The API used for the commands on plugin
   //-----------------------------------------------------
   class IExtraCommand
   {
   public:
      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~IExtraCommand() {}

      //-----------------------------------------------------
      ///\brief               Get the extra command name
      ///\return              The extra command name
      //-----------------------------------------------------
      virtual const std::string& getCommand() const = 0;

      //-----------------------------------------------------
      ///\brief Obtain the command data
      ///\return a datacontainer
      //-----------------------------------------------------
      virtual const CDataContainer & getData() const = 0;
};
	
} } } // namespace shared::plugin::yPluginApi	
	
	