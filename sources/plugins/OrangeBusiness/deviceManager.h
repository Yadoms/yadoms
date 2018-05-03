#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include "IEquipment.h"
#include "IurlManager.h"
#include "Decoder.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//-----------------------------------------------------
///\brief WES equipment
//-----------------------------------------------------
class CEquipmentManager
{
public:

   //TODO : Apply all comments

   //-----------------------------------------------------
   ///\brief                         Constructor from restart (devices and keywords already registered)
   ///\param[in]   api               Yadoms API
   //-----------------------------------------------------
   explicit CEquipmentManager(boost::shared_ptr<yApi::IYPluginApi> api);
   explicit CEquipmentManager(const std::map<std::string, boost::shared_ptr<equipments::IEquipment>>& deviceList);
   void refreshEquipments(boost::shared_ptr<yApi::IYPluginApi> api,
                          boost::shared_ptr<IurlManager> frameManager,
                          const std::string& apikey,
                          boost::shared_ptr<CDecoder> decoder);

   void refreshEquipment(boost::shared_ptr<yApi::IYPluginApi> api,
                         const boost::shared_ptr<equipments::IEquipment> equipment,
                         boost::shared_ptr<IurlManager> frameManager,
                         const std::string& apikey,
                         boost::shared_ptr<CDecoder> decoder);

   void removeAllDevices(boost::shared_ptr<yApi::IYPluginApi> api);

   const int size() const;

   //-----------------------------------------------------
   ///\brief                     Destructor
   //-----------------------------------------------------
   virtual ~CEquipmentManager();

private:

   std::map<std::string, boost::shared_ptr<equipments::IEquipment>> m_deviceList;
};
