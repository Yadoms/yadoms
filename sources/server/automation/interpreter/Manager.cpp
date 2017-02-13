#include "stdafx.h"
#include "Manager.h"
#include "Factory.h"
#include <shared/exception/InvalidParameter.hpp>
#include "tools/SupportedPlatformsChecker.h"
#include <shared/Executable.h>
#include <shared/Log.h>

namespace automation
{
   namespace interpreter
   {
      CManager::CManager(const IPathProvider& pathProvider)
         : m_pathProvider(pathProvider),
           m_factory(boost::make_shared<CFactory>(m_pathProvider))
      {
      }

      CManager::~CManager()
      {
         unloadInterpreters();
      }

      void CManager::loadInterpreters()
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_loadedInterpretersMutex);

         for (const auto& interpreterDirectory : findInterpreterDirectories())
         {
            auto interpreterKeyName = interpreterDirectory.filename().string();
            if (m_loadedInterpreters.find(interpreterKeyName) == m_loadedInterpreters.end())
            {
               // Not already loaded

               // Check if compatible with current platform
               if (isInterpreterCompatibleWithPlatform(interpreterKeyName))
               {
                  try
                  {
                     m_loadedInterpreters[interpreterKeyName] = m_factory->createInterpreterInstance(interpreterKeyName,
                                                                                                     [this](bool running, const std::string& interpreterType)
                                                                                                     {
                                                                                                        if (!running)
                                                                                                           onInterpreterUnloaded(interpreterType);
                                                                                                     },
                                                                                                     m_onScriptStoppedFct);
                  }
                  catch (std::exception& e)
                  {
                     YADOMS_LOG(error) << "Fail to start interpreter " << interpreterKeyName << " : " << e.what();
                  }
               }
            }
         }
      }

      void CManager::unloadInterpreters()
      {
         YADOMS_LOG(information) << "Unload interpreters...";

         {
            boost::lock_guard<boost::recursive_mutex> lock(m_loadedInterpretersMutex);
            for (const auto& runningInterpreter : m_loadedInterpreters)
               runningInterpreter.second->requestToStop();
         }

         auto timeout = shared::currentTime::Provider().now() + boost::posix_time::seconds(30);
         do
         {
            {
               boost::lock_guard<boost::recursive_mutex> lock(m_loadedInterpretersMutex);
               if (m_loadedInterpreters.empty())
                  break;
            }
            boost::this_thread::yield();
         }
         while (shared::currentTime::Provider().now() < timeout);

         {
            boost::lock_guard<boost::recursive_mutex> lock(m_loadedInterpretersMutex);
            if (!m_loadedInterpreters.empty())
            {
               YADOMS_LOG(information) << "Fail to stop all interpreters";
               return;
            }
         }

         YADOMS_LOG(information) << "All interpreters are stopped";
      }

      bool CManager::isInterpreterCompatibleWithPlatform(const std::string& interpreterType) const
      {
         shared::CDataContainer container;
         try
         {
            boost::filesystem::path packageFile;
            packageFile = m_pathProvider.scriptInterpretersPath() / interpreterType / "package.json";
            container.deserializeFromFile(packageFile.string());
         }
         catch (shared::exception::CException& e)
         {
            throw shared::exception::CInvalidParameter(std::string("Error reading package.json : ") + e.what());
         }

         return tools::CSupportedPlatformsChecker::isSupported(container.get<shared::CDataContainer>("supportedPlatforms"));
      }

      std::vector<boost::filesystem::path> CManager::findInterpreterDirectories() const
      {
         // Look for all subdirectories in m_interpretersPath directory, where it contains interpreter executable file with same name,
         // for example a subdirectory "Python" containing a "python.exe" file (Windows) / "python" (other platforms)
         std::vector<boost::filesystem::path> interpreters;

         if (boost::filesystem::exists(m_pathProvider.scriptInterpretersPath()) && boost::filesystem::is_directory(m_pathProvider.scriptInterpretersPath()))
         {
            for (boost::filesystem::directory_iterator subDirIterator(m_pathProvider.scriptInterpretersPath());
                 subDirIterator != boost::filesystem::directory_iterator();
                 ++subDirIterator)
            {
               if (boost::filesystem::is_directory(subDirIterator->status()))
               {
                  // Subdirectory, check if it is a interpreter (= contains a executable file with same name)
                  const auto expectedExecutableName(shared::CExecutable::ToFileName(subDirIterator->path().filename().string()));
                  for (boost::filesystem::directory_iterator fileIterator(subDirIterator->path());
                       fileIterator != boost::filesystem::directory_iterator();
                       ++fileIterator)
                  {
                     if (boost::filesystem::is_regular_file(fileIterator->status()) && // It's a file...
                        boost::iequals(fileIterator->path().filename().string(), expectedExecutableName)) // ...with the same name as sub-directory...
                     {
                        interpreters.push_back(subDirIterator->path());
                     }
                  }
               }
            }
         }

         return interpreters;
      }

      std::vector<std::string> CManager::getAvailableInterpreters()
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_loadedInterpretersMutex);

         std::vector<std::string> interpreters;

         // Update loaded interpreters list if necessary
         loadInterpreters();

         // Now find corresponding interpreter
         for (const auto& itInterpreter : m_loadedInterpreters)
            interpreters.push_back(itInterpreter.first);

         return interpreters;
      }

      void CManager::unloadInterpreter(const std::string& interpreterType)
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_loadedInterpretersMutex);

         const auto interpreter = m_loadedInterpreters.find(interpreterType);
         if (interpreter != m_loadedInterpreters.end())
            interpreter->second->requestToStop();
      }

      void CManager::onInterpreterUnloaded(const std::string& interpreterType)
      {
         boost::thread([this, interpreterType]()
            {
               boost::lock_guard<boost::recursive_mutex> lock(m_loadedInterpretersMutex);

               const auto interpreter = m_loadedInterpreters.find(interpreterType);
               if (interpreter != m_loadedInterpreters.end())
                  m_loadedInterpreters.erase(interpreter);
            });
      }

      boost::shared_ptr<IInstance> CManager::getInterpreterInstance(const std::string& interpreterType)
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_loadedInterpretersMutex);

         // Update loaded interpreters list if necessary
         loadInterpreters();

         // Now find corresponding interpreter
         auto interpreter = m_loadedInterpreters.find(interpreterType);
         if (interpreter != m_loadedInterpreters.end() && interpreter->second->isAvalaible())
            return interpreter->second;

         throw std::runtime_error("Interpreter " + interpreterType + " not found ");
      }

      std::string CManager::getScriptContent(const std::string& interpreterType,
                                             const std::string& scriptPath)
      {
         // Load the file content (delegated to the interpreter)
         auto scriptInterpreter = getInterpreterInstance(interpreterType);
         return scriptInterpreter->loadScriptContent(scriptPath);
      }

      std::string CManager::getScriptTemplateContent(const std::string& interpreterType)
      {
         // Load the template file content (delegated to the interpreter)
         auto scriptInterpreter = getInterpreterInstance(interpreterType);
         return scriptInterpreter->loadScriptContent(std::string());
      }

      void CManager::updateScriptFile(const std::string& interpreterType,
                                      const std::string& scriptPath,
                                      const std::string& code)
      {
         // First build directory tree
         boost::filesystem::remove_all(scriptPath);

         // Create directory chain
         boost::system::error_code ec;
         auto triesCount = 0;
         do
         {
            // Under Windows, create_directories immediately called after remove_all can raise error. So retry until it works.
            boost::filesystem::create_directories(scriptPath, ec);
            if (++triesCount > 3)
               throw shared::exception::CException("Unable to create directories");
            boost::this_thread::sleep(boost::posix_time::milliseconds(200));
         }
         while (ec != boost::system::errc::success);

         // Create the file and put the code in (delegated to the interpreter)
         auto scriptInterpreter = getInterpreterInstance(interpreterType);
         scriptInterpreter->saveScriptContent(scriptPath,
                                              code);
      }

      void CManager::deleteScriptFile(const std::string& interpreterType,
                                      const std::string& scriptPath,
                                      bool doBackup)
      {
         if (doBackup)
         {
            if (!boost::filesystem::exists(scriptPath))
               return; // To remove reference to unexisting scripts (in case of database import, without associated scripts)
            const auto backupPath(scriptPath + ".bak");
            boost::filesystem::remove_all(backupPath);
            boost::filesystem::rename(scriptPath, backupPath);
         }
         else
         {
            boost::filesystem::remove_all(scriptPath);
         }
      }

      std::string CManager::getScriptLogContent(int ruleId)
      {
         const auto logFile(getScriptLogFilename(ruleId));

         if (!boost::filesystem::exists(logFile))
            throw shared::exception::CInvalidParameter(logFile.string());

         std::ifstream file(logFile.string().c_str());
         if (!file.is_open())
            throw shared::exception::CInvalidParameter(logFile.string());

         std::istreambuf_iterator<char> eos;
         return std::string(std::istreambuf_iterator<char>(file), eos);
      }

      boost::filesystem::path CManager::getScriptLogFilename(int ruleId) const
      {
         return m_pathProvider.scriptsLogPath() / std::to_string(ruleId) / "rule.log";
      }

      void CManager::setOnScriptStoppedFct(boost::function2<void, int, const std::string&> onScriptStoppedFct)
      {
         m_onScriptStoppedFct = onScriptStoppedFct;
      }
   }
} // namespace automation::interpreter
