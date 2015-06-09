#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "SingleTemperature.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace device {

   //--------------------------------------------------------------
   /// \brief	High Precision Digital Thermometer Device (Family 10)
   //--------------------------------------------------------------
   class CHighPrecisionDigitalThermometer : public CSingleTemperature
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	family Device family
      /// \param[in]	id Device serial number
      /// \param[in]	context yApi context
      /// \param[in]	reader I/O access object
      //--------------------------------------------------------------
      CHighPrecisionDigitalThermometer(EOneWireFamily family, const std::string& id, boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<ioInterfaces::ITemperature> io)
         :CSingleTemperature(family, id, "DS18S20", context, io, kHighPrecisionDigitalThermometer) {}

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CHighPrecisionDigitalThermometer() {}
   };

} // namespace device