#pragma once

#include "../IRule.h"
#include "../IReadRule.h"
#include "../ICommandRule.h"
#include "../ISupportManuallyDeviceCreationRule.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl {

   class CX10Security : public IRule, public IReadRule, public ICommandRule, public ISupportManuallyDeviceCreationRule
   {
   public:
      CX10Security();
      virtual ~CX10Security();
      
      //------------------------------------
      ///\brief Provide the xpl protocol implemented by this class
      ///\return the xpl protocol
      //------------------------------------
      static const xplcore::CXplMessageSchemaIdentifier getProtocol();

      // IRule implementation
      virtual const CDeviceIdentifier getDeviceAddressFromMessage(xplcore::CXplMessage & msg);
      virtual KeywordList identifyKeywords(xplcore::CXplMessage & msg);
      // [END] IRule implementation
     
      // IReadRule implementation
      virtual MessageContent extractMessageData(xplcore::CXplMessage & msg);
      // [END] IReadRule implementation
     
      // ICommandRule implemntation
      virtual boost::shared_ptr< xplcore::CXplMessage > createXplCommand(boost::shared_ptr<const yApi::IDeviceCommand> & commandData, const std::string & rfxAddress, const shared::CDataContainer & innerDetails);
      virtual std::string generateVirtualDeviceIdentifier();
      // [END] ICommandRule implemntation
     
      // ISupportManuallyDeviceCreationRule implementation
      virtual const CDeviceContainer generateDeviceParameters(boost::shared_ptr<const yApi::IManuallyDeviceCreationData> configuration) const;
      // [END] ISupportManuallyDeviceCreationRule implementation   
   
   private:

      static xplcore::CXplMessageSchemaIdentifier m_protocol;
      static std::string m_keywordCommand;
      static std::string m_keywordDevice;
      static std::string m_keywordType;
      static std::string m_keywordTamper;
      static std::string m_keywordLowBattery;
      static std::string m_keywordDelay;
   };
   
} //namespace rfxLanXpl
} //namespace xplrules
