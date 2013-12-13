#include "stdafx.h"
#include "HardwarePluginFactory.h"

CHardwarePluginFactory::CHardwarePluginFactory(const boost::filesystem::path& libraryPath)
      :m_libraryPath(libraryPath), m_construct(NULL), m_getInformation(NULL), m_getDefaultConfiguration(NULL)
{
   load();
}

CHardwarePluginFactory::~CHardwarePluginFactory()
{
   unload();
}


void CHardwarePluginFactory::load()
{
   // Load the plugin library (platform-specific)
   if (!CDynamicLibrary::load(m_libraryPath.string()))
      throw CInvalidPluginException(m_libraryPath.string());

   // Load plugin static methods
   m_construct = (IHardwarePlugin* (*)(void))GetFunctionPointer("construct");
   m_getInformation = (const CHardwarePluginInformation& (*)())GetFunctionPointer("getInformation");
   m_getDefaultConfiguration = (const CHardwarePluginConfiguration& (*)())GetFunctionPointer("getDefaultConfiguration");

   // Check if all non-optional methods are loaded
   if(m_construct == NULL || m_getInformation == NULL)
   {
      // This library is not a valid plugin
      CDynamicLibrary::unload();
      throw CInvalidPluginException(m_libraryPath.string());
   }

   // Log loaded plugin
   YADOMS_LOG(info) << "Hardware plugin loaded : " << getInformation().toString();
}

void CHardwarePluginFactory::unload()
{
   CDynamicLibrary::unload();
}


IHardwarePlugin * CHardwarePluginFactory::construct() const//TODO passer en shared_ptr
{
	BOOST_ASSERT(m_construct);  // construct can not be called if load was unsuccessfully
   if(m_construct != NULL)
		return m_construct();
	return NULL;
}

const boost::filesystem::path& CHardwarePluginFactory::getLibraryPath() const
{
   return m_libraryPath;
}

const CHardwarePluginInformation& CHardwarePluginFactory::getInformation() const
{
   BOOST_ASSERT(m_getInformation);  // getInformation can not be called if load was unsuccessfully
   return m_getInformation();   
}

const boost::optional<const CHardwarePluginConfiguration&> CHardwarePluginFactory::getDefaultConfiguration() const
{
   boost::optional<const CHardwarePluginConfiguration&> defaultConfiguration;
   if (!m_getDefaultConfiguration)
      return boost::optional<const CHardwarePluginConfiguration&>(); // Plugin has no configuration

   return boost::optional<const CHardwarePluginConfiguration&> (m_getDefaultConfiguration());
}


//--------------------------------------------------------------
/// Static functions
//-------------------------------------------------------------

CHardwarePluginInformation CHardwarePluginFactory::getInformation(const boost::filesystem::path& libraryPath)
{
   CHardwarePluginFactory plugin(libraryPath);
   return plugin.getInformation();
}

const boost::optional<const CHardwarePluginConfiguration&> CHardwarePluginFactory::getDefaultConfiguration(const boost::filesystem::path& libraryPath)
{
   CHardwarePluginFactory plugin(libraryPath);
   return plugin.getDefaultConfiguration();
}
