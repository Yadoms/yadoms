#pragma once

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
   /// \param [in] command          The received command (JSON string)
   /// \param [in] deviceParameters The device parameters (JSON string)
   //--------------------------------------------------------------
   virtual void send(const std::string& command, const std::string& deviceParameters) = 0;
};



