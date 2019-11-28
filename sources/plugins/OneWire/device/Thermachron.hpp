#pragma once
#include "IDevice.h"
#include "IIdentification.h"
#include "SingleTemperature.h"

namespace device
{
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
   /// \param[in]	reader I/O access object
   //--------------------------------------------------------------
   CThermachron(EOneWireFamily family,
                const std::string &id,
                boost::shared_ptr<ioInterfaces::ITemperature> io)
       : CSingleTemperature(family, id, "DS1921", io, kThermachron)
   {
   }

   virtual ~CThermachron() = default;
};

} // namespace device
