#pragma once
#include "../Common.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace device {
   //--------------------------------------------------------------
   /// \brief	1-Wire Device identification interface
   //--------------------------------------------------------------
   class IIdentification
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IIdentification() {}

      //--------------------------------------------------------------
      /// \brief	Declare device to Yadoms
      /// \param[in] context IYPluginApi reference
      //--------------------------------------------------------------
      virtual void declare(boost::shared_ptr<yApi::IYPluginApi> context) const = 0;

      //--------------------------------------------------------------
      /// \brief	Device family
      //--------------------------------------------------------------
      virtual EOneWireFamily family() const = 0;

      //--------------------------------------------------------------
      /// \brief	Device Id
      //--------------------------------------------------------------
      virtual std::string id() const = 0;

      //--------------------------------------------------------------
      /// \brief	Convert identification to Yadoms device name
      //--------------------------------------------------------------
      virtual const std::string& deviceName() const = 0;

      //--------------------------------------------------------------
      /// \brief	Device model (chip reference)
      //--------------------------------------------------------------
      virtual const std::string& model() const = 0;
   };
} // namespace device