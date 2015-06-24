#pragma once
#include "../Common.h"
#include "IIdentification.h"

namespace device {

   //--------------------------------------------------------------
   /// \brief	1-Wire Device interface
   //--------------------------------------------------------------
   class IDevice
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IDevice() {}

      //--------------------------------------------------------------
      /// \brief	Historize device data
      //--------------------------------------------------------------
      virtual boost::shared_ptr<const IIdentification> ident() const = 0;

      //--------------------------------------------------------------
      /// \brief	Declare device in Yadoms
      //--------------------------------------------------------------
      virtual void declare() = 0;

      //--------------------------------------------------------------
      /// \brief	Historize device data
      //--------------------------------------------------------------
      virtual void historize() = 0;

      //--------------------------------------------------------------
      /// \brief	Change a keyword state
      //--------------------------------------------------------------
      virtual void set(const std::string& keyword, const std::string& command) = 0;
   };

} // namespace device