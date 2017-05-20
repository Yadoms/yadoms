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
   class CPluginGateway : public ISendMessageAsync
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

      //----------------------------------------------
      ///\brief Destructor
      //----------------------------------------------
      virtual ~CPluginGateway();

      // ISendMessageAsync Implementation
      void sendKeywordCommandAsync(int keywordId, const std::string& body) override;
      void sendDeviceCommandAsync(int deviceId, const std::string& body) override;
      void sendExtraQueryAsync(int pluginId, const shared::plugin::yPluginApi::IExtraQueryData& data, communication::callback::ISynchronousCallback<shared::CDataContainer>& callback) override;
      void sendManuallyDeviceCreationRequest(int pluginId, const shared::plugin::yPluginApi::IManuallyDeviceCreationData& data, communication::callback::ISynchronousCallback<std::string>& callback) override;
      void sendBindingQueryRequest(int pluginId, const shared::plugin::yPluginApi::IBindingQueryData& data, communication::callback::ISynchronousCallback<shared::CDataContainer>& callback) override;
      void sendDeviceConfigurationSchemaRequest(int deviceId, communication::callback::ISynchronousCallback<shared::CDataContainer>& callback) override;
      void sendSetDeviceConfiguration(int deviceId, const shared::CDataContainer& configuration) override;
      // [END] ISendMessageAsync Implementation

   private:
      //----------------------------------------------
      ///\brief  The data provider
      //----------------------------------------------
      boost::shared_ptr<database::IDataProvider> m_dataProvider;

      //----------------------------------------------
      ///\brief  The plugin manager
      //----------------------------------------------
      boost::shared_ptr<pluginSystem::CManager> m_pluginManager;

      //----------------------------------------------
      ///\brief  The acquisition historizer
      //----------------------------------------------
      boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> m_acquisitionHistorizer;
   };
} //namespace communication


