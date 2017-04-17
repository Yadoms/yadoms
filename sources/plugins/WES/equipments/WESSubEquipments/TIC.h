#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace equipments
{
   //-----------------------------------------------------
   ///\brief WES equipment
   //-----------------------------------------------------
   class CTIC
   {
   public:
      //-----------------------------------------------------
      ///\brief                     Constructor
      ///\param[in]   api          Yadoms API
      ///\param[in] device         The device name
      //-----------------------------------------------------
      CTIC(boost::shared_ptr<yApi::IYPluginApi> api,
                    const std::string& name,
                    const shared::CDataContainer& deviceConfiguration);

      std::string getDeviceName() const;
      std::string getDeviceType() const;
      void updateFromDevice( boost::shared_ptr<yApi::IYPluginApi> api);

      //-----------------------------------------------------
      ///\brief                     Destructor
      //-----------------------------------------------------
      virtual ~CTIC();

   private:

      //-----------------------------------------------------
      ///\brief                     The device name
      //-----------------------------------------------------
      std::string m_deviceName;

      //--------------------------------------------------------------
      /// \brief  counter TIC 1
      //--------------------------------------------------------------
      std::vector<boost::shared_ptr<yApi::historization::CEnergy> > m_counterTICList;

   };
} // namespace equipments