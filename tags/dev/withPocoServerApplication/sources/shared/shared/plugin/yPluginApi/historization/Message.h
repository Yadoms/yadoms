#pragma once
#include <shared/Export.h>
#include "IHistorizable.h"
#include "MessageFormatter.h"

namespace shared { namespace plugin { namespace yPluginApi { namespace historization
{
   //-----------------------------------------------------
   ///\brief A message historizable object
   //-----------------------------------------------------
   class YADOMS_SHARED_EXPORT CMessage : public IHistorizable
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in] keywordName     Yadoms keyword name
      ///\param[in] associatedRecipientField The associated recipient field name ("mobile", "email", etc...)
      ///\param[in] accessMode      Access mode
      //-----------------------------------------------------
      CMessage(const std::string& keywordName, const std::string& associatedRecipientField, const EKeywordAccessMode& accessMode);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CMessage();

      // IHistorizable implementation
      virtual const std::string& getKeyword() const;
      virtual const CStandardCapacity& getCapacity() const;
      virtual const EKeywordAccessMode& getAccessMode() const;
      virtual std::string formatValue() const;
      virtual const EMeasureType& getMeasureType() const;
      virtual CDataContainer getTypeInfo() const;
      // [END] IHistorizable implementation;

      //-----------------------------------------------------
      ///\brief                     Set value from Yadoms command
      ///\param[in] yadomsCommand   Yadoms command
      ///\throw                     shared::exception::CInvalidParameter or COutOfRange if fail to parse command
      //-----------------------------------------------------
      void set(const std::string& yadomsCommand);

      //-----------------------------------------------------
      ///\brief                     Set value from on/off state
      ///\param[in] from            Sender recipient ID
      ///\param[in] to              Receiver recipient ID
      ///\param[in] body            Message content
      //-----------------------------------------------------
      void set(int from, int to, const std::string& body);

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

   private:
      //-----------------------------------------------------
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

      //-----------------------------------------------------
      ///\brief                     The associated recipient field name
      //-----------------------------------------------------
      const std::string m_associatedRecipientField;

      //-----------------------------------------------------
      ///\brief                     The access mode
      //-----------------------------------------------------
      const EKeywordAccessMode& m_accessMode;

      //-----------------------------------------------------
      ///\brief                     The message content
      //-----------------------------------------------------
      boost::shared_ptr<CMessageFormatter> m_content;
   };



} } } } // namespace shared::plugin::yPluginApi::historization

