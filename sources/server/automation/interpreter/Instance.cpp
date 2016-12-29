#include "stdafx.h"
#include "Instance.h"

namespace automation
{
   namespace interpreter
   {
      CInstance::CInstance(const boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
                           boost::shared_ptr<shared::process::IProcess> process,
                           boost::shared_ptr<IIpcAdapter> ipcAdapter)
         : m_interpreterInformation(interpreterInformation),
           m_process(process),
           m_ipcAdapter(ipcAdapter)
      {
         postInit(m_interpreterInformation);
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

      boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> CInstance::aboutInterpreter() const
      {
         return m_interpreterInformation;
      }

      void CInstance::postAvalaibleRequest(boost::shared_ptr<shared::script::yInterpreterApi::IAvalaibleRequest> request)
      {
         m_ipcAdapter->postAvalaibleRequest(request);
      }

      void CInstance::postLoadScriptContentRequest(boost::shared_ptr<shared::script::yInterpreterApi::ILoadScriptContentRequest> request)
      {
         m_ipcAdapter->postLoadScriptContentRequest(request);
      }

      void CInstance::postSaveScriptContentRequest(boost::shared_ptr<shared::script::yInterpreterApi::ISaveScriptContentRequest> request)
      {
         m_ipcAdapter->postSaveScriptContentRequest(request);
      }

      void CInstance::postStartScriptRequest(boost::shared_ptr<shared::script::yInterpreterApi::IStartScriptRequest> request)
      {
         m_ipcAdapter->postStartScriptRequest(request);
      }

      void CInstance::postStopScriptRequest(boost::shared_ptr<shared::script::yInterpreterApi::IStopScriptRequest> request)
      {
         m_ipcAdapter->postStopScriptRequest(request);
      }

      void CInstance::postStopRequest() const
      {
         m_ipcAdapter->postStopRequest();
      }

      void CInstance::postInit(boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> information) const
      {
         m_ipcAdapter->postInit(information);
      }
   }
} // namespace automation::interpreter


