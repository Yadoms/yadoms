#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include <shared/Field.hpp>

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
      ///\param[in] command   Yadoms command container
      ///\throw               shared::exception::CInvalidParameter if fail to parse command
      //-----------------------------------------------------
      CMessage(const shared::CDataContainer& command);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CMessage();

      //-----------------------------------------------------
      ///\brief               Get the sender value
      ///\return              The sender value
      //-----------------------------------------------------
      const CField<std::string>& from() const;

      //-----------------------------------------------------
      ///\brief               Get the recipient value
      ///\return              The recipient value
      //-----------------------------------------------------
      const CField<std::string>& to() const;

      //-----------------------------------------------------
      ///\brief               Get the body value
      ///\return              The body value
      //-----------------------------------------------------
      const CField<std::string>& body() const;

   private:
      //-----------------------------------------------------
      ///\brief               The sender value
      //-----------------------------------------------------
      CField<std::string> m_from;

      //-----------------------------------------------------
      ///\brief               The recipient value
      //-----------------------------------------------------
      CField<std::string> m_to;

      //-----------------------------------------------------
      ///\brief               The body value
      //-----------------------------------------------------
      CField<std::string> m_body;
   };



} } } } // namespace shared::plugin::yadomsApi::commands

