#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace equipments
{
   namespace subdevices
   {
      //-----------------------------------------------------
      ///\brief WES equipment
      //-----------------------------------------------------
      class CPulse
      {
      public:
         //-----------------------------------------------------
         ///\brief                     Constructor
         ///\param[in]   api          Yadoms API
         ///\param[in] device         The device name
         //-----------------------------------------------------
         CPulse(boost::shared_ptr<yApi::IYPluginApi> api,
                std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                const std::string& deviceName,
                const std::string& keywordName,
                const std::string& unitName);

         std::string getDeviceName() const;
         void updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                               std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize,
                               const std::string& unitName,
                               const double& flowValue,
                               const std::string& totalValue
         );

         //-----------------------------------------------------
         ///\brief                     Destructor
         //-----------------------------------------------------
         virtual ~CPulse();

      private:

         void initializePulse(boost::shared_ptr<yApi::IYPluginApi> api, 
                              std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                              const std::string& keywordName);

         //-----------------------------------------------------
         ///\brief                     The device name
         //-----------------------------------------------------
         std::string m_deviceName;

         //-----------------------------------------------------
         ///\brief                     The contract name
         //-----------------------------------------------------
         std::string m_unitName;

         //--------------------------------------------------------------
         /// \brief  Keywords
         //--------------------------------------------------------------
         boost::shared_ptr<yApi::historization::CEnergy> m_pulseEnergy;
         boost::shared_ptr<yApi::historization::CVolume> m_pulseVolume;

      };
   }
} // namespace equipments::subdevices