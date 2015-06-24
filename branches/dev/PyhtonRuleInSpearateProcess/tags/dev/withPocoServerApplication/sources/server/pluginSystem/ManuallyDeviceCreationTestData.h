#pragma once
#include <shared/plugin/yPluginApi/IManuallyDeviceCreationTestData.h>


namespace pluginSystem
{
   //-----------------------------------------------------
   ///\brief The IDeviceCommand implementation
   //-----------------------------------------------------
   class CManuallyDeviceCreationTestData : public shared::plugin::yPluginApi::IManuallyDeviceCreationTestData
   {
   public:
      //-----------------------------------------------------
      ///\brief                  Constructor
      ///\param[in] device       The command to send
      ///\param[in] parameters   The parameters of the device (JSON string)
      //-----------------------------------------------------
      CManuallyDeviceCreationTestData(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command, const std::string& parameters);

      //-----------------------------------------------------
      ///\brief               Destructor
      //-----------------------------------------------------
      virtual ~CManuallyDeviceCreationTestData();

      // IManuallyDeviceCreationTestData implementation
      virtual boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> getCommand() const;
      virtual const std::string& getDeviceParameters() const;
      virtual const std::string toString() const;
      // [END] IManuallyDeviceCreationTestData implementation

   private:
      //-----------------------------------------------------
      ///\brief               Command
      //-----------------------------------------------------
      boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> m_command;

      //-----------------------------------------------------
      ///\brief               Device parameters (JSON string)
      //-----------------------------------------------------
      const std::string m_parameters;
   };  
} // namespace pluginSystem	
	
	