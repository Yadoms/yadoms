#pragma once
#include "database/IDataProvider.h"
#include "pluginSystem/Manager.h"
#include "ISendMessageAsync.h"
#include "dataAccessLayer/IAcquisitionHistorizer.h"
#include "callback/ISynchronousCallback.h"

namespace communication
{
   //----------------------------------------------
   ///\brief Class used to communicate with plugins
   //----------------------------------------------
   class CPluginGateway final : public ISendMessageAsync
   {
   public:
      //----------------------------------------------
      ///\brief                     Constructor
      ///\param[in] dataProvider    The main data provider (access to database)
      ///\param[in] acquisitionHistorizer   The acquisition historizer
      ///\param[in] pluginManager   The plugin manager
      //----------------------------------------------
      CPluginGateway(boost::shared_ptr<database::IDataProvider> dataProvider,
                     boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer,
                     boost::shared_ptr<pluginSystem::CManager> pluginManager);

      CPluginGateway(const CPluginGateway&) = delete;
      CPluginGateway(CPluginGateway&&) = delete;
      CPluginGateway& operator=(const CPluginGateway&) = delete;
      CPluginGateway& operator =(CPluginGateway&&) = delete;

      ~CPluginGateway() override = default;

      // ISendMessageAsync Implementation
      void sendKeywordCommandAsync(int keywordId,
                                   const std::string& body) override;
      std::string sendExtraQueryAsync(int pluginInstanceId,
                                      boost::shared_ptr<shared::plugin::yPluginApi::IExtraQueryData> data) override;
      void sendManuallyDeviceCreationRequest(int pluginInstanceId,
                                             const shared::plugin::yPluginApi::IManuallyDeviceCreationData& data,
                                             callback::ISynchronousCallback<std::string>& callback) override;
      void sendBindingQueryRequest(int pluginInstanceId,
                                   const shared::plugin::yPluginApi::IBindingQueryData& data,
                                   callback::ISynchronousCallback<boost::shared_ptr<shared::CDataContainer>>& callback) override;
      void sendDeviceConfigurationSchemaRequest(int deviceId,
                                                callback::ISynchronousCallback<boost::shared_ptr<shared::CDataContainer>>& callback) override;
      void sendSetDeviceConfiguration(int deviceId,
                                      const boost::shared_ptr<shared::CDataContainer>& configuration) override;
      // [END] ISendMessageAsync Implementation

   private:
      boost::shared_ptr<database::IDataProvider> _dataProvider;
      boost::shared_ptr<pluginSystem::CManager> _pluginManager;
      boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> _acquisitionHistorizer;
   };
}
