#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "SingleTemperature.h"

namespace device
{
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
   /// \param[in]	reader I/O access object
   //--------------------------------------------------------------
   CProgrammableResolutionDigitalThermometer(EOneWireFamily family,
                                             const std::string &id,
                                             boost::shared_ptr<ioInterfaces::ITemperature> io)
       : CSingleTemperature(family, id, "DS18B20", io, kProgrammableResolutionDigitalThermometer)
   {
   }

   virtual ~CProgrammableResolutionDigitalThermometer() = default;
};

} // namespace device
