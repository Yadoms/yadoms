#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "SingleTemperature.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace device {

   //--------------------------------------------------------------
   /// \brief	Temperature Device (Family 28)
   //--------------------------------------------------------------
   class CProgrammableResolutionDigitalThermometer : public CSingleTemperature
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	family Device family
      /// \param[in]	id Device serial number
      /// \param[in]	context yApi context
      /// \param[in]	reader I/O access object
      //--------------------------------------------------------------
      CProgrammableResolutionDigitalThermometer(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::ITemperature> io)
         :CSingleTemperature(family, id, "DS18B20", context, io, kProgrammableResolutionDigitalThermometer) {}

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CProgrammableResolutionDigitalThermometer() {}
   };

} // namespace device