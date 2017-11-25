#pragma once
#include <shared/DataContainer.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Interface of device
//--------------------------------------------------------------
namespace equipments
{
   class IEquipment
   {
   public:

      //--------------------------------------------------------------
      /// \brief      getName
      /// \return  the device name
      //--------------------------------------------------------------
      virtual std::string getName() const = 0;

      //--------------------------------------------------------------
      /// \brief      getEUI
      /// \return  the device unique ID
      //--------------------------------------------------------------
      virtual std::string getEUI() const = 0;

      //--------------------------------------------------------------
      /// \brief	                     Create the Device with the counter Id
      //-----------------------------------------------------
      virtual void updateData(boost::shared_ptr<yApi::IYPluginApi> api,
                              const std::string& data) = 0;

      //--------------------------------------------------------------
      /// \brief	                     Update in the database for this equipment the last message id
      //-----------------------------------------------------
      virtual void updatelastMessageId(boost::shared_ptr<yApi::IYPluginApi> api,
                                       const std::string& id) = 0;

      //--------------------------------------------------------------
      /// \brief	                     Get last message id
      //-----------------------------------------------------
      virtual std::string getlastMessageId(boost::shared_ptr<yApi::IYPluginApi> api) = 0;

	  //--------------------------------------------------------------
	  /// \brief	                     Create the Device with the counter Id
	  //-----------------------------------------------------
	  virtual void updateBatteryLevel(boost::shared_ptr<yApi::IYPluginApi> api,
							  int batteryLevel) = 0;

     //--------------------------------------------------------------
     /// \brief	                     Remove this device from Yadoms
     //-----------------------------------------------------
     virtual void removeFromYadoms(boost::shared_ptr<yApi::IYPluginApi> api) = 0;

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IEquipment()
      {}

   };
}// namespace equipments