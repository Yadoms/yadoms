#pragma once

#include "ITransceiver.h"
#include "IPort.h"
#include "RFXtrx.h"

//--------------------------------------------------------------
/// \brief	This class implement the RFXCom protocol
//--------------------------------------------------------------
class CTransceiver : public ITransceiver
{  
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CTransceiver(boost::shared_ptr<IPort> port);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CTransceiver();

   // ITransceiver implementation
   virtual void reset();
   // [END] ITransceiver implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Send a command message to the transceiver
   /// \param [in] command          Command type
   //--------------------------------------------------------------
   void sendCommand(unsigned char command);

   //--------------------------------------------------------------
   /// \brief	                     Adapt a buffer to be loggable
   /// \param [in] ptr              Buffer pointer
   /// \param [in] sier             Buffer size
   //--------------------------------------------------------------
   std::string CTransceiver::msgToString(const void* ptr, size_t size) const;

private:
   //--------------------------------------------------------------
   /// \brief  The communication port
   //--------------------------------------------------------------
   boost::shared_ptr<IPort> m_port;

   //--------------------------------------------------------------
   /// \brief  The message sequence number
   //--------------------------------------------------------------
   unsigned char m_seqNumber;

   //--------------------------------------------------------------
   /// \brief  The message structure (keeped here for better performance)
   //--------------------------------------------------------------
   RBUF m_request;
};



