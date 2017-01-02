#include "stdafx.h"
#include "Python27.h"
#include <interpreter_cpp_api/ImplementationHelper.h>
#include "PythonExecutable.h"
#include "ScriptFile.h"
#include "ScriptProcess.h"
#include <shared/process/ProcessException.hpp>
#include <shared/script/yInterpreterApi/IAvalaibleRequest.h>
#include <shared/script/yInterpreterApi/ILoadScriptContentRequest.h>
#include <shared/script/yInterpreterApi/ISaveScriptContentRequest.h>
#include <shared/script/yInterpreterApi/IStartScriptRequest.h>
#include <shared/script/yInterpreterApi/IStopScriptRequest.h>

// Declare the script interpreter
IMPLEMENT_INTERPRETER(CPython27)


CPython27::CPython27()
   : m_pythonExecutable(boost::make_shared<CPythonExecutable>())
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
            //TODO
            //TODO api->setPluginState(yApi::historization::EPluginState::kStopped);
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
         auto request = api->getEventHandler().getEventData<boost::shared_ptr<yApi::IStartScriptRequest>>();
         try
         {
            //TODO
            //const auto process = createProcess();
            //request->sendSuccess(process->);
         }
         catch (std::exception& e)
         {
            request->sendError(std::string("Unable to start script : ") + e.what());
         }
         break;
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
   static const auto scriptTemplate = CScriptFile::PythonFileRead(boost::filesystem::path(getInterpreterPath().parent_path() / "template.py").string());
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

boost::shared_ptr<shared::process::IProcess> CPython27::createProcess(const std::string& scriptPath,
                                                                    boost::shared_ptr<shared::process::ILogger> scriptLogger,
                                                                    boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi,
                                                                    boost::shared_ptr<shared::process::IProcessObserver> processObserver) const
{
   try
   {
      return boost::make_shared<CScriptProcess>(m_pythonExecutable,
                                                getInterpreterPath(),
                                                boost::make_shared<CScriptFile>(scriptPath),
                                                yScriptApi,
                                                scriptLogger,
                                                processObserver);
   }
   catch (shared::process::CProcessException& ex)
   {
      std::cerr << "Unable to create the Python process, " << ex.what() << std::endl;
      return boost::shared_ptr<shared::process::IProcess>();
   }
}

