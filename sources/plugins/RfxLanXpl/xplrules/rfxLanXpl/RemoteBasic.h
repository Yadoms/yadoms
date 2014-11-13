#pragma once

#include "../IRule.h"
#include "../IReadRule.h"
#include "../ICommandRule.h"
#include "../ISupportManuallyDeviceCreationRule.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl {

   class CRemoteBasic : public IRule, public IReadRule, public ICommandRule, public ISupportManuallyDeviceCreationRule
   {
   public:
      CRemoteBasic();
      virtual ~CRemoteBasic();
      
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
      DECLARE_ENUM_HEADER(ERemoteType,
         ((PC))
         ((Medion))
         ((Ati))
         ((AtiPlus))
      )


      static xplcore::CXplMessageSchemaIdentifier m_protocol;
      static std::string m_keywordKeys;
      static std::string m_keywordDevice;
   };
   
} //namespace rfxLanXpl
} //namespace xplrules
