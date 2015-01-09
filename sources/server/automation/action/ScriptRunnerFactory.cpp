#include "stdafx.h"
#include "ScriptRunnerFactory.h"
#include "ScriptInterpreterNotFound.hpp"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/OutOfRange.hpp>
#include <shared/script/IScriptInterpreter.h>
#include <shared/DynamicLibrary.h>
#include "ScriptInterpreterLibrary.h"

namespace automation { namespace action
{

CScriptRunnerFactory::CScriptRunnerFactory(const std::string& interpretersPath)
   :m_interpretersPath(interpretersPath)
{
}

CScriptRunnerFactory::~CScriptRunnerFactory()
{         
}

void CScriptRunnerFactory::loadInterpreters()
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
            boost::shared_ptr<IScriptInterpreterLibrary> library(new CScriptInterpreterLibrary(*interpreterDirectory));
            m_LoadedInterpreters[interperterKeyName] = library->getInterpreter();
         }
         catch (shared::exception::CInvalidParameter& e)
         {
            YADOMS_LOG(warning) << "Interpreter " << interpreterDirectory->filename().string() << "is not valid and will not be loaded : " << e.what();
         }
      }
   }
}

std::vector<boost::filesystem::path> CScriptRunnerFactory::findInterpreterDirectories()
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

boost::shared_ptr<shared::script::IScriptRunner> CScriptRunnerFactory::createScriptRunner(
   const std::string& scriptPath, const shared::CDataContainer& scriptConfiguration)
{
   try
   {
      boost::shared_ptr<shared::script::IScriptInterpreter> scriptInterpreter = getScriptInterpreter(scriptPath);

      return scriptInterpreter->createRunner(scriptPath, scriptConfiguration);
   }
   catch (CScriptInterpreterNotFound& e)
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

boost::shared_ptr<shared::script::IScriptInterpreter> CScriptRunnerFactory::getScriptInterpreter(const std::string& scriptPath)
{
   // Update interpreters library list if necessary
   loadInterpreters();

   // Now find corresponding interpreter
   for (std::map<std::string, boost::shared_ptr<shared::script::IScriptInterpreter> >::const_iterator itInterpreter = m_LoadedInterpreters.begin();
      itInterpreter != m_LoadedInterpreters.end(); ++itInterpreter)
   {
      boost::shared_ptr<shared::script::IScriptInterpreter> interpreter = itInterpreter->second;
      if (interpreter->canInterpret(scriptPath) && interpreter->isAvailable())
         return interpreter;
   }

   throw CScriptInterpreterNotFound(scriptPath);
}

} } // namespace automation::action	
	
	