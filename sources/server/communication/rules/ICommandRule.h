#pragma once

#include "IRule.h"
#include "communication/command/DeviceCommand.h"

namespace communication { namespace rules {

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
      ///\param [in]    targetDevice   The targetted device
      ///\param [in]    commandData    The command data
      ///\return an xpl message ready to be sent
      //------------------------------------
      virtual boost::shared_ptr< shared::xpl::CXplMessage > createXplCommand(database::entities::CDevice & targetDevice, command::CDeviceCommand & deviceCommand) = 0;

      //--------------------------------
      ///\brief generate a random virtual device identifier (i.e. : 0x123456-2)
      ///\return a virtual device identifier
      //--------------------------------
      virtual std::string generateVirtualDeviceIdentifier() = 0;

   };

} //namespace rules
} //namespace communication

