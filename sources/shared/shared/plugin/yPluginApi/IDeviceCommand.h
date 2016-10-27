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


            //-----------------------------------------------------
            ///\brief               To string transformer
            ///\param[in] command   The commande to print out
            ///\return              The formated string representine the command
            //-----------------------------------------------------
            static std::string toString(boost::shared_ptr<const IDeviceCommand> command)
            {
               return (boost::format("%1%.%2%=%3%") % command->getDevice() % command->getKeyword() % command->getBody()).str();
            }
         };
      }
   }
} // namespace shared::plugin::yPluginApi	


