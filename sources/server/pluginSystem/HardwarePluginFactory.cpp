#include "stdafx.h"
#include "HardwarePluginFactory.h"
#include <shared/plugin/Information/Information.h>
#include <shared/StringExtension.h>

CHardwarePluginFactory::CHardwarePluginFactory(const boost::filesystem::path& libraryPath)
      :m_libraryPath(libraryPath), m_construct(NULL), m_getInformation(NULL), m_getConfigurationSchema(NULL)
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
   m_getInformation = (const shared::plugin::IInformation& (*)())GetFunctionPointer("getInformation");
   m_getConfigurationSchema = (const IHardwarePluginConfigurationSchema& (*)())GetFunctionPointer("getConfigurationSchemaInterface");

   // Check if all non-optional methods are loaded
   if(m_construct == NULL || m_getInformation == NULL)
   {
      // This library is not a valid plugin
      CDynamicLibrary::unload();
      throw CInvalidPluginException(m_libraryPath.string());
   }

   // Log loaded plugin
   YADOMS_LOG(info) << "Hardware plugin loaded : " << getInformation()->toString();
}

void CHardwarePluginFactory::unload()
{
   CDynamicLibrary::unload();
}

IHardwarePlugin* CHardwarePluginFactory::construct() const
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

boost::shared_ptr<const shared::plugin::IInformation> CHardwarePluginFactory::getInformation() const
{
   BOOST_ASSERT(m_getInformation);  // getInformation can not be called if load was unsuccessfully

   // Because library can be unloaded at any time (so memory will be freed), return a copy of informations
   boost::shared_ptr<shared::plugin::IInformation> information(new shared::plugin::CInformation(m_getInformation()));
   return information;
}

std::string CHardwarePluginFactory::getConfigurationSchema() const
{
   if (!m_getConfigurationSchema)
      return CStringExtension::EmptyString; // Plugin has no configuration

   // Because library can be unloaded at any time (so memory will be freed), return a copy of configuration
   return m_getConfigurationSchema().getSchema();
}


//--------------------------------------------------------------
/// Static functions
//-------------------------------------------------------------

boost::shared_ptr<const shared::plugin::IInformation> CHardwarePluginFactory::getInformation(const boost::filesystem::path& libraryPath)
{
   CHardwarePluginFactory plugin(libraryPath);
   return plugin.getInformation();
}

std::string CHardwarePluginFactory::getConfigurationSchema(const boost::filesystem::path& libraryPath)
{
   CHardwarePluginFactory plugin(libraryPath);
   return plugin.getConfigurationSchema();
}
