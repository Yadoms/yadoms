#pragma once


namespace communication {

   //----------------------------------------------
   ///\brief Interface used to send a command
   //----------------------------------------------
   class ISendMessageAsync
   {
   public:
      //----------------------------------------------
      ///\brief Destructor
      //----------------------------------------------
      virtual ~ISendMessageAsync()
      {
      }

      //----------------------------------------------
      ///\brief                     Send a command
      ///\param [in] deviceId       Target device Id
      ///\param [in] keywordId      Target keyword Id
      ///\param [in] body           The command body to send
      //----------------------------------------------
      virtual void sendCommandAsync(int deviceId, int keywordId, const std::string& body) = 0;

      //----------------------------------------------
      ///\brief                     Send a manually device creation request to a plugin
      ///\param [in] pluginId       Plugin Id on which to create device
      ///\param [in] deviceName     Device name
      ///\param [in] keywordName    Keyword name
      ///\param [in] capacity       The capacity used for this device
      ///\param [in] parameters     Device parameters
      //----------------------------------------------
      virtual void sendManuallyDeviceCreationRequestAsync(int pluginId, const std::string& deviceName, const std::string& keywordName, const std::string& capacity, const std::string& parameters) = 0;
   };

} //namespace communication