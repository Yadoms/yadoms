#include "stdafx.h"
#include "Manager.h"
#include "InterpreterNotFound.hpp"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/OutOfRange.hpp>
#include <shared/script/IInterpreter.h>
#include "StopNotifier.h"
#include <shared/DynamicLibrary.h>
#include "InterpreterLibrary.h"
#include "YScriptApiImplementation.h"
#include "GeneralInfo.h"
#include "Properties.h"
#include "tools/SupportedPlatformsChecker.h"
#include <shared/process/Logger.h>

namespace automation
{
   namespace script
   {
      CManager::CManager(const IPathProvider& pathProvider,
                         boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
                         boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager,
                         boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester,
                         boost::shared_ptr<database::IDeviceRequester> dbDeviceRequester,
                         boost::shared_ptr<dataAccessLayer::IKeywordManager> keywordAccessLayer,
                         boost::shared_ptr<database::IRecipientRequester> dbRecipientRequester,
                         boost::shared_ptr<shared::ILocation> location)
         : m_pathProvider(pathProvider),
           m_pluginGateway(pluginGateway),
           m_configurationManager(configurationManager),
           m_dbAcquisitionRequester(dbAcquisitionRequester),
           m_dbDeviceRequester(dbDeviceRequester),
           m_keywordAccessLayer(keywordAccessLayer),
           m_dbRecipientRequester(dbRecipientRequester),
           m_generalInfo(boost::make_shared<CGeneralInfo>(location))
      {
      }

      CManager::~CManager()
      {
      }

      void CManager::loadInterpreters()
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_loadedInterpretersMutex);

         std::vector<boost::filesystem::path> interpreterDirectories = findInterpreterDirectories();

         for (std::vector<boost::filesystem::path>::const_iterator interpreterDirectory = interpreterDirectories.begin();
              interpreterDirectory != interpreterDirectories.end(); ++interpreterDirectory)
         {
            std::string interperterKeyName = interpreterDirectory->filename().string();
            if (m_loadedInterpreters.find(interperterKeyName) == m_loadedInterpreters.end())
            {
               // Not already loaded

               // Check if compatible with current platform
               try
               {
                  if (isInterpreterCompatibleWithPlatform(interperterKeyName))
                  {
                     boost::shared_ptr<IInterpreterLibrary> library(new CInterpreterLibrary(toLibraryPath(interpreterDirectory->filename().string())));
                     m_loadedInterpreters[interperterKeyName] = library;
                  }
               }
               catch (shared::exception::CInvalidParameter& e)
               {
                  YADOMS_LOG(warning) << "Interpreter " << interpreterDirectory->filename().string() << " is not valid and will not be loaded : " << e.what();
               }
            }
         }
      }

      bool CManager::isInterpreterCompatibleWithPlatform(const std::string& interpreterName) const
      {
         shared::CDataContainer container;
         try
         {
            boost::filesystem::path packageFile;
            packageFile = m_pathProvider.scriptInterpretersPath() / interpreterName / "package.json";
            container.deserializeFromFile(packageFile.string());
         }
         catch (shared::exception::CException& e)
         {
            throw shared::exception::CInvalidParameter(std::string("Error reading package.json : ") + e.what());
         }

         return tools::CSupportedPlatformsChecker::isSupported(container.get<shared::CDataContainer>("supportedPlatforms"));
      }

      boost::filesystem::path CManager::toLibraryPath(const std::string& interpreterName) const
      {
         auto path(m_pathProvider.scriptInterpretersPath());
         path /= interpreterName;
         path /= shared::CDynamicLibrary::ToFileName(interpreterName);
         return path;
      }

      std::vector<boost::filesystem::path> CManager::findInterpreterDirectories() const
      {
         // Look for all subdirectories in m_interpretersPath directory, where it contains library with same name,
         // for example a subdirectory "Python" containing a "Python.dll|so" file
         std::vector<boost::filesystem::path> interpreters;

         if (boost::filesystem::exists(m_pathProvider.scriptInterpretersPath()) && boost::filesystem::is_directory(m_pathProvider.scriptInterpretersPath()))
         {
            // Check all subdirectories in m_pluginPath
            for (boost::filesystem::directory_iterator subDirIterator(m_pathProvider.scriptInterpretersPath()); subDirIterator != boost::filesystem::directory_iterator(); ++subDirIterator)
            {
               if (boost::filesystem::is_directory(subDirIterator->status()))
               {
                  // Subdirectory, check if it is a interpreter (= contains a dynamic library with same name)
                  const auto expectedLibName(shared::CDynamicLibrary::ToFileName(subDirIterator->path().filename().string()));
                  for (boost::filesystem::directory_iterator fileIterator(subDirIterator->path()); fileIterator != boost::filesystem::directory_iterator(); ++fileIterator)
                  {
                     if (boost::filesystem::is_regular_file(fileIterator->status()) && // It's a file...
                        boost::iequals(fileIterator->path().filename().string(), expectedLibName)) // ...with the same name as sub-directory...
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
         for (std::map<std::string, boost::shared_ptr<IInterpreterLibrary>>::const_iterator itInterpreter = m_loadedInterpreters.begin();
              itInterpreter != m_loadedInterpreters.end(); ++itInterpreter)
         {
            auto interpreter(itInterpreter->second->getInterpreter());
            interpreters.push_back(interpreter->name());
         }

         return interpreters;
      }

      void CManager::unloadInterpreter(const std::string& interpreterName)
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_loadedInterpretersMutex);

         std::map<std::string, boost::shared_ptr<IInterpreterLibrary>>::const_iterator interpreter = m_loadedInterpreters.find(interpreterName);
         if (interpreter != m_loadedInterpreters.end())
            m_loadedInterpreters.erase(interpreter);
      }

      boost::shared_ptr<shared::script::IInterpreter> CManager::getAssociatedInterpreter(const std::string& interpreterName)
      {
         boost::lock_guard<boost::recursive_mutex> lock(m_loadedInterpretersMutex);

         // Update loaded interpreters list if necessary
         loadInterpreters();

         // Now find corresponding interpreter (warning interpreter name is the name returned by the interpreter, not the map key, which is the interpreter filename)
         for (std::map<std::string, boost::shared_ptr<IInterpreterLibrary>>::const_iterator itInterpreter = m_loadedInterpreters.begin(); itInterpreter != m_loadedInterpreters.end(); ++itInterpreter)
         {
            auto interpreter(itInterpreter->second->getInterpreter());
            if (interpreter->name() == interpreterName)
            {
               if (!interpreter->isAvailable())
                  throw CInterpreterNotFound(interpreterName);

               return interpreter;
            }
         }

         throw CInterpreterNotFound(interpreterName);
      }


      boost::shared_ptr<IProperties> CManager::createScriptProperties(boost::shared_ptr<const database::entities::CRule> ruleData)
      {
         boost::shared_ptr<IProperties> properties(new CProperties(ruleData));
         return properties;
      }

      std::string CManager::getScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData)
      {
         boost::shared_ptr<IProperties> scriptProperties(createScriptProperties(ruleData));

         // Load the file content (delegated to the interpreter)
         boost::shared_ptr<shared::script::IInterpreter> scriptInterpreter = getAssociatedInterpreter(scriptProperties->interpreterName());
         return scriptInterpreter->loadScriptContent(scriptProperties->scriptPath());
      }

      std::string CManager::getScriptTemplateFile(const std::string& interpreterName)
      {
         // Load the template file content (delegated to the interpreter)
         boost::shared_ptr<shared::script::IInterpreter> scriptInterpreter = getAssociatedInterpreter(interpreterName);
         return scriptInterpreter->loadScriptContent(std::string());
      }

      void CManager::updateScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string& code)
      {
         boost::shared_ptr<IProperties> scriptProperties(createScriptProperties(ruleData));

         // First build directory tree
         boost::filesystem::remove_all(scriptProperties->scriptPath());

         // Create directory chain
         boost::system::error_code ec;
         int triesCount = 0;
         do
         {
            // Under Windows, create_directories immediately called after remove_all can raise error. So retry until it works.
            boost::filesystem::create_directories(scriptProperties->scriptPath(), ec);
            if (++triesCount > 3)
               throw shared::exception::CException("Unable to create directories");
            boost::this_thread::sleep(boost::posix_time::milliseconds(200));
         }
         while (ec != boost::system::errc::success);

         // Create the file and put the code in (delegated to the interpreter)
         boost::shared_ptr<shared::script::IInterpreter> scriptInterpreter = getAssociatedInterpreter(scriptProperties->interpreterName());
         scriptInterpreter->saveScriptContent(scriptProperties->scriptPath(), code);
      }

      void CManager::deleteScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData, bool doBackup)
      {
         boost::shared_ptr<IProperties> scriptProperties(createScriptProperties(ruleData));

         if (doBackup)
         {
            const std::string backupPath(scriptProperties->scriptPath() + ".bak");
            boost::filesystem::remove_all(backupPath);
            boost::filesystem::rename(scriptProperties->scriptPath(), backupPath);
         }
         else
         {
            boost::filesystem::remove_all(scriptProperties->scriptPath());
         }
      }

      std::string CManager::getScriptLogFile(boost::shared_ptr<const database::entities::CRule> ruleData)
      {
         const auto logFile(scriptLogFile(ruleData->Id()));

         if (!boost::filesystem::exists(logFile))
            throw shared::exception::CInvalidParameter(logFile.string());

         std::ifstream file(logFile.string().c_str());
         if (!file.is_open())
            throw shared::exception::CInvalidParameter(logFile.string());

         std::istreambuf_iterator<char> eos;
         return std::string(std::istreambuf_iterator<char>(file), eos);
      }

      boost::filesystem::path CManager::scriptLogFile(int ruleId) const
      {
         return m_pathProvider.scriptsLogPath() / std::to_string(ruleId) / "rule.log";
      }

      boost::shared_ptr<shared::process::ILogger> CManager::createScriptLogger(boost::shared_ptr<const database::entities::CRule> ruleData)
      {
         return boost::make_shared<shared::process::CLogger>("script/" + ruleData->Name() + " #" + std::to_string(ruleData->Id()),
                                                             scriptLogFile(ruleData->Id()));
      }

      boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> CManager::createScriptContext(boost::shared_ptr<shared::process::ILogger> scriptLogger)
      {
         boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> yScriptApi(boost::make_shared<CYScriptApiImplementation>(scriptLogger,
                                                                                                                             m_pluginGateway,
                                                                                                                             m_dbAcquisitionRequester,
                                                                                                                             m_dbDeviceRequester,
                                                                                                                             m_keywordAccessLayer,
                                                                                                                             m_dbRecipientRequester,
                                                                                                                             m_generalInfo));
         return yScriptApi;
      }

      boost::shared_ptr<shared::process::IProcessObserver> CManager::createStopNotifier(boost::shared_ptr<IRuleStateHandler> ruleStateHandler, int ruleId)
      {
         return boost::make_shared<StopNotifier>(ruleStateHandler, ruleId);
      }
   }
} // namespace automation::script


