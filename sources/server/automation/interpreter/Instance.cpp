#include "stdafx.h"
#include "Instance.h"
#include "communication/callback/SynchronousCallback.h"
#include <shared/Log.h>
#include "AvalaibleRequest.h"
#include "LoadScriptContentRequest.h"
#include "SaveScriptContentRequest.h"
#include "StartScriptRequest.h"
#include "StopScriptRequest.h"

namespace automation
{
   namespace interpreter
   {
      CInstance::CInstance(const boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
                           boost::shared_ptr<shared::process::IProcess> process,
                           boost::shared_ptr<IIpcAdapter> ipcAdapter)
         : m_interpreterInformation(interpreterInformation),
           m_process(process),
           m_ipcAdapter(ipcAdapter),
           m_avalaible(false)
      {
         m_ipcAdapter->postInit(m_interpreterInformation);
         m_avalaible = getAvalaibility();
      }

      CInstance::~CInstance()
      {
         m_ipcAdapter->postStopRequest();
      }

      boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> CInstance::aboutInterpreter() const
      {
         return m_interpreterInformation;
      }

      bool CInstance::isAvalaible()
      {
         return m_avalaible;
      }

      std::string CInstance::loadScriptContent(const std::string& scriptPath) const
      {
         communication::callback::CSynchronousCallback<std::string> callback;
         auto request(boost::make_shared<CLoadScriptContentRequest>(scriptPath,
                                                                    callback));

         try
         {
            m_ipcAdapter->postLoadScriptContentRequest(request);
            YADOMS_LOG(debug) << "Send loadScriptContentRequest to interpreter " << m_interpreterInformation->getName();

            switch (callback.waitForResult(boost::posix_time::seconds(30)))
            {
            case communication::callback::CSynchronousCallback<bool>::kResult:
               {
                  auto res = callback.getCallbackResult();
                  if (res.Success)
                     return res.Result();
                  YADOMS_LOG(error) << "Unable to load script content from interpreter " << m_interpreterInformation->getName() << " : " << res.ErrorMessage();
               }
            default:
               YADOMS_LOG(error) << "Unable to load script content from interpreter " << m_interpreterInformation->getName() << " : timeout";
            }
         }
         catch (std::exception& e)
         {
            request->sendError((boost::format("Error when loading script content from interpreter %1% : %2%") % m_interpreterInformation->getName() % e.what()).str());
         }

         return std::string();
      }

      void CInstance::saveScriptContent(const std::string& scriptPath,
                                        const std::string& scriptContent) const
      {
         communication::callback::CSynchronousCallback<bool> callback;
         auto request(boost::make_shared<CSaveScriptContentRequest>(scriptPath,
                                                                    scriptContent,
                                                                    callback));

         try
         {
            m_ipcAdapter->postSaveScriptContentRequest(request);
            YADOMS_LOG(debug) << "Send saveScriptContentRequest to interpreter " << m_interpreterInformation->getName();

            switch (callback.waitForResult(boost::posix_time::seconds(30)))
            {
            case communication::callback::CSynchronousCallback<bool>::kResult:
               {
                  auto res = callback.getCallbackResult();
                  if (res.Success)
                     return;
                  YADOMS_LOG(error) << "Unable to save script content from interpreter " << m_interpreterInformation->getName() << " : " << res.ErrorMessage();
               }
            default:
               YADOMS_LOG(error) << "Unable to save script content from interpreter " << m_interpreterInformation->getName() << " : timeout";
            }
         }
         catch (std::exception& e)
         {
            request->sendError((boost::format("Error when saving script content from interpreter %1% : %2%") % m_interpreterInformation->getName() % e.what()).str());
         }
      }

      std::string CInstance::startScript(const std::string& scriptPath,
                                         const std::string& yScriptApiId) const
      {
         communication::callback::CSynchronousCallback<std::string> callback;
         auto request(boost::make_shared<CStartScriptRequest>(scriptPath,
                                                              yScriptApiId,
                                                              callback));

         try
         {
            m_ipcAdapter->postStartScriptRequest(request);
            YADOMS_LOG(debug) << "Send startScriptRequest to interpreter " << m_interpreterInformation->getName();

            switch (callback.waitForResult(boost::posix_time::seconds(30)))
            {
            case communication::callback::CSynchronousCallback<std::string>::kResult:
               {
                  auto res = callback.getCallbackResult();
                  if (res.Success)
                     return res.Result();
                  YADOMS_LOG(error) << "Unable to start script from interpreter " << m_interpreterInformation->getName() << " : " << res.ErrorMessage();
               }
            default:
               YADOMS_LOG(error) << "Unable to start script from interpreter " << m_interpreterInformation->getName() << " : timeout";
            }
         }
         catch (std::exception& e)
         {
            request->sendError((boost::format("Error when starting script from interpreter %1% : %2%") % m_interpreterInformation->getName() % e.what()).str());
         }

         return std::string();
      }

      void CInstance::stopScript(const std::string& scriptProcessId) const
      {
         communication::callback::CSynchronousCallback<bool> callback;
         auto request(boost::make_shared<CStopScriptRequest>(scriptProcessId,
                                                             callback));

         try
         {
            m_ipcAdapter->postStopScriptRequest(request);
            YADOMS_LOG(debug) << "Send stopScriptRequest to interpreter " << m_interpreterInformation->getName();

            switch (callback.waitForResult(boost::posix_time::seconds(30)))
            {
            case communication::callback::CSynchronousCallback<bool>::kResult:
               {
                  auto res = callback.getCallbackResult();
                  if (res.Success)
                     return;
                  YADOMS_LOG(error) << "Unable to stop script from interpreter " << m_interpreterInformation->getName() << " : " << res.ErrorMessage();
               }
            default:
               YADOMS_LOG(error) << "Unable to stop script from interpreter " << m_interpreterInformation->getName() << " : timeout";
            }
         }
         catch (std::exception& e)
         {
            request->sendError((boost::format("Error when stopping script from interpreter %1% : %2%") % m_interpreterInformation->getName() % e.what()).str());
         }
      }

      bool CInstance::getAvalaibility() const
      {
         communication::callback::CSynchronousCallback<bool> callback;
         auto request(boost::make_shared<CAvalaibleRequest>(callback));

         try
         {
            m_ipcAdapter->postAvalaibleRequest(request);
            YADOMS_LOG(debug) << "Send AvalaibleRequest to interpreter " << m_interpreterInformation->getName();

            switch (callback.waitForResult(boost::posix_time::seconds(30)))
            {
            case communication::callback::CSynchronousCallback<bool>::kResult:
               {
                  auto res = callback.getCallbackResult();
                  if (res.Success)
                     return res.Result();
                  YADOMS_LOG(error) << "Unable to get avalaibility of interpreter " << m_interpreterInformation->getName() << " : " << res.ErrorMessage();
               }
            default:
               YADOMS_LOG(error) << "Unable to get avalaibility of interpreter " << m_interpreterInformation->getName() << " : timeout";
            }
         }
         catch (std::exception& e)
         {
            request->sendError((boost::format("Error when requesting AvalaibleRequest on interpreter %1% : %2%") % m_interpreterInformation->getName() % e.what()).str());
         }

         return false;
      }
   }
} // namespace automation::interpreter


