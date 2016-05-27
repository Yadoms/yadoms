#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "SingleTemperature.h"
#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace device
{
   //--------------------------------------------------------------
   /// \brief	Temperature Device (Family 22)
   //--------------------------------------------------------------
   class CEconoDigitalThermometer : public CSingleTemperature
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in]	family Device family
      /// \param[in]	id Device serial number
      /// \param[in]	api yApi context
      /// \param[in]	reader I/O access object
      //--------------------------------------------------------------
      CEconoDigitalThermometer(EOneWireFamily family,
                               const std::string& id,
                               boost::shared_ptr<yApi::IYPluginApi> api,
                               boost::shared_ptr<ioInterfaces::ITemperature> io)
         :CSingleTemperature(family, id, "DS1922", api, io, kEconoDigitalThermometer)
      {
      }

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CEconoDigitalThermometer()
      {
      }
   };
} // namespace device