#pragma once
#include <shared/DataContainer.h>
#include "device/IDevice.h"

//--------------------------------------------------------------
/// \brief	OneWire Engine interface
//--------------------------------------------------------------
class IEngine
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IEngine() {}

   //--------------------------------------------------------------
   /// \brief Evaluate if new configuration will require to restart engine
   /// \param[in] newConfigurationData  The new configuration
   /// \return bool if restart is required
   //--------------------------------------------------------------
   virtual bool newConfigurationRequireRestart(const boost::shared_ptr<shared::CDataContainer>& newConfigurationData) const = 0;

   //--------------------------------------------------------------
   /// \brief Scan 1-wire network to list connected devices
   /// \return Connected devices list (map key is full device name "ff-iiiiiiiiiiii")
   //--------------------------------------------------------------
   virtual std::map<std::string, boost::shared_ptr<device::IDevice> > scanNetwork() = 0;
};
