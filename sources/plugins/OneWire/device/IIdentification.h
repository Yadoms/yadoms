#pragma once
#include "../Common.h"
#include <shared/DataContainer.h>

namespace device
{
//--------------------------------------------------------------
/// \brief	1-Wire Device identification interface
//--------------------------------------------------------------
class IIdentification
{
public:
   virtual ~IIdentification() = default;

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
   virtual const std::string &deviceName() const = 0;

   //--------------------------------------------------------------
   /// \brief	Device model (chip reference)
   //--------------------------------------------------------------
   virtual const std::string &model() const = 0;

   //--------------------------------------------------------------
   /// \brief	Details (1-wire specific data)
   //--------------------------------------------------------------
   virtual const boost::shared_ptr<shared::CDataContainer> &details() const = 0;
};
} // namespace device
