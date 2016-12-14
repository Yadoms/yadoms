#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "../IWUConfiguration.h"
#include "../IdeviceConfiguration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Interface of device
//--------------------------------------------------------------
namespace modules
{
   class IModule
   {
   public:

      //--------------------------------------------------------------
      /// \brief	  Parse the answer from the web Site
      /// \param[in] api             pointer to the API
      /// \param[in] dataToParse     data to parse
      //--------------------------------------------------------------
      virtual void parse(boost::shared_ptr<yApi::IYPluginApi> api,
                         const shared::CDataContainer dataToParse) = 0;

      //--------------------------------------------------------------
      /// \brief	  Update the configuration when something change from the HMI
      /// \param[in] api                  pointer to the API
      /// \param[in] wuConfiguration      The Plugin configuration
      //--------------------------------------------------------------
      virtual void onPluginUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                  IWUConfiguration& wuConfiguration) = 0;

      //--------------------------------------------------------------
      /// \brief	  Update the configuration when something change from the HMI
      /// \param[in] api                  pointer to the API
      /// \param[in] deviceconfiguration  The Configuration of the device
      //--------------------------------------------------------------
      virtual void onDeviceUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                                  boost::shared_ptr<IdeviceConfiguration> deviceConfiguration) = 0;

      //--------------------------------------------------------------
      /// \brief	  Return the url
      /// \return    The url string
      //--------------------------------------------------------------
      virtual std::string getUrl() const = 0;

      //--------------------------------------------------------------
      /// \brief	  Return the name of the device
      /// \return    The url string
      //--------------------------------------------------------------
      virtual std::string getName() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IModule()
      {}

   };
}// namespace modules