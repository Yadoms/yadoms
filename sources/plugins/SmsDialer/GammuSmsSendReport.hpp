#pragma once

//--------------------------------------------------------------
/// \brief	Gammu SMS send report
//--------------------------------------------------------------
class CGammuSmsSendReport //TODO encore utile ?
{
public:
   //--------------------------------------------------------------
   /// \brief	                        Constructor
   /// \param[in]  status              Status code (0 if OK, other values can be phone-specific)
   /// \param[in]  messageReference    Id referencing the message sent
   //--------------------------------------------------------------
   CGammuSmsSendReport(int status, int messageReference)
      :m_status(status), m_messageReference(messageReference)
   {
   }

   //--------------------------------------------------------------
   /// \brief      Destructor
   //--------------------------------------------------------------
   virtual ~CGammuSmsSendReport()
   {
   }

   //--------------------------------------------------------------
   /// \brief  Message sent status getter
   /// \return The message sent status code
   //--------------------------------------------------------------
   int getStatus() const { return m_status; }

   //--------------------------------------------------------------
   /// \brief  Message sent Id getter
   /// \return The Id of the message sent
   //--------------------------------------------------------------
   int getMessageReference() const { return m_messageReference; }

private:
   //--------------------------------------------------------------
   /// \brief  The status of message sent (0 if OK, other values can be phone-specific)
   //--------------------------------------------------------------
   int m_status;

   //--------------------------------------------------------------
   /// \brief  The Id referencing the message sent
   //--------------------------------------------------------------
   int m_messageReference;
};