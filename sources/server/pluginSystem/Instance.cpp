#include "stdafx.h"
#include "Instance.h"

namespace pluginSystem
{
   CInstance::CInstance(boost::shared_ptr<const database::entities::CPlugin> instanceInformation,
                        const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                        boost::shared_ptr<shared::process::IProcess> process,
                        boost::shared_ptr<IContextAccessor> contextAccessor)
      : m_instanceInformation(instanceInformation),
        m_pluginInformation(pluginInformation),
        m_process(process),
        m_contextAccessor(contextAccessor)
   {
      postPluginInformation(m_pluginInformation);
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
      m_contextAccessor->postStopRequest();
   }

   void CInstance::postPluginInformation(boost::shared_ptr<const shared::plugin::information::IInformation> information) const
   {
      m_contextAccessor->postPluginInformation(information);
   }

   void CInstance::postDeviceCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IDeviceCommand> deviceCommand) const
   {
      m_contextAccessor->postDeviceCommand(deviceCommand);
   }

   void CInstance::postBindingQueryRequest(boost::shared_ptr<shared::plugin::yPluginApi::IBindingQueryRequest> request) const
   {
      m_contextAccessor->postBindingQueryRequest(request);
   }

   void CInstance::postManuallyDeviceCreationRequest(boost::shared_ptr<shared::plugin::yPluginApi::IManuallyDeviceCreationRequest> request) const
   {
      m_contextAccessor->postManuallyDeviceCreationRequest(request);
   }

   void CInstance::postExtraCommand(boost::shared_ptr<const shared::plugin::yPluginApi::IExtraCommand> extraCommand) const
   {
      m_contextAccessor->postExtraCommand(extraCommand);
   }

   void CInstance::updateConfiguration(const shared::CDataContainer& newConfiguration) const
   {
      m_contextAccessor->postUpdateConfiguration(newConfiguration);
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
