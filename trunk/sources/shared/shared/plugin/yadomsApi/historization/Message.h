#pragma once
#include <shared/Export.h>
#include <shared/DataContainer.h>
#include "IHistorizable.h"

namespace shared { namespace plugin { namespace yadomsApi { namespace historization
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
      //-----------------------------------------------------
      CMessage(const std::string& keywordName);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CMessage();

      // IHistorizable implementation
      virtual const std::string& getKeyword() const;
      virtual const CStandardCapacity& getCapacity() const;
      virtual const std::string formatValue() const;
      virtual const EMeasureType& getMeasureType() const;
      // [END] IHistorizable implementation;

      //-----------------------------------------------------
      ///\brief                     Set value from Yadoms command
      ///\param[in] yadomsCommand   Yadoms command container
      ///\throw                     shared::exception::CInvalidParameter or COutOfRange if fail to parse command
      //-----------------------------------------------------
      void set(const shared::CDataContainer& yadomsCommand);

      //-----------------------------------------------------
      ///\brief                     Set value from on/off state
      ///\param[in] isOn            switch on/off state (true = on)
      //-----------------------------------------------------
      void set(const std::string& from, const std::string& to, const std::string& body);

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
      ///\brief                     The keyword name
      //-----------------------------------------------------
      const std::string m_keywordName;

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



} } } } // namespace shared::plugin::yadomsApi::historization

