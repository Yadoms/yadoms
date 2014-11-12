#pragma once

#include "../IRule.h"
#include "../ICommandRule.h"
#include <shared/enumeration/EnumHelpers.hpp>


namespace xplrules { namespace rfxLanXpl {

   class CControlBasic : public IRule, public ICommandRule
   {
   public:
      CControlBasic();
      virtual ~CControlBasic();
      
      //------------------------------------
      ///\brief Provide the xpl protocol implemented by this class
      ///\return the xpl protocol
      //------------------------------------
      static const xplcore::CXplMessageSchemaIdentifier getProtocol();

      // IRule implementation
      virtual const CDeviceIdentifier getDeviceAddressFromMessage(xplcore::CXplMessage & msg);
      virtual KeywordList identifyKeywords(xplcore::CXplMessage & msg);
      // [END] IRule implementation
     
      // ICommandRule implemntation
      virtual boost::shared_ptr< xplcore::CXplMessage > createXplCommand(boost::shared_ptr<const yApi::IDeviceCommand> & commandData, const std::string & rfxAddress, const shared::CDataContainer & innerDetails);
      virtual std::string generateVirtualDeviceIdentifier();
      // [END] ICommandRule implemntation
   private:
      DECLARE_ENUM_HEADER(EType,
         ((Ninja))
         ((Mertik))
         ((Output))
      );

      static xplcore::CXplMessageSchemaIdentifier m_protocol;

      static std::string m_keywordCurrent;
      static std::string m_keywordType;
      static std::string m_keywordDevice;
   };
   
} //namespace rfxLanXpl
} //namespace xplrules
