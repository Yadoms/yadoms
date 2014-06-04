#pragma once
#include <shared/event/EventHandler.hpp>
#include "Callback.h"

namespace communication { namespace command {

   //----------------------------------------------
   ///\brief Class container for a device command
   //----------------------------------------------
   class CDeviceCommand
   {
   public:
      //----------------------------------------------
      ///\brief Typedef for the command data
      //----------------------------------------------
      typedef std::map<std::string, std::string> CommandData;

      //----------------------------------------------
      ///\brief Constructor
      //----------------------------------------------
      CDeviceCommand(int deviceId, const CommandData& data, boost::shared_ptr<CCallback> callBack = boost::shared_ptr<CCallback>());

      //----------------------------------------------
      ///\brief Destructor
      //----------------------------------------------
      virtual ~CDeviceCommand();


      //----------------------------------------------
      ///\brief Get the device id
      //----------------------------------------------
      int getDeviceId() const;

      //----------------------------------------------
      ///\brief Get the command data
      //----------------------------------------------
      const CommandData& getCommandData() const;

      //--------------------------------------------------------------
      /// \brief	    Gets an human readable string of this instance
      /// \return     A human readable string of this instance
      //--------------------------------------------------------------
      std::string toString() const;


      //--------------------------------------------------------------
      /// \brief	    Get the callback (may be null if callback is not used)
      /// \return    the callback
      //--------------------------------------------------------------
      const boost::shared_ptr<const CCallback> & getCallback() const;

   private:

      //----------------------------------------------
      ///\brief The device id
      //----------------------------------------------
      int m_deviceId;

      //----------------------------------------------
      ///\brief The command data
      //----------------------------------------------
      const CommandData m_commandData;

      //----------------------------------------------
      ///\brief The result callback
      //----------------------------------------------
      boost::shared_ptr<const CCallback> m_callBack;
   };

} //namespace command
} //namespace communication
