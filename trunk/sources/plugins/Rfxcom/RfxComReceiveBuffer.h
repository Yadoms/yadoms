#pragma once
#include "Buffer.hpp"
//TODO fichiers (.h/.cpp) à virer
//--------------------------------------------------------------
/// \brief	This class manage the receive buffer from rfxcom
// Its objective is to agregate cut messages
//--------------------------------------------------------------
class CRfxcommReceiveBuffer
{
public:
   //--------------------------------------------------------------
   /// \brief	                     Constructor
   //--------------------------------------------------------------
   CRfxcommReceiveBuffer();

   //--------------------------------------------------------------
   /// \brief	                     Destructor
   //--------------------------------------------------------------
   virtual ~CRfxcommReceiveBuffer();

   //--------------------------------------------------------------
   /// \brief	                     Clear the receive buffer content
   //--------------------------------------------------------------
   void flush();

   //--------------------------------------------------------------
   /// \brief	                     Reset the receive buffer content
   /// \return                      true if a message is complete
   //--------------------------------------------------------------
   bool isComplete() const;

   //--------------------------------------------------------------
   /// \brief	                     Check if buffer is empty
   /// \return                      true if empty
   //--------------------------------------------------------------
   bool isEmpty() const;

   //--------------------------------------------------------------
   /// \brief	                     Append data into buffer
   /// \param[in] toAppend          Data to append to buffer
   //--------------------------------------------------------------
   void append(const CByteBuffer& toAppend);

   //--------------------------------------------------------------
   /// \brief	                     Get the next message in the receive buffer
   /// \return                      The next complete message
   //--------------------------------------------------------------
   boost::shared_ptr<CByteBuffer> popNextMessage();

private:
   //--------------------------------------------------------------
   /// \brief	Buffer content
   //--------------------------------------------------------------
   std::vector<unsigned char> m_content;

};


