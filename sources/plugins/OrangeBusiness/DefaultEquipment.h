#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IEquipment.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace equipments
{
   //-----------------------------------------------------
   ///\brief WES equipment
   //-----------------------------------------------------
   class CDefaultEquipment : public IEquipment
   {
   public:

      //-----------------------------------------------------
      ///\brief                          Constructor from restart (devices and keywords already registered)
      ///\param[in]   api               Yadoms API
      ///\param[in] device              The device name
      ///\param[in] deviceConfiguration The device configuration
      //-----------------------------------------------------
      CDefaultEquipment(const std::string& name,
                        const std::string& devEUID);

      // IEquipment implementation
      std::string getName() const override;
      std::string getEUI() const override;
      void createDevice(boost::shared_ptr<yApi::IYPluginApi> api) override;
      void updateData(boost::shared_ptr<yApi::IYPluginApi> api,
                      std::string& data) override;
	  void updateBatteryLevel(boost::shared_ptr<yApi::IYPluginApi> api,
					  int batteryLevel) override;
      // [END] IEquipment implementation

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CDefaultEquipment();

   private:

      //--------------------------------------------------------------
      /// \brief	    Message historization object
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CText> m_messageKeyword;

      //--------------------------------------------------------------
      /// \brief	The battery level (percent)
      //--------------------------------------------------------------
      boost::shared_ptr<yApi::historization::CBatteryLevel> m_batteryLevel;

      std::string m_name;
      std::string m_devEUI;
   };
} // namespace equipments