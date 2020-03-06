#include "stdafx.h"
#include "Instance.h"
#include "startupOptions/IStartupOptions.h"
#include <shared/ServiceLocator.h>

namespace pluginSystem
{
   CInstance::CInstance(boost::shared_ptr<const database::entities::CPlugin> instanceInformation,
                        const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                        const boost::filesystem::path& dataPath,
                        const boost::filesystem::path& logPath,
                        boost::shared_ptr<shared::process::IProcess> process,
                        boost::shared_ptr<IIpcAdapter> ipcAdapter)
      : m_instanceInformation(instanceInformation),
        m_pluginInformation(pluginInformation),
        m_process(process),
        m_ipcAdapter(ipcAdapter)
   {
      postInit(m_pluginInformation, dataPath, logPath);
   }

   CInstance::~CInstance()
   {
   }

   void CInstance::requestStop()
   {
      postStopRequest();
   }

   void CInstance::kill()
   {
      m_process->kill();
   }

   void CInstance::postStopRequest() const
   {
      m_ipcAdapter->postStopRequest();
   }

   void CInstance::postInit(boost::shared_ptr<const shared::plugin::information::IInformation> information,
                            const boost::filesystem::path& dataPath,
                            const boost::filesystem::path& logPath) const
   {
      const auto& startupOptions = shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>();

      m_ipcAdapter->postInit(information,
                             dataPath,
                             logPath,
                             startupOptions->getLogLevel(),
                             startupOptions->getProxyHost(),
                             startupOptions->getProxyPort(),
                             startupOptions->getProxyUsername(),
                             startupOptions->getProxyPassword(),
                             startupOptions->getProxyBypass());
   }

   void CInstance::postDeviceCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> deviceCommand)
   {
      m_ipcAdapter->postDeviceCommand(deviceCommand);
   }

   void CInstance::postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request)
   {
      m_ipcAdapter->postBindingQueryRequest(request);
   }

   void CInstance::postDeviceConfigurationSchemaRequest(boost::shared_ptr<shared::plugin::yPluginApi::IDeviceConfigurationSchemaRequest> request)
   {
      m_ipcAdapter->postDeviceConfigurationSchemaRequest(request);
   }

   void CInstance::postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request)
   {
      m_ipcAdapter->postManuallyDeviceCreationRequest(request);
   }

   void CInstance::postSetDeviceConfiguration(boost::shared_ptr<const shared::plugin::yPluginApi::ISetDeviceConfiguration> command)
   {
      m_ipcAdapter->postSetDeviceConfiguration(command);
   }

   void CInstance::postDeviceRemoved(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceRemoved> event)
   {
      m_ipcAdapter->postDeviceRemoved(event);
   }

   void CInstance::postExtraQuery(boost::shared_ptr<shared::plugin::yPluginApi::IExtraQuery> extraQuery, const std::string& taskId)
   {
      m_ipcAdapter->postExtraQuery(extraQuery, taskId);
   }

   void CInstance::updateConfiguration(const shared::CDataContainerSharedPtr& newConfiguration)
   {
      m_ipcAdapter->postUpdateConfiguration(newConfiguration);
   }

   boost::shared_ptr<const database::entities::CPlugin> CInstance::about() const
   {
      return m_instanceInformation;
   }

   boost::shared_ptr<const shared::plugin::information::IInformation> CInstance::aboutPlugin() const
   {
      return m_pluginInformation;
   }
} // namespace pluginSystem
