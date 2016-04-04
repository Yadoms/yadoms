//
// Instance.h
//
// yadoms-plugin instance handler
//
#pragma once

#include <shared/event/EventHandler.hpp>
#include <shared/process/IRunner.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include "IInstance.h"
#include "ILibrary.h"
#include "database/entities/Entities.h"
#include "database/IDataProvider.h"
#include "dataAccessLayer/IAcquisitionHistorizer.h"
#include "dataAccessLayer/IDeviceManager.h"
#include "IQualifier.h"

namespace pluginSystem
{

   //--------------------------------------------------------------
   /// \brief	this class is used to manage a plugin instance. 
   //--------------------------------------------------------------
   class CInstance : public IInstance
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param [in]	pluginInformation          Information on the plugin
      /// \param [in]   pluginData                 the database entity
      /// \param [in]   dataProvider               the database accessor
      /// \param [in]   deviceManager              the device manager
      /// \param [in]   acquisitionHistorizer      the acquisition historizer
      /// \param [in]   qualifier                  the plugin qualifier
      /// \param [in]   supervisor                 the supervisor event handler
      /// \param [in]   pluginManagerEventId       The ID to use to send events to supervisor
      /// \param [in]   runner                     The instance runner
      //--------------------------------------------------------------
      CInstance(
         const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
         boost::shared_ptr<const database::entities::CPlugin> pluginData,
         boost::shared_ptr<database::IDataProvider> dataProvider,
         boost::shared_ptr<dataAccessLayer::IDeviceManager> deviceManager,
         boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> acquisitionHistorizer,
         const boost::shared_ptr<IQualifier> qualifier,
         boost::shared_ptr<shared::event::CEventHandler> supervisor,
         int pluginManagerEventId,
         boost::shared_ptr<shared::process::IRunner> runner);//TODO faire le ménage dans les paramètres

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CInstance();


      // IInstance Implementation
      void updateConfiguration(const shared::CDataContainer & newConfiguration) const override;
      void requestStop() override;
      std::string getPluginName() const override;
      // [END] IInstance Implementation



      //--------------------------------------------------------------
      /// \brief			            Post a command to the plugin
      /// \param  command           Command to post
      //--------------------------------------------------------------
      virtual void postCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> command) const;

      //--------------------------------------------------------------
      /// \brief			            Post a manually device creation request to the plugin
      /// \param  request           Request data
      //--------------------------------------------------------------
      virtual void postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> & request) const;

		//--------------------------------------------------------------
		/// \brief                 Post a custom query request to a plugin
		/// \param [in] request    Request data
		//--------------------------------------------------------------
      virtual void postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> & request) const;

   private:
      //--------------------------------------------------------------
      /// \brief			      The plugin information
      //--------------------------------------------------------------
      const boost::shared_ptr<const shared::plugin::information::IInformation> m_pluginInformation;

      //-----------------------------------------------------
      ///\brief               Instance data
      //-----------------------------------------------------
      boost::shared_ptr<const database::entities::CPlugin> m_instanceData;

      //-----------------------------------------------------
      ///\brief               The data provider
      //-----------------------------------------------------
      boost::shared_ptr<database::IDataProvider> m_dataProvider;

      //-----------------------------------------------------
      ///\brief               The device manager
      //-----------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IDeviceManager> m_deviceManager;

      //-----------------------------------------------------
      ///\brief               The acquisition historizer
      //-----------------------------------------------------
      boost::shared_ptr<dataAccessLayer::IAcquisitionHistorizer> m_acquisitionHistorizer;

      //--------------------------------------------------------------
      /// \brief			The plugin instance
      //--------------------------------------------------------------
      boost::shared_ptr<shared::plugin::IPlugin> m_pPluginInstance;

      //--------------------------------------------------------------
      /// \brief			The plugin qualifier
      //--------------------------------------------------------------
      const boost::shared_ptr<IQualifier> m_qualifier;

      //--------------------------------------------------------------
      /// \brief			The supervisor event handler
      //--------------------------------------------------------------
      boost::shared_ptr<shared::event::CEventHandler> m_supervisor;

      //--------------------------------------------------------------
      /// \brief			ID to use to send events to supervisor
      //--------------------------------------------------------------
      const int m_pluginManagerEventId;

      //-----------------------------------------------------
      ///\brief               The script runner
      //-----------------------------------------------------
      boost::shared_ptr<shared::process::IRunner> m_runner;
   };

} // namespace pluginSystem
