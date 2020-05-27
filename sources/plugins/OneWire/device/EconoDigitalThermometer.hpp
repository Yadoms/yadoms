#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "SingleTemperature.h"

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
   /// \param[in]	reader I/O access object
   //--------------------------------------------------------------
   CEconoDigitalThermometer(EOneWireFamily family,
                            const std::string &id,
                            boost::shared_ptr<ioInterfaces::ITemperature> io)
       : CSingleTemperature(family, id, "DS1922", io, kEconoDigitalThermometer)
   {
   }

   virtual ~CEconoDigitalThermometer() = default;
};
} // namespace device
