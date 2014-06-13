#pragma once
#include <shared/Export.h>


namespace shared { namespace plugin { namespace yadomsApi { namespace commands
{
   //-----------------------------------------------------
   ///\brief The message command parser
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CMessage
   {
   public:
      //-----------------------------------------------------
      ///\brief               Constructor
      ///\param[in] command   Yadoms command, as JSON string
      ///\throw               shared::exception::CInvalidParameter if fail to parse command
      //-----------------------------------------------------
      CMessage(const std::string& command);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CMessage();

      //-----------------------------------------------------
      ///\brief               Get the sender value
      ///\return              The sender value
      //-----------------------------------------------------
      const std::string& from() const;

      //-----------------------------------------------------
      ///\brief               Get the recipient value
      ///\return              The recipient value
      //-----------------------------------------------------
      const std::string& to() const;

      //-----------------------------------------------------
      ///\brief               Get the body value
      ///\return              The body value
      //-----------------------------------------------------
      const std::string& body() const;

   private:
      //-----------------------------------------------------
      ///\brief               The sender value
      //-----------------------------------------------------
      std::string m_from;

      //-----------------------------------------------------
      ///\brief               The recipient value
      //-----------------------------------------------------
      std::string m_to;

      //-----------------------------------------------------
      ///\brief               The body value
      //-----------------------------------------------------
      std::string m_body;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

