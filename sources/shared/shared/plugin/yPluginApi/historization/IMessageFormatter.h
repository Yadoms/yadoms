#pragma once

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief The message formatter interface
            //-----------------------------------------------------
            class IMessageFormatter
            {
            public:
               //-----------------------------------------------------
               ///\brief               Get the sender value
               ///\return              The sender value
               //-----------------------------------------------------
               virtual int from() const = 0;

               //-----------------------------------------------------
               ///\brief               Tells if the sender value is provided
               ///\return              true if the sender value is provided, false in other case
               //-----------------------------------------------------
               virtual bool isFromProvided() const = 0;

               //-----------------------------------------------------
               ///\brief               Get the recipient value
               ///\return              The recipient value
               //-----------------------------------------------------
               virtual int to() const = 0;

               //-----------------------------------------------------
               ///\brief               Tells if the recipient value is provided
               ///\return              true if the recipient value is provided, false in other case
               //-----------------------------------------------------
               virtual bool isToProvided() const = 0;

               //-----------------------------------------------------
               ///\brief               Get the body value
               ///\return              The body value
               //-----------------------------------------------------
               virtual const std::string& body() const = 0;

               //-----------------------------------------------------
               ///\brief               Tells if the body value is provided
               ///\return              true if the body value is provided, false in other case
               //-----------------------------------------------------
               virtual bool isBodyProvided() const = 0;

               //-----------------------------------------------------
               ///\brief                     Format value to Yadoms format
               ///\return                    Formatted data
               //-----------------------------------------------------
               virtual std::string formatValue() const = 0;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization
