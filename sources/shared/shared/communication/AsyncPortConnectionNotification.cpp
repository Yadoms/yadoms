#include "stdafx.h"
#include "AsyncPortConnectionNotification.h"

namespace shared
{
   namespace communication
   {
      CAsyncPortConnectionNotification::CAsyncPortConnectionNotification()
         :m_connected(true), m_i18nErrorMessage("")
      {
      }

      CAsyncPortConnectionNotification::CAsyncPortConnectionNotification(const std::string & i18nErrorMessage)
         :m_connected(false), m_i18nErrorMessage(i18nErrorMessage)
      {
      }
      
      CAsyncPortConnectionNotification::CAsyncPortConnectionNotification(const std::string & i18nErrorMessage, const std::map<std::string, std::string> & innerErrorMessage)
         :m_connected(false), m_i18nErrorMessage(i18nErrorMessage), m_i18nMessageParameters(innerErrorMessage)
      {
      }
      
      CAsyncPortConnectionNotification::~CAsyncPortConnectionNotification()
      {
      }
      
      bool CAsyncPortConnectionNotification::isConnected() const
      {
         return m_connected;
      }
      
      const std::string & CAsyncPortConnectionNotification::getErrorMessageI18n() const
      {
         return m_i18nErrorMessage;
      }
      
      const std::map<std::string, std::string> & CAsyncPortConnectionNotification::getErrorMessageI18nParameters() const
      {
         return m_i18nMessageParameters;
      }

   }
} // namespace shared::communication


