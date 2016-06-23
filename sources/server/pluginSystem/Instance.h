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
#include <shared/plugin/yPluginApi/IExtraCommand.h>
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
      /// \param [in]   process                    The instance process
      /// \param [in]   ipcAdapter                 The api IPC adapter
      //--------------------------------------------------------------
      CInstance(boost::shared_ptr<const database::entities::CPlugin> instanceInformation,
                const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
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
      void postExtraCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IExtraCommand> extraCommand) override;
      void postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request) override;
      void postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request) override;
      // [END] IInstance Implementation

   protected:
      void postPluginInformation(boost::shared_ptr<const shared::plugin::information::IInformation> information) const;
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
