#pragma once

#include "../IRule.h"
#include "../IReadRule.h"
#include "../ICommandRule.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl {

   class CAcBasic : public IRule, public IReadRule, public ICommandRule
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
      virtual boost::shared_ptr< xplcore::CXplMessage > createXplCommand(boost::shared_ptr<const yApi::IDeviceCommand> & commandData, const std::string & rfxAddress);
      virtual std::string generateVirtualDeviceIdentifier();
      // [END] ICommandRule implemntation
   private:
      //-----------------------------------------------------
      ///\brief                     The switch state
      //-----------------------------------------------------
      DECLARE_ENUM_HEADER(EState,
         ((Off)(0))
         ((On)(1))
         ((Dim)(2))
      );


      static std::string m_keywordAddress;
      static std::string m_keywordUnit;
      static std::string m_keywordCommand;
      static std::string m_keywordCommandValues;
      static std::string m_keywordLevel;
      static xplcore::CXplMessageSchemaIdentifier m_protocol;
   };
   
} //namespace rfxLanXpl
} //namespace xplrules
