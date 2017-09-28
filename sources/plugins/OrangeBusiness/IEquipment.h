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
      virtual void createDevice(boost::shared_ptr<yApi::IYPluginApi> api) = 0;

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IEquipment()
      {}

   };
}// namespace equipments