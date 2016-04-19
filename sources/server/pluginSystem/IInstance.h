#pragma once
#include <server/database/sqlite/requesters/Plugin.h>

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

      //-----------------------------------------------------
      ///\brief               Request to stop instance
      //-----------------------------------------------------
      virtual void requestStop() = 0;

      //-----------------------------------------------------
      ///\brief               Kill the instance
      //-----------------------------------------------------
      virtual void kill() = 0;

      //--------------------------------------------------------------
      /// \brief			            Notify the plugin about its configuration changed
      /// \param  newConfiguration  The new configuration to apply
      //--------------------------------------------------------------
      virtual void updateConfiguration(const shared::CDataContainer & newConfiguration) const = 0;

      //-----------------------------------------------------
      ///\brief               Get information about this instance
      ///\return              Instance information
      //-----------------------------------------------------
      virtual boost::shared_ptr<const database::entities::CPlugin> about() const = 0;

      //-----------------------------------------------------
      ///\brief               Get information about the plugin associated with this instance
      ///\return              Plugin information
      //-----------------------------------------------------
      virtual boost::shared_ptr<const shared::plugin::information::IInformation> aboutPlugin() const = 0;
   };
	
} // namespace pluginSystem	
	
	