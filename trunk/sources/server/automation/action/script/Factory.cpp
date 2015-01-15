#include "stdafx.h"
#include "Factory.h"
#include "InterpreterNotFound.hpp"
#include <shared/Log.h>
#include <shared/exception/InvalidParameter.hpp>
#include <shared/exception/OutOfRange.hpp>
#include <shared/script/IInterpreter.h>
#include <shared/DynamicLibrary.h>
#include "InterpreterLibrary.h"

namespace automation { namespace action { namespace script
{

CFactory::CFactory(const std::string& interpretersPath)
   :m_interpretersPath(interpretersPath)
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

boost::shared_ptr<shared::script::IRunner> CFactory::createScriptRunner(
   const std::string& scriptName, const shared::CDataContainer& scriptConfiguration)
{
   try
   {
      boost::shared_ptr<shared::script::IInterpreter> scriptInterpreter = getAssociatedInterpreter(scriptName);

      return scriptInterpreter->createRunner(scriptName, scriptConfiguration);
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

boost::shared_ptr<shared::script::IInterpreter> CFactory::getAssociatedInterpreter(const std::string& scriptName)
{
   // Update loaded interpreters list if necessary
   loadInterpreters();

   // Now find corresponding interpreter
   for (std::map<std::string, boost::shared_ptr<IInterpreterLibrary> >::const_iterator itInterpreter = m_LoadedInterpreters.begin();
      itInterpreter != m_LoadedInterpreters.end(); ++itInterpreter)
   {
      boost::shared_ptr<shared::script::IInterpreter> interpreter(itInterpreter->second->getInterpreter());
      if (interpreter->canInterpret(scriptName) && interpreter->isAvailable())
         return interpreter;
   }

   throw CInterpreterNotFound(scriptName);
}

} } } // namespace automation::action::script

