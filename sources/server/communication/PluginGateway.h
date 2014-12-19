#pragma once
#include "database/IDataProvider.h"
#include "pluginSystem/Manager.h"
#include "ISendMessageAsync.h"
#include "dataAccessLayer/IAcquisitionHistorizer.h"
#include "callback/ISynchronousCallback.h"

namespace communication {


   //----------------------------------------------
   ///\brief Class used to communicate with plugins
   //----------------------------------------------
   class CPluginGateway : public ISendMessageAsync
   {
   public:
      //----------------------------------------------
      ///\brief                     Constructor
      ///\param[in] dataProvider    The main data provider (access to database)
      ///\param[in] pluginManager   The plugin manager
      //----------------------------------------------
      CPluginGateway(boost::shared_ptr<database::IDataProvider> dataProvider, boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer, boost::shared_ptr<pluginSystem::CManager> pluginManager);

      //----------------------------------------------
      ///\brief Destructor
      //----------------------------------------------
      virtual ~CPluginGateway();

   public:
      // ISendMessageAsync Implementation
      virtual void sendCommandAsync(int keywordId, const std::string & body);
      virtual void sendManuallyDeviceCreationRequest(int pluginId, const shared::plugin::yPluginApi::IManuallyDeviceCreationData & data, communication::callback::ISynchronousCallback<std::string> & callback);
      virtual void sendBindingQueryRequest(int pluginId, const shared::plugin::yPluginApi::IBindingQueryData & data, communication::callback::ISynchronousCallback< shared::CDataContainer > & callback);
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
