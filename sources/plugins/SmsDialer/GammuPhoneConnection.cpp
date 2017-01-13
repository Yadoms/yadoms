#include "stdafx.h"
#include <shared/Log.h>
#include "PhoneException.hpp"
#include "GammuPhoneConnection.h"


CGammuPhoneConnection::CGammuPhoneConnection(const ISmsDialerConfiguration& configuration)
   : m_configuration(configuration)
{
   GSM_InitLocales(nullptr);

   // Initialize Gammu context
   m_gsmContext = GSM_AllocStateMachine();
   if (m_gsmContext == nullptr)
      throw CPhoneException("Unable to initialize the Gammu library context");

   // Configure Gammu
   // Get pointer to configuration structure
   GSM_Config* gsmConfig = GSM_GetConfig(m_gsmContext, 0);
   // Set configuration, first freeing old values
   // Use strdup function because pointer seems to be freed by Gammu
   free(gsmConfig->Device);
   gsmConfig->Device = strdup(m_configuration.getGammuPort().c_str());
   free(gsmConfig->Connection);
   gsmConfig->Connection = strdup(m_configuration.getGammuProtocol().c_str());
   GSM_SetConfigNum(m_gsmContext, 1);

   // Try to connect
   connect();
}

CGammuPhoneConnection::~CGammuPhoneConnection()
{
   // Close connection
   disconnect();

   GSM_FreeStateMachine(m_gsmContext);
}

GSM_StateMachine* CGammuPhoneConnection::getGsmContext() const
{
   return m_gsmContext;
}

void CGammuPhoneConnection::handleGammuError(GSM_Error gsmError,
                                             const std::string& errorMessage)
{
   if (gsmError != ERR_NONE)
      throw CPhoneException(std::string("Phone connection : ") + errorMessage + std::string(" : ") + std::string(GSM_ErrorString(gsmError)));
}

bool CGammuPhoneConnection::connect() const
{
   if (isConnected())
      return true;

   disconnect();

   try
   {
      // Connect to phone
      handleGammuError(GSM_InitConnection(m_gsmContext, 1), "Unable to connect to phone");
   }
   catch (CPhoneException& e)
   {
      YADOMS_LOG(error) << e.what() ;
      disconnect();
      return false;
   }

   return isConnected();
}

void CGammuPhoneConnection::disconnect() const
{
   try
   {
      handleGammuError(GSM_TerminateConnection(m_gsmContext), "Unable to close connection");
   }
   catch (CPhoneException& e)
   {
      YADOMS_LOG(error) << e.what() ;
   }
}

bool CGammuPhoneConnection::isConnected() const
{
   // Check if serial port is open, and if get the phone manufacturer (to check if a phone is really connected)
   if (GSM_IsConnected(m_gsmContext) != TRUE)
      return false;

   return GSM_GetManufacturer(m_gsmContext, NULL) == ERR_NONE;
}

