#pragma once
#include <shared/DataContainer.h>

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //-----------------------------------------------------
         ///\brief Interface for a device configuration request
         //-----------------------------------------------------
         class IDeviceConfigurationSchemaRequest
         {
         public:

            //-----------------------------------------------------
            ///\brief Destructor
            //-----------------------------------------------------
            virtual ~IDeviceConfigurationSchemaRequest()
            {
            }

            //-----------------------------------------------------
            ///\brief Get the device which configuration schema is requested
            //-----------------------------------------------------
            virtual std::string device() const = 0;

            //-----------------------------------------------------
            ///\brief Answer the request with success, providing the device configuration schema
            ///\param [in] deviceConfigurationSchema  The device configuration schema
            //-----------------------------------------------------
            virtual void sendSuccess(const boost::shared_ptr<CDataContainer>& deviceConfigurationSchema) = 0;

            //-----------------------------------------------------
            ///\brief Answer the request with error
            ///\param [in] errorMessage  The error message
            //-----------------------------------------------------
            virtual void sendError(const std::string& errorMessage) = 0;
         };
      }
   }
} // namespace shared::plugin::yPluginApi	


