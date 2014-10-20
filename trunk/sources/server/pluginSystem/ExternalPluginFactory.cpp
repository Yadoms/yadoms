#include "stdafx.h"
#include "ExternalPluginFactory.h"
#include "pluginSystem/Information.h"
#include <shared/StringExtension.h>
#include <shared/exception/Exception.hpp>

namespace pluginSystem
{

CExternalPluginFactory::CExternalPluginFactory(const boost::filesystem::path& libraryPath)
      :m_libraryPath(libraryPath), m_construct(NULL)
{
   load();
}

CExternalPluginFactory::~CExternalPluginFactory()
{
   unload();
}

void CExternalPluginFactory::load()
{
   // Load the plugin library (platform-specific)
   if (!shared::CDynamicLibrary::load(m_libraryPath.string()))
      throw CInvalidPluginException(m_libraryPath.string());

   // Load plugin static methods
   m_construct = (shared::plugin::IPlugin* (*)(void))GetFunctionPointer("construct");

   // Check if all non-optional methods are loaded
   if(m_construct == NULL)
   {
      // This library is not a valid plugin
      shared::CDynamicLibrary::unload();
      throw CInvalidPluginException(m_libraryPath.string());
   }

   // Check if functions throw
   try
   {
      getInformation();
      delete construct();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Plugin " << m_libraryPath << " crashed at loading, considered as not valid plugin.";
      shared::CDynamicLibrary::unload();
      throw CInvalidPluginException(m_libraryPath.string());
   }

   // Log loaded plugin
   YADOMS_LOG(info) << "Plugin loaded : " << getInformation()->toString();
}

void CExternalPluginFactory::unload()
{
   shared::CDynamicLibrary::unload();
}

shared::plugin::IPlugin* CExternalPluginFactory::construct() const
{
	BOOST_ASSERT(m_construct);  // construct can not be called if load was unsuccessfully
   if(m_construct != NULL)
		return m_construct();
	return NULL;
}

const boost::filesystem::path& CExternalPluginFactory::getLibraryPath() const
{
   return m_libraryPath;
}

boost::shared_ptr<const shared::plugin::information::IInformation> CExternalPluginFactory::getInformation() const
{
   // Because library can be unloaded at any time (so memory will be freed), return a copy of informations
   try
   {
      boost::shared_ptr<shared::plugin::information::IInformation> information(
         new pluginSystem::CInformation(getLibraryPath().parent_path()));
      return information;
   }
   catch(shared::exception::CException& e)
   {
      YADOMS_LOG(error) << "Error getting plugin " << shared::CDynamicLibrary::ToLibName(m_libraryPath.string()) << " information : " << e.what();
      throw shared::exception::CException("Error getting plugin information");
   }
}


//--------------------------------------------------------------
/// Static functions
//-------------------------------------------------------------

boost::shared_ptr<const shared::plugin::information::IInformation> CExternalPluginFactory::getInformation(const boost::filesystem::path& libraryPath)
{
   CExternalPluginFactory plugin(libraryPath);
   return plugin.getInformation();
}

} // namespace pluginSystem
