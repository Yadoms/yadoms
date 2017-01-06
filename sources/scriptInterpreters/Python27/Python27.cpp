#include "stdafx.h"
#include "Python27.h"
#include <interpreter_cpp_api/ImplementationHelper.h>
#include "ScriptFile.h"
#include <shared/script/yInterpreterApi/IAvalaibleRequest.h>
#include <shared/script/yInterpreterApi/ILoadScriptContentRequest.h>
#include <shared/script/yInterpreterApi/ISaveScriptContentRequest.h>
#include <shared/script/yInterpreterApi/IStartScript.h>
#include <shared/script/yInterpreterApi/IStopScript.h>
#include "Factory.h"

// Declare the script interpreter
IMPLEMENT_INTERPRETER(CPython27)


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

   std::cout << "Python interpreter is starting..." << std::endl;

   while (true)
   {
      switch (api->getEventHandler().waitForEvents())
      {
      case yApi::IYInterpreterApi::kEventStopRequested:
         {
            // Yadoms request the interpreter to stop. Note that interpreter must be stop in 10 seconds max, otherwise it will be killed.
            std::cout << "Stop requested" << std::endl;

            int scriptInstanceIdToStop;
            while (true)
            {
               {
                  boost::lock_guard<boost::recursive_mutex> lock(m_processesMutex);
                  if (m_processes.empty())
                     break;
                  scriptInstanceIdToStop = m_processes.begin()->first;
               }
               stopScript(scriptInstanceIdToStop);
            }

            std::cout << "Python interpreter is stopped" << std::endl;
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
            auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IStartScript>>();
            startScript(request->getScriptInstanceId(),
                        request->getScriptPath(),
                        request->getScriptApiId());
            break;
         }

      case yApi::IYInterpreterApi::kEventStopScript:
         {
            auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IStopScript>>();
            stopScript(request->getScriptInstanceId());
         }

      default:
         {
            std::cerr << "Unknown or unsupported message id " << api->getEventHandler().getEventId() << std::endl;
            break;
         }
      }
   }
}

const boost::filesystem::path& CPython27::getInterpreterPath() const
{
   static const auto interpreterPath = m_api->getInformation()->getPath();
   return interpreterPath;
}

const std::string& CPython27::getScriptTemplate() const
{
   static const auto scriptTemplate = CScriptFile::PythonFileRead(boost::filesystem::path(getInterpreterPath() / "template.py").string());
   return scriptTemplate;
}

bool CPython27::isAvailable() const
{
   if (!m_pythonExecutable->found())
      return false;

   // Now check version
   static const std::string expectedVersionString("Python 2.7");
   if (m_pythonExecutable->version().find(expectedVersionString) == std::string::npos)
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
                            const std::string& scriptPath,
                            const std::string& scriptApiId)
{
   {
      boost::lock_guard<boost::recursive_mutex> lock(m_processesMutex);
      if (m_processes.find(scriptInstanceId) != m_processes.end())
      {
         std::cerr << "Unable to start script #" << scriptInstanceId << " : script is already running" << std::endl;
         return;
      }
   }

   try
   {
      try
      {
         m_processes[scriptInstanceId] = m_factory->createScriptProcess(scriptInstanceId,
                                                                        scriptPath,
                                                                        m_pythonExecutable,
                                                                        getInterpreterPath(),
                                                                        scriptApiId,
                                                                        [this](bool running, int scriptId)
                                                                        {
                                                                           if (!running)
                                                                              onScriptStopped(scriptId);
                                                                        });
      }
      catch (std::exception& e)
      {
         std::cerr << "Fail to start script #" << scriptInstanceId << " : " << e.what() << std::endl;
         m_api->notifyScriptStopped(scriptInstanceId,
                                    "Fail to start script");
      }
   }
   catch (std::exception& e)
   {
      m_api->notifyScriptStopped(scriptInstanceId,
                                 std::string("Unable to start script : ") + e.what());
   }
}

void CPython27::stopScript(int scriptInstanceId)
{
   boost::lock_guard<boost::recursive_mutex> lock(m_processesMutex);
   if (m_processes.find(scriptInstanceId) == m_processes.end())
   {
      std::cerr << "Unable to stop script #" << scriptInstanceId << " : unknown script, maybe already stopped" << std::endl;
      return;
   }
   m_processes.erase(scriptInstanceId);
}

void CPython27::onScriptStopped(int scriptInstanceId)
{
   m_api->notifyScriptStopped(scriptInstanceId);

   boost::lock_guard<boost::recursive_mutex> lock(m_processesMutex);
   const auto script = m_processes.find(scriptInstanceId);
   if (script != m_processes.end())
      m_processes.erase(script);
}

