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


   { // TODO bloc à virer quand le code sera récupéré par l'IHM
      // TODO : exemple de récupération du nécessaire pour création d'une nouvelle instance de plugin par l'IHM
      // 1 - L'IHM récupère la structure de la conf avec les valeurs par défaut
      const boost::optional<const CHardwarePluginConfiguration&> defaultConfiguration(getDefaultConfiguration());
      // Ici, defaultConfiguration.is_initialized() indique si le plugin contient une configuration
      // Dans notre exemple, on va se contenter de vérifier pas assertion qu'il en contient une
      BOOST_ASSERT(defaultConfiguration);
      // 2 - Copie de la configuration en local
      CHardwarePluginConfiguration newConf = defaultConfiguration.get();
      for (CHardwarePluginConfiguration::CHardwarePluginConfigurationMap::const_iterator it = newConf.getMap().begin() ;
         it != newConf.getMap().end() ; ++it)
      {
         boost::shared_ptr<CHardwarePluginConfigurationParameter> parameter = (*it).second;

         // Get parameter name, description and value (as string)
            YADOMS_LOG(debug) << parameter->getName() << " (" << parameter->getDescription() << ")" << " = " << parameter->valueToString();

         // Process specific parameters types
         if (dynamic_cast<CHardwarePluginConfigurationEnumGeneric*>(parameter.get()))//TODO : voir si on ne peut pas mettre les dynamic_cast dans la conf
         {
            // Enum, get all available values
            std::ostringstream os;
            os << "Available values : ";
            boost::shared_ptr<std::vector<std::string> > values = dynamic_cast<CHardwarePluginConfigurationEnumGeneric*>(parameter.get())->getAvailableValues();
            BOOST_FOREACH(std::string value, *values)
               os << value << "|";
            YADOMS_LOG(debug) << os.str();
         }
      }
      // 3 - L'utilisateur modifie 2 valeurs
      newConf.set("BoolParameter","true");
      newConf.set("EnumParameter","EnumValue3");
      // 4 - L'utilisateur valide la configuration
      //TODO : instancier le plugin en lui passant la conf (à faire dans le superviseur)
      std::string stringAEnregistrerEnBase = newConf.serializeValues();


      // Dans le cas d'une mise à jour de conf d'une instance de plugin existante, il faudrait commencer par
      std::string stringProvenantDeLaBase("{\"BoolParameter\":\"true\",\"EnumParameter\":\"7\",\"Serial port\":\"COM5\"}");
      CHardwarePluginConfiguration newConf2 = defaultConfiguration.get();
      newConf2.unserializeValues(stringProvenantDeLaBase);
      //... et pour l'affichage c'est pareil
      // A la fin, notifier l'instance du plugin de son changement de conf via la fonction updateConfiguration
   }
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
   if (m_getDefaultConfiguration)
      defaultConfiguration = m_getDefaultConfiguration();
   return defaultConfiguration;
}

CHardwarePluginInformation CHardwarePluginFactory::getInformation(const boost::filesystem::path& libraryPath)
{
   CHardwarePluginFactory plugin(libraryPath);
   return plugin.getInformation();
}
