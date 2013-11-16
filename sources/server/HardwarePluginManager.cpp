#pragma once

#include "PluginLibrary.hpp"
#include "PluginInstance.hpp"

class CPluginManager
{
public:
   CPluginManager()
   {
   }

   virtual ~CPluginManager()
   {
      // m_PluginInstances should be emptied by call to StopPlugins
      BOOST_ASSERT(m_PluginInstances.empty();)

      // PluginLibraries should be emptied (or some libraries are not freed)
      BOOST_ASSERT(m_PluginInstances.empty();)
   }

   // Returns all plugin libraries installed
   std::list<std::string> FindAvalaiblePluginLibraries()
   {
      // Parse the hardware plugin directory to find all plugin libraries
      // TODO
   }

   void StartAllPlugins()
   {
      std::list<std::string> avalaibleLibraryNames = FindAvalaiblePluginLibraries();

      for (std::list<std::string>::const_iterator libNameIt = avalaibleLibraryNames.begin();
           libNameIt != avalaibleLibraryNames.end() ; ++libNameIt)
      {
         // Load the library
         CPluginLibrary* newLibrary = new CPluginLibrary(*libNameIt);
         if (!newLibrary->Load())
         {
            delete newLibrary;
            continue;
         }
         PluginLibraryList.add(newLibrary);
         
         // Plugin instanciation
         PluginInstance* newPluginInstance(newLibrary->construct());
         PluginInstances.add(newPluginInstance);

         // Start plugin
         newPluginInstance->Start();
      }
   }

   void StopAllPlugins()
   {
      for (PluginList::const_iterator it = m_PluginInstances.begin();
           it != m_PluginInstances.end() ; ++it)
      {
         // Stop the plugin
         (*it)->Stop();

         // Free plugin
         m_PluginInstances.remove(it);
         delete *it;
      }
      
      // Free the libraries
      while (!m_PluginLibraries.empty())
      {
         m_PluginLibraries.Unload();
         delete m_PluginLibraries.front();
         m_PluginLibraries.pop_front();         
      }
   }

private:
   typedef std::list<CPluginLibrary*> PluginLibraryList;
   PluginLibraryList m_PluginLibraries;

   typedef std::list<PluginInstance*> PluginList;
   PluginList m_PluginInstances;
};
