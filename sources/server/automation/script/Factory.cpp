#include "stdafx.h"
#include "Factory.h"
#include "InterpreterNotFound.hpp"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/OutOfRange.hpp>
#include <shared/script/IInterpreter.h>
#include <shared/DynamicLibrary.h>
#include "InterpreterLibrary.h"
#include "YScriptApiImplementation.h"
#include "Properties.h"

namespace automation { namespace script
{

CFactory::CFactory(const std::string& interpretersPath,
      boost::shared_ptr<communication::ISendMessageAsync> pluginGateway,
      boost::shared_ptr<shared::notification::CNotificationCenter> notificationCenter,
      boost::shared_ptr<database::IAcquisitionRequester> dbAcquisitionRequester)
   :m_interpretersPath(interpretersPath),
   m_pluginGateway(pluginGateway),
   m_notificationCenter(notificationCenter),
   m_dbAcquisitionRequester(dbAcquisitionRequester)
{
}

CFactory::~CFactory()
{         
}

void CFactory::loadInterpreters()
{
   std::vector<boost::filesystem::path> interpreterDirectories = findInterpreterDirectories();

   for (std::vector<boost::filesystem::path>::const_iterator interpreterDirectory = interpreterDirectories.begin();
      interpreterDirectory != interpreterDirectories.end(); ++interpreterDirectory)
   {
      std::string interperterKeyName = interpreterDirectory->filename().string();
      if (m_LoadedInterpreters.find(interperterKeyName) == m_LoadedInterpreters.end())
      {
         // Not already loaded
         try
         {
            boost::shared_ptr<IInterpreterLibrary> library(new CInterpreterLibrary(toLibraryPath(interpreterDirectory->filename().string())));
            m_LoadedInterpreters[interperterKeyName] = library;
         }
         catch (shared::exception::CInvalidParameter& e)
         {
            YADOMS_LOG(warning) << "Interpreter " << interpreterDirectory->filename().string() << "is not valid and will not be loaded : " << e.what();
         }
      }
   }
}

boost::filesystem::path CFactory::toLibraryPath(const std::string& interpreterName) const
{
   boost::filesystem::path path(m_interpretersPath);
   path /= interpreterName;
   path /= shared::CDynamicLibrary::ToFileName(interpreterName);
   return path;
}

std::vector<boost::filesystem::path> CFactory::findInterpreterDirectories()
{
   // Look for all subdirectories in m_interpretersPath directory, where it contains library with same name,
   // for example a subdirectory "Python" containing a "Python.dll|so" file
   std::vector<boost::filesystem::path> interpreters;

   if (boost::filesystem::exists(m_interpretersPath) && boost::filesystem::is_directory(m_interpretersPath))
   {
      // Check all subdirectories in m_pluginPath
      for(boost::filesystem::directory_iterator subDirIterator(m_interpretersPath) ; subDirIterator != boost::filesystem::directory_iterator() ; ++subDirIterator)
      {
         if (boost::filesystem::is_directory(subDirIterator->status()))
         {
            // Subdirectory, check if it is a interpreter (= contains a dynamic library with same name)
            const std::string expectedLibName(shared::CDynamicLibrary::ToFileName(subDirIterator->path().filename().string()));
            for (boost::filesystem::directory_iterator fileIterator(subDirIterator->path()); fileIterator != boost::filesystem::directory_iterator() ; ++fileIterator)
            {
               if (boost::filesystem::is_regular_file(fileIterator->status()) &&                // It's a file...
                  boost::iequals(fileIterator->path().filename().string(), expectedLibName))    // ...with the same name as sub-directory...
               {
                  interpreters.push_back(subDirIterator->path());
               }
            }
         }
      }
   }

   return interpreters;
}

std::vector<std::string> CFactory::getAvailableInterpreters() 
{
   std::vector<std::string> interpreters;

   // Update loaded interpreters list if necessary
   loadInterpreters();

   // Now find corresponding interpreter
   for (std::map<std::string, boost::shared_ptr<IInterpreterLibrary> >::const_iterator itInterpreter = m_LoadedInterpreters.begin();
      itInterpreter != m_LoadedInterpreters.end(); ++itInterpreter)
   {
      boost::shared_ptr<shared::script::IInterpreter> interpreter(itInterpreter->second->getInterpreter());
      interpreters.push_back(interpreter->name());
   }

   return interpreters;
}

boost::shared_ptr<IProperties> CFactory::createScriptProperties(boost::shared_ptr<const database::entities::CRule> ruleData)
{
   boost::shared_ptr<IProperties> properties(new CProperties(ruleData));
   return properties;
}

std::string CFactory::getScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData)
{
   boost::shared_ptr<IProperties> scriptProperties(createScriptProperties(ruleData));

   // Create the file and put the code in (delegate to the interpreter)
   boost::shared_ptr<shared::script::IInterpreter> scriptInterpreter = getAssociatedInterpreter(scriptProperties->interpreterName());
   return scriptInterpreter->loadScriptContent(scriptProperties->scriptPath()); 
}

void CFactory::updateScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData, const std::string& code)
{
   boost::shared_ptr<IProperties> scriptProperties(createScriptProperties(ruleData));

   // First build directory tree
   boost::filesystem::remove_all(scriptProperties->scriptPath());
   boost::filesystem::create_directories(scriptProperties->scriptPath());

   // Create the file and put the code in (delegate to the interpreter)
   boost::shared_ptr<shared::script::IInterpreter> scriptInterpreter = getAssociatedInterpreter(scriptProperties->interpreterName());
   scriptInterpreter->saveScriptContent(scriptProperties->scriptPath(), code);
}

void CFactory::deleteScriptFile(boost::shared_ptr<const database::entities::CRule> ruleData, bool doBackup)
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

boost::shared_ptr<shared::script::IRunner> CFactory::createScriptRunner(boost::shared_ptr<const IProperties> scriptProperties)
{
   try
   {
      boost::shared_ptr<shared::script::IInterpreter> scriptInterpreter = getAssociatedInterpreter(scriptProperties->interpreterName());

      return scriptInterpreter->createRunner(scriptProperties->scriptPath(), scriptProperties->configuration());
   }
   catch (CInterpreterNotFound& e)
   {
      YADOMS_LOG(error) << "Unable to run script : " << e.what();
      throw shared::exception::CInvalidParameter("Unable to run script");
   }
   catch (shared::exception::COutOfRange& e)
   {
      YADOMS_LOG(error) << "Script type is not supported : " << e.what();
      throw shared::exception::CInvalidParameter("script type");
   }
}

boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> CFactory::createScriptContext()
{
   boost::shared_ptr<shared::script::yScriptApi::IYScriptApi> context(
      new CYScriptApiImplementation(m_pluginGateway, m_notificationCenter, m_dbAcquisitionRequester));
   return context;
}


boost::shared_ptr<shared::script::IInterpreter> CFactory::getAssociatedInterpreter(const std::string& interpreterName)
{
   // Update loaded interpreters list if necessary
   loadInterpreters();

   // Now find corresponding interpreter
   std::map<std::string, boost::shared_ptr<IInterpreterLibrary> >::const_iterator itInterpreter = m_LoadedInterpreters.find(interpreterName);
   if (itInterpreter == m_LoadedInterpreters.end())
      throw CInterpreterNotFound(interpreterName);

   boost::shared_ptr<shared::script::IInterpreter> interpreter(itInterpreter->second->getInterpreter());
   if (!interpreter->isAvailable())
      throw CInterpreterNotFound(interpreterName);
   
   return interpreter;
}

} } // namespace automation::script

