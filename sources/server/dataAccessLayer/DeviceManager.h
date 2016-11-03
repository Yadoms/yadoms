#pragma once
#include "IDeviceManager.h"
#include "database/IDeviceRequester.h"

namespace dataAccessLayer
{
   class CDeviceManager : public IDeviceManager
   {
   public:
      //--------------------------------------------------------------
      /// \brief                          Constructor
      /// \param [in] deviceRequester     The device requester
      //--------------------------------------------------------------
      explicit CDeviceManager(boost::shared_ptr<database::IDeviceRequester> deviceRequester);

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~CDeviceManager();

      // IDeviceManager Implementation
      bool deviceExists(const int deviceId) const override;
      bool deviceExists(const int pluginId,
                        const std::string& deviceName) const override;
      boost::shared_ptr<database::entities::CDevice> getDevice(int deviceId) const override;
      boost::shared_ptr<database::entities::CDevice> getDevice(const int pluginId,
                                                               const std::string& name) const override;
      std::vector<boost::shared_ptr<database::entities::CDevice>> getDeviceWithCapacity(const std::string& capacityName,
                                                                                        const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode) const override;
      std::vector<boost::shared_ptr<database::entities::CDevice>> getDeviceWithCapacityType(const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode,
                                                                                            const shared::plugin::yPluginApi::EKeywordDataType& capacityType) const override;
      boost::shared_ptr<database::entities::CDevice> createDevice(int pluginId,
                                                                  const std::string& name,
                                                                  const std::string& friendlyName,
                                                                  const std::string& model,
                                                                  const shared::CDataContainer& details) override;
      std::vector<boost::shared_ptr<database::entities::CDevice>> getDevices() const override;
      void updateDeviceFriendlyName(int deviceId,
                                    const std::string& newFriendlyName) override;
      void removeDevice(int deviceId) override;
      void removeDevice(int pluginId,
                        const std::string& deviceName) override;
      void removeAllDeviceForPlugin(int pluginId) override;
      // [END] IDeviceManager Implementation

   protected:
      //--------------------------------------------------------------
      /// \brief           The real data requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IDeviceRequester> m_deviceRequester;
   };
} //namespace dataAccessLayer 


