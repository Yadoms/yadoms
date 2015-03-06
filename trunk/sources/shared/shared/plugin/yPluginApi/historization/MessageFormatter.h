#pragma once
#include <shared/Export.h>
#include "IHistorizable.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   //-----------------------------------------------------
   ///\brief The message formatter
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CMessageFormatter
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor, from individual parameters
      ///\param[in] from            Sender recipient ID
      ///\param[in] to              Receiver recipient ID
      ///\param[in] body            Message content
      //-----------------------------------------------------
      CMessageFormatter(int from, int to, const std::string& body);

      //-----------------------------------------------------
      ///\brief                     Constructor, from Yadoms command
      ///\param[in] yadomsCommand   Yadoms command
      ///\throw                     shared::exception::CInvalidParameter or COutOfRange if fail to parse command
      //-----------------------------------------------------
      CMessageFormatter(const std::string& yadomsCommand);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CMessageFormatter();

      //-----------------------------------------------------
      ///\brief               Get the sender value
      ///\return              The sender value
      //-----------------------------------------------------
      int from() const;

      //-----------------------------------------------------
      ///\brief               Get the recipient value
      ///\return              The recipient value
      //-----------------------------------------------------
      int to() const;

      //-----------------------------------------------------
      ///\brief               Get the body value
      ///\return              The body value
      //-----------------------------------------------------
      const std::string& body() const;

      //-----------------------------------------------------
      ///\brief                     Format value to Yadoms format
      ///\return                    Formatted data
      //-----------------------------------------------------
      virtual std::string formatValue() const;

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

} } } } // namespace shared::plugin::yPluginApi::historization

