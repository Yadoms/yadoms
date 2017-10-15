#pragma once
#include <shared/Export.h>

namespace shared
{
   namespace communication
   {
      //--------------------------------------------------------------
      /// \brief	A container class for IAsyncPort connection notification
      //--------------------------------------------------------------
      class YADOMS_SHARED_EXPORT CAsyncPortConnectionNotification
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor for success operationg (port connected)
         //--------------------------------------------------------------
         CAsyncPortConnectionNotification();

         //--------------------------------------------------------------
         /// \brief	Constructor for error (with i18n message)
         //--------------------------------------------------------------
         CAsyncPortConnectionNotification(const std::string & i18nErrorMessage);
         
         //--------------------------------------------------------------
         /// \brief	Constructor for error (with i18n message + i18n parameters)
         //--------------------------------------------------------------
         CAsyncPortConnectionNotification(const std::string & i18nErrorMessage, const std::map<std::string, std::string> & m_i18nMessageParameters);
         
         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CAsyncPortConnectionNotification();
         
         //--------------------------------------------------------------
         /// \brief	Obtain the connected status
         /// \return true if port is connected, other cases false
         //--------------------------------------------------------------
         bool isConnected() const;
         
         //--------------------------------------------------------------
         /// \brief	Obtain the error (i18n string)
         /// \return An internationalizable string
         //--------------------------------------------------------------
         const std::string & getErrorMessageI18n() const;
         
         //--------------------------------------------------------------
         /// \brief	Obtain the error content (inner message)
         /// \return An string
         //--------------------------------------------------------------
         const std::map<std::string, std::string> & getErrorMessageI18nParameters() const;
         
      private:
         //--------------------------------------------------------------
         /// \brief	Indicate if the serial port is connected
         //--------------------------------------------------------------
         bool m_connected;

         //--------------------------------------------------------------
         /// \brief	The error message : internationalizable string. ie: "serialport.cannotOpen"
         //--------------------------------------------------------------
         std::string m_i18nErrorMessage; //this is an internationalizable string. ie: "serialport.cannotOpen"

         //--------------------------------------------------------------
         /// \brief	The error inner message : this is a parameter, or detail of error. "COM1"
         //--------------------------------------------------------------
         std::map<std::string, std::string> m_i18nMessageParameters;
      };
   }
} // namespace shared::communication
