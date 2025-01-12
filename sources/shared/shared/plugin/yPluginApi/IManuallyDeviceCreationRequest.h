#pragma once

#include "IManuallyDeviceCreationData.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //-----------------------------------------------------
         ///\brief Interface for manually device creation request
         //-----------------------------------------------------
         class IManuallyDeviceCreationRequest
         {
         public:

            //-----------------------------------------------------
            ///\brief Destructor
            //-----------------------------------------------------
            virtual ~IManuallyDeviceCreationRequest()
            {
            }

            //-----------------------------------------------------
            ///\brief Get the request data
            //-----------------------------------------------------
            virtual const IManuallyDeviceCreationData& getData() const = 0;

            //-----------------------------------------------------
            ///\brief Answer the request with success
            //-----------------------------------------------------
            virtual void sendSuccess() = 0;

            //-----------------------------------------------------
            ///\brief Answer the request with error
            ///\param [in] errorMessage  The error message
            //-----------------------------------------------------
            virtual void sendError(const std::string& errorMessage) = 0;
         };
      }
   }
} // namespace shared::plugin::yPluginApi	


