#pragma once

#include "../IRule.h"
#include "../IReadRule.h"
#include "../ICommandRule.h"
#include "../ISupportManuallyDeviceCreationRule.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl {

   class CAcBasic : public IRule, public IReadRule, public ICommandRule, public ISupportManuallyDeviceCreationRule
   {
   public:
      CAcBasic();
      virtual ~CAcBasic();
      
      //------------------------------------
      ///\brief Provide the xpl protocol implemented by this class
      ///\return the xpl protocol
      //------------------------------------
      static const xplcore::CXplMessageSchemaIdentifier getProtocol();

      // IRule implementation
      virtual const CDeviceIdentifier getDeviceAddressFromMessage(xplcore::CXplMessage & msg);
      virtual KeywordList identifyKeywords(xplcore::CXplMessage & msg);
      // [END] IRule implementation

      // IReadRule implemntation
      virtual MessageContent extractMessageData(xplcore::CXplMessage & msg);
      // [END] IReadRule implemntation

      // ICommandRule implemntation
      virtual boost::shared_ptr< xplcore::CXplMessage > createXplCommand(boost::shared_ptr<const yApi::IDeviceCommand> & commandData, const std::string & rfxAddress, const shared::CDataContainer & innerDetails);
      virtual std::string generateVirtualDeviceIdentifier();
      // [END] ICommandRule implemntation

      // ISupportManuallyDeviceCreationRule implementation
      virtual const CDeviceContainer generateDeviceParameters(shared::CDataContainer & configuration) const;
      // [END] ISupportManuallyDeviceCreationRule implementation  

      //-----------------------------------------------------
      ///\brief                     The switch state
      //-----------------------------------------------------
      DECLARE_ENUM_HEADER(EState,
         ((Off))
         ((On))
         ((Dim))
      );

   private:

      static std::string m_keywordAddress;
      static std::string m_keywordUnit;
      static std::string m_keywordCommand;
      static std::string m_keywordCommandValues;
      static std::string m_keywordLevel;
      static xplcore::CXplMessageSchemaIdentifier m_protocol;
   };
   
} //namespace rfxLanXpl
} //namespace xplrules
