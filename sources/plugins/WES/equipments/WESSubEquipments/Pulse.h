#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

namespace equipments
{
   namespace subdevices
   {
      //-----------------------------------------------------
      ///\brief list of units available
      //-----------------------------------------------------
      typedef enum {
         undefined,
         litre,
         m3,
         Wh,
         KWh
      } EUnit;

      //-----------------------------------------------------
      ///\brief WES equipment
      //-----------------------------------------------------
      class CPulse
      {
      public:
         //-----------------------------------------------------
         ///\brief                                      Constructor
         ///\param[in] api                             Yadoms API
         ///\param[in] keywordsToDeclare               list of keywords to declare
         ///\param[in] deviceName                      The device name
         ///\param[in] keywordName                     The keyword name
         ///\param[in] unitName                        The unit used by the WES
         //-----------------------------------------------------
         CPulse(boost::shared_ptr<yApi::IYPluginApi> api,
                std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToDeclare,
                const std::string& deviceName,
                const std::string& keywordName,
                const EUnit unitName);

         //-----------------------------------------------------
         ///\brief                                      updateFromDevice
         ///\param[in] api                             Yadoms API
         ///\param[in] keywordsToHistorize             list of keywords to historize
         ///\param[in] unitName                        The unit used by the WES
         ///\param[in] totalValue                      The total value sent by the WES
         //-----------------------------------------------------
         void updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api,
                               std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> >& keywordsToHistorize,
                               const EUnit unitName,
                               const double& totalValue
         );

         //-----------------------------------------------------
         ///\brief                                      updateConfiguration
         ///\param[in] api                             Yadoms API
         ///\param[in] unitName                        the unit used
         //-----------------------------------------------------
         void updateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                  const EUnit unitName);

         //-----------------------------------------------------
         ///\brief                     get the device name
         ///\return                    the pulse keyword Name
         //-----------------------------------------------------
         std::string name();

         //-----------------------------------------------------
         ///\brief                     Destructor
         //-----------------------------------------------------
         virtual ~CPulse();

      private:

         //-----------------------------------------------------
         ///\brief                     The device name
         //-----------------------------------------------------
         std::string m_deviceName;

         //-----------------------------------------------------
         ///\brief                     The keyword name
         //-----------------------------------------------------
         std::string m_keywordName;

         //-----------------------------------------------------
         ///\brief                     The contract name
         //-----------------------------------------------------
         EUnit m_unitName;

         //--------------------------------------------------------------
         /// \brief  Main keywords
         //--------------------------------------------------------------
         boost::shared_ptr<yApi::historization::CEnergy> m_pulseEnergy;
         boost::shared_ptr<yApi::historization::CVolume> m_pulseVolume;
      };
   }
} // namespace equipments::subdevices