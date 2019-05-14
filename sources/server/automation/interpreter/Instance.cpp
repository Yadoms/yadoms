#include "stdafx.h"
#include "Instance.h"
#include "communication/callback/SynchronousCallback.h"
#include <shared/Log.h>
#include "AvalaibleRequest.h"
#include "LoadScriptContentRequest.h"
#include "SaveScriptContentRequest.h"
#include "StartScript.h"
#include "StopScript.h"
#include "PurgeScriptLog.h"
#include <shared/ServiceLocator.h>
#include "startupOptions/IStartupOptions.h"
#include "automation/RuleException.hpp"

namespace automation
{
   namespace interpreter
   {
      CInstance::CInstance(const boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> interpreterInformation,
                           const boost::filesystem::path& logPath,
                           boost::shared_ptr<shared::process::IProcess> process,
                           boost::shared_ptr<IIpcAdapter> ipcAdapter)
         : m_interpreterInformation(interpreterInformation),
           m_process(process),
           m_ipcAdapter(ipcAdapter),
           m_avalaible(false)
      {
         m_ipcAdapter->postInit(m_interpreterInformation,
                                logPath,
                                shared::CServiceLocator::instance().get<const startupOptions::IStartupOptions>()->getLogLevel());
         m_avalaible = getAvalaibility();
      }

      CInstance::~CInstance()
      {
      }

      boost::shared_ptr<const shared::script::yInterpreterApi::IInformation> CInstance::aboutInterpreter() const
      {
         return m_interpreterInformation;
      }

      void CInstance::requestToStop()
      {
         m_ipcAdapter->postStopRequest();
      }

      bool CInstance::isAvalaible()
      {
         return m_avalaible;
      }

      std::string CInstance::loadScriptContent(const std::string& scriptPath) const
      {
         communication::callback::CSynchronousCallback<std::string> callback;
         const auto request(boost::make_shared<CLoadScriptContentRequest>(scriptPath,
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
                  if (res.success)
                     return res.result();
                  throw std::runtime_error(res.errorMessage());
               }
            default:
               throw std::runtime_error("timeout");
            }
         }
         catch (std::exception& e)
         {
            const auto errorMessage = (boost::format("Error when loading script content from interpreter %1% : %2%") % m_interpreterInformation->
               getName() % e.what()).str();
            YADOMS_LOG(error) << errorMessage;
            throw CRuleException(errorMessage);
         }
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
                  if (res.success)
                     return;
                  YADOMS_LOG(error) << "Unable to save script content from interpreter " << m_interpreterInformation->getName() << " : " << res.
                     errorMessage();
               }
            default:
               YADOMS_LOG(error) << "Unable to save script content from interpreter " << m_interpreterInformation->getName() << " : timeout";
            }
         }
         catch (std::exception& e)
         {
            request->sendError(
               (boost::format("Error when saving script content from interpreter %1% : %2%") % m_interpreterInformation->getName() % e.what()).str());
         }
      }

      void CInstance::startScript(int scriptInstanceId,
                                  const boost::filesystem::path& scriptPath,
                                  const std::string& yScriptApiId,
                                  const boost::filesystem::path& scriptLogPath) const
      {
         const auto request = boost::make_shared<CStartScript>(scriptInstanceId,
                                                               scriptPath,
                                                               yScriptApiId,
                                                               scriptLogPath);

         try
         {
            m_ipcAdapter->postStartScript(request);
            YADOMS_LOG(debug) << "Send startScript to interpreter " << m_interpreterInformation->getName();
         }
         catch (std::exception& e)
         {
            YADOMS_LOG(error) << "Unable to start script from interpreter " << m_interpreterInformation->getName() << " : " << e.what();
         }
      }

      void CInstance::stopScript(int scriptInstanceId) const
      {
         const auto request(boost::make_shared<CStopScript>(scriptInstanceId));

         try
         {
            m_ipcAdapter->postStopScript(request);
            YADOMS_LOG(debug) << "Send stopScript to interpreter " << m_interpreterInformation->getName();
         }
         catch (std::exception& e)
         {
            YADOMS_LOG(error) << "Error when stopping script from interpreter " << m_interpreterInformation->getName() << " : " << e.what();
         }
      }

      void CInstance::purgeScriptLog(int scriptInstanceId,
                                     const boost::filesystem::path& scriptLogPath) const
      {
         const auto request(boost::make_shared<CPurgeScriptLog>(scriptInstanceId,
                                                                scriptLogPath));

         try
         {
            m_ipcAdapter->postPurgeScriptLog(request);
            YADOMS_LOG(debug) << "Send purgeScriptLog to interpreter " << m_interpreterInformation->getName();
         }
         catch (std::exception& e)
         {
            YADOMS_LOG(error) << "Error when purging script log from interpreter " << m_interpreterInformation->getName() << " : " << e.what();
         }
      }

      bool CInstance::getAvalaibility() const
      {
         communication::callback::CSynchronousCallback<bool> callback;
         auto request(boost::make_shared<CAvalaibleRequest>(callback));

         try
         {
            m_ipcAdapter->postAvailableRequest(request);
            YADOMS_LOG(debug) << "Send AvailableRequest to interpreter " << m_interpreterInformation->getName();

            switch (callback.waitForResult(boost::posix_time::seconds(30)))
            {
            case communication::callback::CSynchronousCallback<bool>::kResult:
               {
                  auto res = callback.getCallbackResult();
                  if (res.success)
                     return res.result();
                  YADOMS_LOG(error) << "Unable to get availability of interpreter " << m_interpreterInformation->getName() << " : " << res.
                     errorMessage();
               }
            default:
               YADOMS_LOG(error) << "Unable to get availability of interpreter " << m_interpreterInformation->getName() << " : timeout";
            }
         }
         catch (std::exception& e)
         {
            request->sendError(
               (boost::format("Error when requesting AvailableRequest on interpreter %1% : %2%") % m_interpreterInformation->getName() % e.what()).
               str());
         }

         return false;
      }
   }
} // namespace automation::interpreter
