#pragma once
#include "shared/DataContainer.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //-----------------------------------------------------
         ///\brief The API used for the commands on device
         //-----------------------------------------------------
         class IDeviceRemoved
         {
         public:
            //-----------------------------------------------------
            ///\brief               Destructor
            //-----------------------------------------------------
            virtual ~IDeviceRemoved()
            {
            }

            //-----------------------------------------------------
            ///\brief               Get the removed device
            ///\return              The removed device
            //-----------------------------------------------------
            virtual const std::string& device() const = 0;

            //-----------------------------------------------------
            ///\brief               Get the removed device details
            ///\return              The removed device details
            //-----------------------------------------------------
            virtual const boost::shared_ptr<CDataContainer>& details() const = 0;
         };
      }
   }
} // namespace shared::plugin::yPluginApi	


