
#include <sstream>
#include <boost/log/trivial.hpp>

#include "HardwarePluginFactory.h"

CHardwarePluginFactory::CHardwarePluginFactory()
      :m_construct(NULL), m_destruct(NULL), m_getInformation(NULL)
{
}

CHardwarePluginFactory::~CHardwarePluginFactory()
{
   BOOST_ASSERT(m_construct == NULL && m_destruct == NULL && m_getInformation == NULL);
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

      //check if all methods are loaded
      if(m_construct !=NULL && m_destruct!=NULL && m_getInformation != NULL)
      {
          result = TRUE;
          
         //log laoded plugin
         std::ostringstream loadedPluginLog;
         loadedPluginLog << "Hardware plugin loaded : " << getInformation().getName();
         loadedPluginLog << " v" << getInformation().getVersion();
         loadedPluginLog << "[" << getInformation().getReleaseType() << "]";
         loadedPluginLog << " from " << getInformation().getAuthor();
         loadedPluginLog << " (" << getInformation().getUrl() << ")";
         std::cout << loadedPluginLog.str();
   //TODO en attendant que le log marche      BOOST_LOG_TRIVIAL(info) << loadedPluginLog.str();          
      }
      else
      {
          //library is not compatible (do not have construct and destruct exporeted functions)
          //free memory
          unload();
          result = FALSE;
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
   BOOST_ASSERT(m_getInformation);  // getInformation can not be called if load was unsuccessfull
   return m_getInformation();   
}
