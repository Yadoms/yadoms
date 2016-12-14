//
// Instance.h
//
// yadoms-plugin instance handler
//
#pragma once

#include <shared/process/IProcess.h>
#include <shared/plugin/information/IInformation.h>
#include <shared/plugin/yPluginApi/IBindingQueryRequest.h>
#include <shared/plugin/yPluginApi/IDeviceCommand.h>
#include <shared/plugin/yPluginApi/IExtraQuery.h>
#include "IInstance.h"
#include "IIpcAdapter.h"

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
      /// \param [in]	instanceInformation        Information on the instance
      /// \param [in]	pluginInformation          Information on the plugin
      /// \param [in]   dataPath                   the instance data path
      /// \param [in]   logPath                    the instance log path
      /// \param [in]   process                    The instance process
      /// \param [in]   ipcAdapter                 The api IPC adapter
      //--------------------------------------------------------------
      CInstance(boost::shared_ptr<const database::entities::CPlugin> instanceInformation,
                const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                const boost::filesystem::path& dataPath,
                const boost::filesystem::path& logPath,
                boost::shared_ptr<shared::process::IProcess> process,
                boost::shared_ptr<IIpcAdapter> ipcAdapter);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CInstance();

      // IInstance Implementation
      void updateConfiguration(const shared::CDataContainer& newConfiguration) override;
      void requestStop() override;
      void kill() override;
      boost::shared_ptr<const database::entities::CPlugin> about() const override;
      boost::shared_ptr<const shared::plugin::information::IInformation> aboutPlugin() const override;
      void postDeviceCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> deviceCommand) override;
      void postExtraQuery(boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> extraQuery) override;
      void postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request) override;
      void postDeviceConfigurationSchemaRequest(boost::shared_ptr<shared::plugin::yPluginApi::IDeviceConfigurationSchemaRequest> request) override;
      void postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request) override;
      void postSetDeviceConfiguration(boost::shared_ptr<const shared::plugin::yPluginApi::ISetDeviceConfiguration> command) override;
      void postDeviceRemoved(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceRemoved> event) override;
      // [END] IInstance Implementation

   protected:
      void postInit(boost::shared_ptr<const shared::plugin::information::IInformation> information, const boost::filesystem::path& dataPath, const boost::filesystem::path& logPath) const;
      void postStopRequest() const;

   private:
      //--------------------------------------------------------------
      /// \brief			      The plugin information
      //--------------------------------------------------------------
      const boost::shared_ptr<const database::entities::CPlugin> m_instanceInformation;

      //--------------------------------------------------------------
      /// \brief			      The plugin information
      //--------------------------------------------------------------
      const boost::shared_ptr<const shared::plugin::information::IInformation> m_pluginInformation;

      //-----------------------------------------------------
      ///\brief               The script process
      //-----------------------------------------------------
      boost::shared_ptr<shared::process::IProcess> m_process;

      //-----------------------------------------------------
      ///\brief               The api context accessor
      //-----------------------------------------------------
      boost::shared_ptr<IIpcAdapter> m_ipcAdapter;
   };
} // namespace pluginSystem
