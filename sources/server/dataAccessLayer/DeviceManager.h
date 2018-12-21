#pragma once
#include "IDeviceManager.h"
#include "database/IDeviceRequester.h"
#include "database/IKeywordRequester.h"
#include "database/IAcquisitionRequester.h"
#include "IKeywordManager.h"

namespace dataAccessLayer
{
   class CDeviceManager : public IDeviceManager
   {
   public:
      //--------------------------------------------------------------
      /// \brief                          Constructor
      /// \param [in] deviceRequester     The device requester
      /// \param [in] keywordRequester    The keyword requester
      /// \param [in] acquisitionRequester The acquisition requester
      //--------------------------------------------------------------
      CDeviceManager(boost::shared_ptr<database::IDeviceRequester> deviceRequester,
                     boost::shared_ptr<database::IKeywordRequester> keywordRequester,
                     boost::shared_ptr<database::IAcquisitionRequester> acquisitionRequester,
                     boost::shared_ptr<IKeywordManager> keywordManager);

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~CDeviceManager();

      // IDeviceManager Implementation
      bool deviceExists(int deviceId) const override;
      bool deviceExists(int pluginId, const std::string& deviceName) const override;
      boost::shared_ptr<database::entities::CDevice> getDevice(int deviceId) const override;
      boost::shared_ptr<database::entities::CDevice> getDeviceInPlugin(int pluginId, const std::string& name, bool includeBlacklistDevice) const override;
      std::vector<boost::shared_ptr<database::entities::CDevice>> getDeviceWithCapacity(const std::string& capacityName,
                                                                                        const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode) const override;
      std::vector<boost::shared_ptr<database::entities::CDevice>> getDeviceWithCapacityType(const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode,
                                                                                            const shared::plugin::yPluginApi::EKeywordDataType& capacityType) const override;
      boost::shared_ptr<database::entities::CDevice> createDevice(int pluginId,
                                                                  const std::string& name,
                                                                  const std::string& friendlyName,
                                                                  const std::string& type,
                                                                  const std::string& model,
                                                                  const shared::CDataContainer& details) override;
      std::vector<boost::shared_ptr<database::entities::CDevice>> getDevices() const override;
      std::vector<std::string> getDevicesForPluginInstance(int pluginId) const override;
      void updateDeviceFriendlyName(int deviceId, const std::string& newFriendlyName) override;
      void updateDeviceConfiguration(int deviceId, const shared::CDataContainer& configuration) override;
      void updateDeviceDetails(int deviceId, const shared::CDataContainer& details) override;
      void updateDeviceModel(int deviceId, const std::string& model) override;
      void updateDeviceType(int deviceId, const std::string& type) override;
      void updateDeviceBlacklistState(int deviceId, bool blacklist) override;
      void updateDeviceState(int deviceId, const shared::plugin::yPluginApi::historization::EDeviceState& state, const std::string& customMessageId, const shared::CDataContainer &data) const override;
      void removeDevice(int deviceId) override;
      void removeDevice(int pluginId, const std::string& deviceName) override;
      void removeAllDeviceForPlugin(int pluginId) override;
      void cleanupDevice(int deviceId) override;
      // [END] IDeviceManager Implementation

   protected:
      //--------------------------------------------------------------
      /// \brief           The real data requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IDeviceRequester> m_deviceRequester;
      boost::shared_ptr<database::IKeywordRequester> m_keywordRequester;
      boost::shared_ptr<database::IAcquisitionRequester> m_acquisitionRequester;
      boost::shared_ptr<IKeywordManager> m_keywordManager;
   };
} //namespace dataAccessLayer 


