#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "SingleTemperature.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace device {

   //--------------------------------------------------------------
   /// \brief	Thermachron Device (Family 21)
   //--------------------------------------------------------------
   class CThermachron : public CSingleTemperature
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	family Device family
      /// \param[in]	id Device serial number
      /// \param[in]	context yApi context
      /// \param[in]	reader I/O access object
      //--------------------------------------------------------------
      CThermachron(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::ITemperature> io)
         :CSingleTemperature(family, id, "DS1921", context, io, kThermachron) {}

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CThermachron() {}
   };

} // namespace device