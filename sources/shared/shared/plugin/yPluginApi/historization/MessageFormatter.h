#pragma once
#include "IMessageFormatter.h"

namespace shared
{
   namespace plugin
   {
      namespace yPluginApi
      {
         namespace historization
         {
            //-----------------------------------------------------
            ///\brief The message formatter
            //-----------------------------------------------------
            class CMessageFormatter : public IMessageFormatter
            {
            public:
               //-----------------------------------------------------
               ///\brief                     Constructor, from individual parameters
               ///\param[in] from            Sender recipient ID
               ///\param[in] to              Receiver recipient ID
               ///\param[in] body            Message content
               //-----------------------------------------------------
               CMessageFormatter(int from,
                                 int to,
                                 const std::string& body);

               //-----------------------------------------------------
               ///\brief                     Constructor, from Yadoms command
               ///\param[in] yadomsCommand   Yadoms command
               ///\throw                     shared::exception::CInvalidParameter or COutOfRange if fail to parse command
               //-----------------------------------------------------
               explicit CMessageFormatter(const std::string& yadomsCommand);

               //-----------------------------------------------------
               ///\brief                     Destructor
               //-----------------------------------------------------
               virtual ~CMessageFormatter();

               // IMessageFormatter implementation
               int from() const override;
               bool isFromProvided() const override;
               int to() const override;
               bool isToProvided() const override;
               const std::string& body() const override;
               bool isBodyProvided() const override;
               std::string formatValue() const override;
               // [END] IMessageFormatter implementation

            private:
               //-----------------------------------------------------
               ///\brief               The sender value
               //-----------------------------------------------------
               int m_from;

               //-----------------------------------------------------
               ///\brief               The recipient value
               //-----------------------------------------------------
               int m_to;

               //-----------------------------------------------------
               ///\brief               The body value
               //-----------------------------------------------------
               std::string m_body;
            };
         }
      }
   }
} // namespace shared::plugin::yPluginApi::historization


