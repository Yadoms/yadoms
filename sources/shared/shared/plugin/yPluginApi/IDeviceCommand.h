#pragma once

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         //-----------------------------------------------------
         ///\brief The API used for the commands on device
         //-----------------------------------------------------
         class IDeviceCommand
         {
         public:
            //-----------------------------------------------------
            ///\brief               Destructor
            //-----------------------------------------------------
            virtual ~IDeviceCommand()
            {
            }

            //-----------------------------------------------------
            ///\brief               Get the target device
            ///\return              The device to command
            //-----------------------------------------------------
            virtual const std::string& getDevice() const = 0;

            //-----------------------------------------------------
            ///\brief               Get the target keyword
            ///\return              The keyword to command
            //-----------------------------------------------------
            virtual const std::string& getKeyword() const = 0;

            //-----------------------------------------------------
            ///\brief               Get the command body
            ///\return              The command body, as string
            //-----------------------------------------------------
            virtual const std::string& getBody() const = 0;
         };
      }
   }
} // namespace shared::plugin::yPluginApi	


