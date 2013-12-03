#include "stdafx.h"
#include "HardwarePluginFactory.h"

CHardwarePluginFactory::CHardwarePluginFactory(const std::string & initialDir)
      :m_initialDir(initialDir), m_construct(NULL), m_getInformation(NULL), m_getDefaultConfiguration(NULL)
{
}

CHardwarePluginFactory::~CHardwarePluginFactory()
{
   CDynamicLibrary::unload();
}


bool CHardwarePluginFactory::load(const std::string & libraryFile)
{
   // Load the plugin library (platform-specific)
   boost::filesystem::path libraryPath = boost::filesystem::path(m_initialDir) / boost::filesystem::path(libraryFile);
   if (!CDynamicLibrary::load(libraryPath.string()))
      return false;

   // Load plugin static methods
   m_construct = (IHardwarePlugin* (*)(void))GetFunctionPointer("construct");
   m_getInformation = (const CHardwarePluginInformation& (*)())GetFunctionPointer("getInformation");
   m_getDefaultConfiguration = (const CHardwarePluginConfiguration& (*)())GetFunctionPointer("getDefaultConfiguration");

   // Check if all non-optional methods are loaded
   if(m_construct == NULL || m_getInformation == NULL)
   {
      // This library is not a valid plugin
      CDynamicLibrary::unload();
      return false;
   }

   // Log loaded plugin
   std::cout << formatPluginInformations() << std::endl;


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
            std::cout << parameter->getName();
            std::cout << " (" << parameter->getDescription() << ")";
            std::cout << " = " << parameter->valueToString() << std::endl;

         // Process specific parameters types
         if (dynamic_cast<CHardwarePluginConfigurationEnumGeneric*>(parameter.get()))//TODO : voir si on ne peut pas mettre les dynamic_cast dans la conf
         {
            // Enum, get all available values
            std::cout << "Available values : ";
            const CHardwarePluginConfigurationEnumGeneric::ValuesNames& values = dynamic_cast<CHardwarePluginConfigurationEnumGeneric*>(parameter.get())->getAvailableValues();
            for (CHardwarePluginConfigurationEnumGeneric::ValuesNames::const_iterator it = values.begin() ; it != values.end() ; ++it)
               std::cout << (*it).second << "|";
            std::cout << std::endl;
         }
      }
      // 3 - L'utilisateur modifie 2 valeurs
      newConf.set("BoolParameter","true");
      newConf.set("EnumParameter","EnumValue3");
      // 4 - L'utilisateur valide la configuration
      //TODO : instancier le plugin en lui passant la conf (à faire dans le superviseur)
      std::string stringAEnregistrerEnBase = newConf.serializeValues();


      // Dans le cas d'une mise à jour de conf d'une instance de plugin existante, il faudrait commencer par
      std::string stringProvenantDeLaBase("{\"BoolParameter\":\"true\",\"EnumParameter\":\"7\",\"Serial port\":\"tty3\"}");
      CHardwarePluginConfiguration newConf2 = defaultConfiguration.get();
      newConf2.unserializeValues(stringProvenantDeLaBase);
      //... et pour l'affichage c'est pareil
      // A la fin, notifier l'instance du plugin de son changement de conf via la fonction updateConfiguration
   }
 
   return true;
}

//--------------------------------------------------------------
/// \brief	    Free library file
//-------------------------------------------------------------
void CHardwarePluginFactory::unload()
{
   CDynamicLibrary::unload();
}


//--------------------------------------------------------------
/// \brief	    Construct a plugin instance (call the contruct library method)
/// \return     a new plugin instance
//-------------------------------------------------------------
IHardwarePlugin * CHardwarePluginFactory::construct()
{
	BOOST_ASSERT(m_construct);  // construct can not be called if load was unsuccessfully
   if(m_construct != NULL)
		return m_construct();
	return NULL;
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

std::string CHardwarePluginFactory::formatPluginInformations() const
{
   std::ostringstream formatedInformations;

   formatedInformations << "Hardware plugin loaded : " << getInformation().getName();
   formatedInformations << " v" << getInformation().getVersion();
   formatedInformations << "[" << getInformation().getReleaseType() << "]";
   formatedInformations << " from " << getInformation().getAuthor();
   formatedInformations << " (" << getInformation().getUrl() << ")";

   return formatedInformations.str();
}
