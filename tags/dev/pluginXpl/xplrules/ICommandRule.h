#pragma once

#include "IRule.h"
#include <shared/plugin/yadomsApi/IYadomsApi.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

namespace xplrules {

   //------------------------------------
   ///\brief   This interface describe rules which handles commands
   //------------------------------------
   class ICommandRule : public IRule
   {
   public:
      //------------------------------------
      ///\brief Destructor
      //------------------------------------
      virtual ~ICommandRule(){}

      //------------------------------------
      ///\brief Fill a command message
      ///\param [in]    commandData    The command data
      ///\return an xpl message ready to be sent
      //------------------------------------
      virtual boost::shared_ptr< xplcore::CXplMessage > createXplCommand(boost::shared_ptr<yApi::IDeviceCommand> & commandData, const std::string & rfxAddress) = 0;
      
      //--------------------------------
      ///\brief generate a random virtual device identifier (i.e. : 0x123456-2)
      ///\return a virtual device identifier
      //--------------------------------
      virtual std::string generateVirtualDeviceIdentifier() = 0;


   };

} //namespace xplrules


