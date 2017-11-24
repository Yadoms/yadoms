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
      ///\param[in] name                 The device name
      ///\param[in] devEUID              The EUID of the equipment
      //-----------------------------------------------------
      CDefaultEquipment(const std::string& name,
                        const std::string& devEUID);


      //-----------------------------------------------------
      ///\brief                          Constructor with creation (devices and keywords do not exist)
      ///\param[in] name                 The device name
      ///\param[in] devEUID              The EUID of the equipment
      ///\param[in] api                  Yadoms API
      //-----------------------------------------------------
      CDefaultEquipment::CDefaultEquipment(const std::string& name,
                                           const std::string& devEUID,
                                           boost::shared_ptr<yApi::IYPluginApi> api);

      // IEquipment implementation
      std::string getName() const override;
      std::string getEUI() const override;
      void updateData(boost::shared_ptr<yApi::IYPluginApi> api,
                      const std::string& data) override;
	  void updateBatteryLevel(boost::shared_ptr<yApi::IYPluginApi> api,
					  int batteryLevel) override;
     void updatelastMessageId(boost::shared_ptr<yApi::IYPluginApi> api,
                              const std::string& id) override;
     std::string getlastMessageId(boost::shared_ptr<yApi::IYPluginApi> api) override;
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
      std::string m_lastMessageId;
   };
} // namespace equipments