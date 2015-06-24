#pragma once
#include "IPortLogger.h"

//--------------------------------------------------------------
/// \brief	This class manage a communication port logger
//--------------------------------------------------------------
class CPortLogger : public IPortLogger
{  
public:
   //--------------------------------------------------------------
   /// \brief	            Constructor
   /// \param[in] prefix   String to display as prefix on each log
   //--------------------------------------------------------------
   CPortLogger(const std::string& prefix);

   //--------------------------------------------------------------
   /// \brief	            Destructor
   //--------------------------------------------------------------
   virtual ~CPortLogger();

   // IPortLogger implementation
   virtual void logReceived(const CByteBuffer& data);
   virtual void logSent(const CByteBuffer& data);
   // [END] IPortLogger implementation

protected:
   //--------------------------------------------------------------
   /// \brief	                     Adapt a buffer to be loggable
   /// \param [in] data             Buffer
   //--------------------------------------------------------------
   std::string msgToString(const CByteBuffer& data) const;

private:
   //--------------------------------------------------------------
   /// \brief	            String to display as prefix on each log
   //--------------------------------------------------------------
   const std::string m_prefix;
};
