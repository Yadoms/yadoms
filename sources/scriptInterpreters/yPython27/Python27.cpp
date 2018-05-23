#include "stdafx.h"
#include "Python27.h"
#include <interpreter_cpp_api/ImplementationHelper.h>
#include "ScriptFile.h"
#include <shared/script/yInterpreterApi/IAvalaibleRequest.h>
#include <shared/script/yInterpreterApi/ILoadScriptContentRequest.h>
#include <shared/script/yInterpreterApi/ISaveScriptContentRequest.h>
#include <shared/script/yInterpreterApi/IStartScript.h>
#include <shared/script/yInterpreterApi/IStopScript.h>
#include <shared/script/yInterpreterApi/IPurgeScriptLog.h>
#include "Factory.h"
#include "EventScriptStopped.h"
#include <shared/Log.h>
#include "ScriptProcess.h"
#include <ScriptLogger.h>

// Declare the script interpreter
IMPLEMENT_INTERPRETER(CPython27)

enum
{
   kEventScriptStopped = yApi::IYInterpreterApi::kPluginFirstEventId
};


CPython27::CPython27()
   : m_factory(boost::make_shared<CFactory>()),
     m_pythonExecutable(m_factory->createPythonExecutable())
{
}

CPython27::~CPython27()
{
}

void CPython27::doWork(boost::shared_ptr<yApi::IYInterpreterApi> api)
{
   m_api = api;

   YADOMS_LOG(information) << "Python interpreter is starting...";

   while (true)
   {
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYInterpreterApi::kEventStopRequested:
         {
            // Yadoms request the interpreter to stop. Note that interpreter must be stop in 10 seconds max, otherwise it will be killed.
            YADOMS_LOG(information) << "Stop requested";
            onStopRequested();
            YADOMS_LOG(information) << "Python interpreter is stopped";
            return;
         }

      case yApi::IYInterpreterApi::kEventAvalaibleRequest:
         {
            auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IAvalaibleRequest>>();
            request->sendSuccess(isAvailable());
            break;
         }

      case yApi::IYInterpreterApi::kEventLoadScriptContentRequest:
         {
            auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::ILoadScriptContentRequest>>();
            try
            {
               request->sendSuccess(loadScriptContent(request->getScriptPath()));
            }
            catch (std::exception& e)
            {
               request->sendError(std::string("Unable to load script content : ") + e.what());
            }
            break;
         }

      case yApi::IYInterpreterApi::kEventSaveScriptContent:
         {
            auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::ISaveScriptContentRequest>>();
            try
            {
               saveScriptContent(request->getScriptPath(),
                                 request->getScriptContent());
               request->sendSuccess();
            }
            catch (std::exception& e)
            {
               request->sendError(std::string("Unable to save script content : ") + e.what());
            }
            break;
         }

      case yApi::IYInterpreterApi::kEventStartScript:
         {
            const auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IStartScript>>();
            startScript(request->getScriptInstanceId(),
                        request->getScriptPath(),
                        request->getScriptApiId(),
                        request->getScriptLogPath());
            break;
         }

      case yApi::IYInterpreterApi::kEventStopScript:
         {
            const auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IStopScript>>();
            stopScript(request->getScriptInstanceId());
            break;
         }

      case yApi::IYInterpreterApi::kEventPurgeScriptLog:
         {
            const auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IPurgeScriptLog>>();
            deleteScriptLog(request->getScriptInstanceId(),
                            request->getScriptLogPath());
            break;
         }

      case kEventScriptStopped:
         {
            onScriptStopped(api->getEventHandler().getEventData<boost::shared_ptr<const IEventScriptStopped>>());
            break;
         }

      default:
         {
            YADOMS_LOG(error) << "Unknown or unsupported message id " << api->getEventHandler().getEventId();
            break;
         }
      }
   }
}

const boost::filesystem::path& CPython27::getInterpreterPath() const
{
   static const auto InterpreterPath = m_api->getInformation()->getPath();
   return InterpreterPath;
}

const std::string& CPython27::getScriptTemplate() const
{
   static const auto ScriptTemplate = CScriptFile::pythonFileRead(boost::filesystem::path(getInterpreterPath() / "template.py").string());
   return ScriptTemplate;
}

bool CPython27::isAvailable() const
{
   if (!m_pythonExecutable->found())
      return false;

   // Now check version
   static const std::string ExpectedVersionString("Python 2.7");
   if (m_pythonExecutable->version().find(ExpectedVersionString) == std::string::npos)
      return false;

   return true;
}

std::string CPython27::loadScriptContent(const std::string& scriptPath) const
{
   if (scriptPath.empty())
      return getScriptTemplate();

   CScriptFile file(scriptPath);
   return file.read();
}

void CPython27::saveScriptContent(const std::string& scriptPath,
                                  const std::string& content)
{
   CScriptFile file(scriptPath);
   file.write(content);
}

void CPython27::startScript(int scriptInstanceId,
                            const boost::filesystem::path& scriptPath,
                            const std::string& scriptApiId,
                            const boost::filesystem::path& scriptLogPath)
{
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_processesMutex);
      if (m_scriptProcesses.find(scriptInstanceId) != m_scriptProcesses.end())
      {
         YADOMS_LOG(error) << "Unable to start script #" << scriptInstanceId << " : script is already running";
         return;
      }
   }

   try
   {
      m_scriptProcesses[scriptInstanceId] = m_factory->createScriptProcess(scriptInstanceId,
                                                                           scriptPath,
                                                                           m_pythonExecutable,
                                                                           getInterpreterPath(),
                                                                           scriptApiId,
                                                                           scriptLogPath,
                                                                           [this](bool running, int scriptId, const std::string& error)
                                                                           {
                                                                              if (!running)
                                                                                 m_api->getEventHandler().postEvent(kEventScriptStopped,
                                                                                                                    static_cast<boost::shared_ptr<const IEventScriptStopped>>(boost::make_shared<const CEventScriptStopped>(scriptId, error)));
                                                                           });
   }
   catch (std::exception& e)
   {
      YADOMS_LOG(error) << "Fail to start script #" << scriptInstanceId << " : " << e.what();
      m_api->notifyScriptStopped(scriptInstanceId,
                                 "Fail to start script");
   }
}

void CPython27::stopScript(int scriptInstanceId)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_processesMutex);
   const auto script = m_scriptProcesses.find(scriptInstanceId);
   if (script == m_scriptProcesses.end())
   {
      YADOMS_LOG(error) << "Unable to stop script #" << scriptInstanceId << " : unknown script, maybe already stopped";
      return;
   }
   script->second->kill();
}

void CPython27::onScriptStopped(boost::shared_ptr<const IEventScriptStopped> eventStopped)
{
   m_api->notifyScriptStopped(eventStopped->scriptId(),
                              eventStopped->error());

   boost::lock_guard<boost::recursive_mutex> lock(m_processesMutex);
   const auto script = m_scriptProcesses.find(eventStopped->scriptId());
   if (script != m_scriptProcesses.end())
      m_scriptProcesses.erase(script);
}

void CPython27::onStopRequested()
{
   // Ask all scripts to stop
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_processesMutex);
      for (const auto& scriptToStop : m_scriptProcesses)
         stopScript(scriptToStop.first);
   }

   // Wait scripts are stopped
   while (!m_scriptProcesses.empty())
   {
      switch (m_api->getEventHandler().waitForEvents(boost::posix_time::seconds(10)))
      {
      case kEventScriptStopped:
         {
            onScriptStopped(m_api->getEventHandler().getEventData<boost::shared_ptr<const IEventScriptStopped>>());
            break;
         }
      default:
         break;
      }
   }
}

void CPython27::deleteScriptLog(int scriptInstanceId,
                                const boost::filesystem::path& scriptLogPath)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_processesMutex);

   const auto scriptProcessIt = m_scriptProcesses.find(scriptInstanceId);
   if (scriptProcessIt == m_scriptProcesses.end())
   {
      // Script is not running
      interpreter_cpp_api::CScriptLogger::purgeLogFile(scriptLogPath.parent_path());
      return;
   }

   const auto scriptProcess = boost::dynamic_pointer_cast<CScriptProcess>(scriptProcessIt->second);
   if (!scriptProcess)
      return;

   scriptProcess->logger()->purgeLogFile();
}

