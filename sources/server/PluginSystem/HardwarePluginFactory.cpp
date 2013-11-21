
#include <sstream>
#include <boost/log/trivial.hpp>

#include "HardwarePluginFactory.h"

CHardwarePluginFactory::CHardwarePluginFactory()
{
}

CHardwarePluginFactory::~CHardwarePluginFactory()
{
}


bool CHardwarePluginFactory::load(const std::string & libraryFile)
{
   //load the plugin file
   bool result = CPluginFactory<plugins::IHardwarePlugin>::load(libraryFile);
   if(result)
   {
      //load additional exported methods
      m_getInformation = (const plugins::CHardwarePluginInformation& (*)())GetFunctionPointer("getInformation");

      //check if all methods are loaded
      if(m_getInformation == NULL)
         result = false;

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

   return result;
}


const plugins::CHardwarePluginInformation& CHardwarePluginFactory::getInformation() const
{
   BOOST_ASSERT(m_getInformation);  // getInformation can not be called if load was unsuccessfull
   return m_getInformation();   
}
