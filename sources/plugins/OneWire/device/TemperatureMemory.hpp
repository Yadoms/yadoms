#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "SingleTemperature.h"

namespace device
{
//--------------------------------------------------------------
/// \brief	Temperature Device (Family 3B)
//--------------------------------------------------------------
class CTemperatureMemory : public CSingleTemperature
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in]	family Device family
   /// \param[in]	id Device serial number
   /// \param[in]	reader I/O access object
   //--------------------------------------------------------------
   CTemperatureMemory(EOneWireFamily family,
                      const std::string &id,
                      boost::shared_ptr<ioInterfaces::ITemperature> io)
       : CSingleTemperature(family, id, "DS1825, MAX31826", io, kTemperatureMemory)
   {
   }

   virtual ~CTemperatureMemory() = default;
};

} // namespace device
