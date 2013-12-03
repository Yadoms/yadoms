#include "stdafx.h"
#include "HardwarePluginManager.h"

CHardwarePluginManager::CHardwarePluginManager(const std::string & initialDir)
{
   buildPluginFactoryList(initialDir);
}

CHardwarePluginManager::~CHardwarePluginManager()
{
}


// Returns all plugin libraries installed
std::list<std::string> CHardwarePluginManager::findAvalaiblePlugins(const std::string & initialDir)
{
   // Parse the hardware plugin directory to find all plugin libraries
   // TODO
   std::list<std::string> result;
   // TODO : doit chercher dans initialDir, mais ne retourner que des noms.ext 
   result.push_back("fakePlugin.dll");

   return result;
}


void CHardwarePluginManager::buildPluginFactoryList(const std::string & initialDir)
{
   // Search for library files
   std::list<std::string> avalaiblePluginFileNames = findAvalaiblePlugins(initialDir);

   for (std::list<std::string>::const_iterator libNameIt = avalaiblePluginFileNames.begin() ;
      libNameIt != avalaiblePluginFileNames.end() ; ++libNameIt)
   {
      // Generate factory for current found plugin
      boost::shared_ptr<CHardwarePluginFactory> pNewFactory (new CHardwarePluginFactory(initialDir));
      if (!pNewFactory->load(*libNameIt))
      {
         std::cout << "Invalid plugin found " << *libNameIt << std::endl;  // TODO : log
         continue;
      }
      m_pluginFactories[*libNameIt] = pNewFactory;
   }
}

const boost::shared_ptr<CHardwarePluginFactory> CHardwarePluginManager::getFactory(const std::string & pluginName)//TODO const
{
   return m_pluginFactories[pluginName];
}


