#pragma once

#include "../IRule.h"
#include "../IReadRule.h"
#include "../ICommandRule.h"
#include "../ISupportManuallyDeviceCreationRule.h"
#include <shared/enumeration/EnumHelpers.hpp>

namespace xplrules { namespace rfxLanXpl {

   class CX10Basic : public IRule, public IReadRule, public ICommandRule, public ISupportManuallyDeviceCreationRule
   {
   public:
      CX10Basic();
      virtual ~CX10Basic();
      
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
      virtual const CDeviceContainer generateDeviceParameters(shared::CDataContainer & configuration) const;
      // [END] ISupportManuallyDeviceCreationRule implementation   
   
   private:
      DECLARE_ENUM_HEADER(EProtocol,
         ((X10))
         ((Arc))
         ((Flamingo))
         ((Koppla))
         ((Waveman))
         ((Harrison))
         ((He105))
         ((Rts10))
      )

      DECLARE_ENUM_HEADER(ECommands,
         ((On))
         ((Off))
         ((Dim))
         ((Bright))
         ((AllLightsOn))
         ((AllLightsOff))
      )

   private:
      //------------------------------------
      ///\brief Provide the device comercial name from its protocol
      ///\return the device commercial name
      //------------------------------------
      std::string getDeviceCommercialName(const CX10Basic::EProtocol & protocol) const;


      static xplcore::CXplMessageSchemaIdentifier m_protocol;
      static std::string m_keywordCommand;
      static std::string m_keywordLevel;
      static std::string m_keywordProtocol;
      static std::string m_keywordDevice;
   };
   
} //namespace rfxLanXpl
} //namespace xplrules
