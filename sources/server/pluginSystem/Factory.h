#pragma once
#include "IFactory.h"

namespace pluginSystem
{

   //--------------------------------------------------------------
   /// \brief	IFactory implementation
   //--------------------------------------------------------------
   class CFactory : public IFactory, public boost::enable_shared_from_this<IFactory>
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      //--------------------------------------------------------------
      CFactory();

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CFactory();

      // IFactory Implementation
      boost::shared_ptr<const shared::plugin::information::IInformation> createInformation(const boost::filesystem::path& pluginPath) const override;
      boost::shared_ptr<IInstance> createInstance(
         const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
         boost::shared_ptr<const database::entities::CPlugin> pluginData,
         boost::shared_ptr<database::IDataProvider> dataProvider,
         boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
         boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer,
         const boost::shared_ptr<IQualifier> qualifier,
         boost::shared_ptr<shared::event::CEventHandler> supervisor,
         int pluginManagerEventId) const override;
      boost::shared_ptr<IInstanceStateHandler> createInstanceStateHandler(
         boost::shared_ptr<database::IPluginRequester> dbRequester,
         boost::shared_ptr<dataAccessLayer::IEventLogger> eventLogger,
         boost::shared_ptr<shared::event::CEventHandler> managerEventHandler,
         int instanceId) const override;
      // [END] IFactory Implementation
   };

} // namespace pluginSystem
