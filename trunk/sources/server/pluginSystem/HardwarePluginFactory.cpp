#include "stdafx.h"
#include "HardwarePluginFactory.h"

CHardwarePluginFactory::CHardwarePluginFactory()
      :m_construct(NULL), m_destruct(NULL), m_getInformation(NULL), m_getDefaultConfiguration(NULL)
{
}

CHardwarePluginFactory::~CHardwarePluginFactory()
{
   BOOST_ASSERT(m_construct == NULL && m_destruct == NULL && m_getInformation == NULL && m_getDefaultConfiguration == NULL);
}


bool CHardwarePluginFactory::load(const std::string & libraryFile)
{
   //load the plugin file
   bool result = CDynamicLibrary::load(libraryFile);
   if(result)
   {
   
      //load additional exported methods
      m_construct = (IHardwarePlugin* (*)(void))GetFunctionPointer("construct");
      m_destruct =  (void (*)(IHardwarePlugin*))GetFunctionPointer("destruct"); 
      m_getInformation = (const CHardwarePluginInformation& (*)())GetFunctionPointer("getInformation");
      m_getDefaultConfiguration = (const CHardwarePluginConfiguration& (*)())GetFunctionPointer("getDefaultConfiguration");

      //check if all methods are loaded
      if(m_construct !=NULL && m_destruct!=NULL && m_getInformation != NULL && m_getDefaultConfiguration != NULL)
      {
          result = true;
          
         //log loaded plugin
         std::ostringstream loadedPluginLog;
         loadedPluginLog << "Hardware plugin loaded : " << getInformation().getName();
         loadedPluginLog << " v" << getInformation().getVersion();
         loadedPluginLog << "[" << getInformation().getReleaseType() << "]";
         loadedPluginLog << " from " << getInformation().getAuthor();
         loadedPluginLog << " (" << getInformation().getUrl() << ")";
         std::cout << loadedPluginLog.str() << std::endl;
   //TODO en attendant que le log marche      BOOST_LOG_TRIVIAL(info) << loadedPluginLog.str();          

         // TODO : exemple de récupération du nécessaire pour création d'une nouvelle instance de plugin par l'IHM
         // 1 - L'IHM récupère la structure de la conf avec les valeurs par défaut
         CHardwarePluginConfiguration newConf = getDefaultConfiguration();
         for (CHardwarePluginConfiguration::CHardwarePluginConfigurationMap::const_iterator it = newConf.getMap().begin() ;
            it != newConf.getMap().end() ; ++it)
         {
            boost::shared_ptr<CHardwarePluginConfigurationParameter> parameter = (*it).second;

            // Get parameter name and value (as string)
            std::cout << parameter->getName();
            std::cout << " (" << parameter->getDescription() << ")";
            std::cout << " = " << parameter->valueToString() << std::endl;
            
            // Process specific parameters types
            if (dynamic_cast<CHardwarePluginConfigurationEnumGeneric*>(parameter.get()))
            {
               // Enum, get all available values
               std::cout << "Available values : ";
               const CHardwarePluginConfigurationEnumGeneric::ValuesNames& values = dynamic_cast<CHardwarePluginConfigurationEnumGeneric*>(parameter.get())->getAvailableValues();
               for (CHardwarePluginConfigurationEnumGeneric::ValuesNames::const_iterator it = values.begin() ; it != values.end() ; ++it)
                  std::cout << (*it).second << "|";
               std::cout << std::endl;
            }
         }
         // 2 - L'utilisateur modifie 2 valeurs
         newConf.set("BoolParameter","true");
         newConf.set("EnumParameter","EnumValue3");
         // 3 - L'utilisateur valide la configuration
         //TODO : instancier le plugin en lui passant la conf (à faire dans le superviseur)
         std::string stringAEnregistrerEnBase = newConf.serializeValues();


         // Dans le cas d'une mise à jour de conf d'une instance de plugin existante, il faudrait commencer par
         std::string stringProvenantDeLaBase("{\"BoolParameter\":\"true\",\"EnumParameter\":\"7\",\"Serial port\":\"tty3\"}");
         CHardwarePluginConfiguration newConf2 = getDefaultConfiguration();
         newConf2.unserializeValues(stringProvenantDeLaBase);
         //... et pour l'affichage c'est pareil
         // A la fin, notifier l'instance du plugin de son changement de conf via la fonction updateConfiguration
 

      }
      else
      {
          //library is not compatible (do not have construct and destruct exported functions)
          //free memory
          unload();
          result = false;
      }   


   }

   return result;
}

//--------------------------------------------------------------
/// \brief	    Free library file
//-------------------------------------------------------------
void CHardwarePluginFactory::unload()
{
   m_construct =NULL;
   m_destruct=NULL;
   m_getInformation= NULL;
   m_getDefaultConfiguration=NULL;
}


//--------------------------------------------------------------
/// \brief	    Construct a plugin instance (call the contruct library method)
/// \return     a new plugin instance
//-------------------------------------------------------------
IHardwarePlugin * CHardwarePluginFactory::construct()
{
	BOOST_ASSERT(m_construct);  // construct can not be called if load was unsuccessfull
   if(m_construct != NULL)
		return m_construct();
	return NULL;
}

//--------------------------------------------------------------
/// \brief	    Destruct a plugin instance
/// \param [in] item: the plugin to destruct
//-------------------------------------------------------------
void CHardwarePluginFactory::destruct(IHardwarePlugin* item)
{
	BOOST_ASSERT(m_destruct);  // destruct can not be called if load was unsuccessfull
	if(m_destruct != NULL)
		m_destruct(item);
}

const CHardwarePluginInformation& CHardwarePluginFactory::getInformation() const
{
   BOOST_ASSERT(m_getInformation);  // getInformation can not be called if load was unsuccessfully
   return m_getInformation();   
}

const CHardwarePluginConfiguration& CHardwarePluginFactory::getDefaultConfiguration() const//TODO : à renommer ? en getDefaultConfiguration
{
   BOOST_ASSERT(m_getDefaultConfiguration);  // getDefaultConfiguration can not be called if load was unsuccessfully
   return m_getDefaultConfiguration();   
}