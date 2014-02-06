#include "stdafx.h"

#include "HardwarePluginInstance.h"
#include <shared/HardwarePlugin/Interfaces/IHardwarePlugin.h>
#include <shared/HardwarePlugin/Interfaces/IHardwarePluginInformation.h>


CHardwarePluginInstance::CHardwarePluginInstance(
   const boost::shared_ptr<const CHardwarePluginFactory> plugin,
   const boost::shared_ptr<CHardware> context,
   const boost::shared_ptr<IHardwarePluginQualifier> qualifier)
    : CThreadBase(context->getName()), m_pPlugin(plugin), m_context(context), m_qualifier(qualifier)
{
	BOOST_ASSERT(m_pPlugin);
   m_pPluginInstance.reset(m_pPlugin->construct());
   start();
}

CHardwarePluginInstance::~CHardwarePluginInstance()
{
   stop();
}

void CHardwarePluginInstance::doWork()
{
   BOOST_ASSERT(m_pPluginInstance);
   YADOMS_LOG_CONFIGURE(getName());

   // Loop to restart plugin when crashed
   bool gracefullyExit = false;
   while (!gracefullyExit && m_qualifier->isSafe(m_pPlugin))
   {
      try
      {
         m_pPluginInstance->doWork(m_context->getConfiguration());
         if (getStatus() == kStopping)
         {
            // Normal stop
            gracefullyExit = true;
         }
         else
         {
            // Plugin has stopped without stop requested
            YADOMS_LOG(error) << getName() << " has stopped without stop requested.";
            m_qualifier->signalCrash(m_pPlugin, "Plugin stopped itself");
         }
      }
      catch (boost::thread_interrupted&)
      {
         // End-of-thread exception was not catch by plugin,
         // it's a developer's error, we have to report it
         YADOMS_LOG(error) << getName() << " didn't catch boost::thread_interrupted.";
         m_qualifier->signalCrash(m_pPlugin, "Plugin didn't catch boost::thread_interrupted");
      }
      catch (std::exception& e)
      {
         // Plugin crashed
         YADOMS_LOG(error) << getName() << " crashed in doWork with exception : " << e.what();
         m_qualifier->signalCrash(m_pPlugin, std::string("Plugin crashed in doWork with exception : ") + e.what());
      }
      catch (...)
      {
         // Plugin crashed
         YADOMS_LOG(error) << getName() << " crashed in doWork with unknown exception.";
         m_qualifier->signalCrash(m_pPlugin, "Plugin crashed in doWork with unknown exception");
      }
   }

   if (!gracefullyExit && !m_qualifier->isSafe(m_pPlugin))
   {
      YADOMS_LOG(error) << getName() << " plugin(" << m_pPlugin->getInformation()->getName() << ") was evaluated as not safe and disabled.";
      //TODO : c'est pas le tout de l'dire, mais faut l'faire !
   }
}

void CHardwarePluginInstance::updateConfiguration(const std::string& newConfiguration) const
{
   BOOST_ASSERT(m_pPluginInstance);

   try
   {
      m_pPluginInstance->updateConfiguration(newConfiguration);
   }
   catch (std::exception& e)
   {
      // Plugin crashed
      YADOMS_LOG(error) << getName() << " crashed in updateConfiguration with exception : " << e.what();
      m_qualifier->signalCrash(m_pPlugin, std::string("Plugin crashed in doWork with exception : ") + e.what());
   }
   catch (...)
   {
      // Plugin crashed
      YADOMS_LOG(error) << getName() << " crashed in updateConfiguration with unknown exception.";
      m_qualifier->signalCrash(m_pPlugin, "Plugin crashed in doWork with unknown exception");
   }
}

const std::string CHardwarePluginInstance::getPluginName() const
{
   return m_pPlugin->getLibraryPath().stem().string();
}
