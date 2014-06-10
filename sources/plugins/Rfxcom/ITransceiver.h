#pragma once
#include <shared/plugin/yadomsApi/IYadomsApi.h>

// Shortcut to yadomsApi namespace
namespace yApi = shared::plugin::yadomsApi;

//--------------------------------------------------------------
/// \brief	The RFXCom protocol interface
//--------------------------------------------------------------
class ITransceiver
{  
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~ITransceiver() {}

   //--------------------------------------------------------------
   /// \brief	                     Reset the transceiver
   //--------------------------------------------------------------
   virtual void sendReset() = 0;

   //--------------------------------------------------------------
   /// \brief	                     Send Yadoms command to RFXCom
   /// \param [in] command          The received command
   //--------------------------------------------------------------
   virtual void send(boost::shared_ptr<yApi::IDeviceCommand> command) = 0;
};



