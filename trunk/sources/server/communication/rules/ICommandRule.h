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
      virtual void fillMessage(boost::shared_ptr< shared::xpl::CXplMessage > & messagetoFill, database::entities::CDevice & targetDevice, communication::command::CDeviceCommand & commandData) = 0;
   };

} //namespace rules
} //namespace communication

