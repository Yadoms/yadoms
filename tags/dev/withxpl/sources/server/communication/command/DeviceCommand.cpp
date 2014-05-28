﻿#include "stdafx.h"
#include "DeviceCommand.h"


namespace communication { namespace command {


   //----------------------------------------------
   ///\brief Constructor
   //----------------------------------------------
   CDeviceCommand::CDeviceCommand(int deviceId, CommandData data, boost::shared_ptr<CCallback> callBack)
      :m_deviceId(deviceId), m_commandData(data), m_callBack(callBack)
   {
   }

   //----------------------------------------------
   ///\brief Destructor
   //----------------------------------------------
   CDeviceCommand::~CDeviceCommand()
   {
   }

   //----------------------------------------------
   ///\brief Get the device id
   //----------------------------------------------
   int CDeviceCommand::getDeviceId()
   {
      return m_deviceId;
   }

   //----------------------------------------------
   ///\brief Get the command data
   //----------------------------------------------
   CDeviceCommand::CommandData & CDeviceCommand::getCommandData()
   {
      return m_commandData;
   }

   //--------------------------------------------------------------
   /// \brief	    Get the callback (may be null if callback is not used)
   /// \return    the callback
   //--------------------------------------------------------------
   boost::shared_ptr<CCallback> & CDeviceCommand::getCallback()
   {
      return m_callBack;
   }

   //--------------------------------------------------------------
   /// \brief	    Gets an human readable string of this instance
   /// \return     A human readable string of this instance
   //--------------------------------------------------------------
   std::string CDeviceCommand::toString() const
   {
      std::string fullCommand;
      fullCommand = (boost::format("Device Id = %1%") % m_deviceId).str();
      fullCommand += "Data : ";

      CommandData::const_iterator i;
      for(i= m_commandData.begin(); i!=m_commandData.end(); ++i)
      {
         fullCommand += (boost::format("Key %1% ; Value %2%") % i->first % i->second).str();
      }
      return fullCommand;

   }
   
} //namespace command
} //namespace communication
