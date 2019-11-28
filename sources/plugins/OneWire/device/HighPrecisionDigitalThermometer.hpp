#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "SingleTemperature.h"

namespace device
{
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
   /// \param[in]	reader I/O access object
   //--------------------------------------------------------------
   CHighPrecisionDigitalThermometer(EOneWireFamily family,
                                    const std::string &id,
                                    boost::shared_ptr<ioInterfaces::ITemperature> io)
       : CSingleTemperature(family, id, "DS18S20", io, kHighPrecisionDigitalThermometer)
   {
   }

   virtual ~CHighPrecisionDigitalThermometer() = default;
};
} // namespace device
