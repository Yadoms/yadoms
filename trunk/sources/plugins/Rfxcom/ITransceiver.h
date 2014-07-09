#pragma once
#include <shared/plugin/yadomsApi/IYadomsApi.h>
#include <shared/DataContainer.h>
#include "Buffer.hpp"

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
   virtual void processReset() = 0;

   //--------------------------------------------------------------
   /// \brief	                     Send Yadoms command to RFXCom
   /// \param [in] command          The received command (JSON string)
   /// \param [in] deviceParameters The device parameters (JSON string)
   //--------------------------------------------------------------
   virtual void send(const shared::CDataContainer& command, const shared::CDataContainer& deviceParameters) = 0;

   //--------------------------------------------------------------
   /// \brief	                     Decode RFXCom data and historize to Yadoms
   /// \param [in] data             Data received
   //--------------------------------------------------------------
   virtual void historize(const CByteBuffer& data) const = 0;
};
