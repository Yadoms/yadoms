#pragma once
#include <shared/DataContainer.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/NetworkInterface.h>
#include "../IWESConfiguration.h"

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
      /// \brief      getDeviceName
      /// \return  the device name
      //--------------------------------------------------------------
      virtual std::string getDeviceName() const = 0;

      //--------------------------------------------------------------
      /// \brief      getDeviceType
      /// \return  the device type
      //--------------------------------------------------------------
      virtual std::string getDeviceType() const = 0;

      //--------------------------------------------------------------
      /// \brief      getDeviceType
      /// \return  the device type
      //--------------------------------------------------------------
      virtual bool isMasterDevice() const = 0;

      //--------------------------------------------------------------
      /// \brief      updateFromDevice
      ///\param[in]   api                     Yadoms API
      ///\param[in]   values                  New Values to enter
      ///\param[in]   forceHistorization      force historization for all values (initialization of the device, reset of the device, ...)
      //--------------------------------------------------------------
      virtual void updateFromDevice(boost::shared_ptr<yApi::IYPluginApi> api, 
                                    const boost::shared_ptr<IWESConfiguration> pluginConfiguration,
                                    bool forceHistorization) = 0;

      //--------------------------------------------------------------
      /// \brief      updateConfiguration    build the message to send to the WES
      ///\param[in]   api                     Yadoms API
      ///\param[in] parameters                parameters that have to be included in addition of elements processed into this function
      ///\param[in] command                   the command received from the Yadoms web client
      //--------------------------------------------------------------
      virtual void updateConfiguration(boost::shared_ptr<yApi::IYPluginApi> api,
                                       shared::CDataContainer& newConfiguration) = 0;

      //--------------------------------------------------------------
      /// \brief      updateConfiguration    build the message to send to the WES
      ///\param[in]   api                     Yadoms API
      ///\param[in] parameters                parameters that have to be included in addition of elements processed into this function
      ///\param[in] command                   the command received from the Yadoms web client
      //--------------------------------------------------------------
      virtual void sendCommand(boost::shared_ptr<yApi::IYPluginApi> api,
                               std::string& keyword,
                               std::string& command) = 0;

      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IEquipment()
      {}

   };
}// namespace equipments