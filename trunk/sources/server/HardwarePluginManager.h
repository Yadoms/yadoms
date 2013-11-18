#pragma once

#include "PluginFactory.hpp"
#include "HardwarePluginInstance.h"
#include "../plugins/hardware/IHardwarePlugin.h"
#include "../plugins/hardware/HardwarePluginInformation.h"
#include <list>

class CHardwarePluginManager
{
public:
   CHardwarePluginManager();
   virtual ~CHardwarePluginManager();

   void buildPluginFactoryList(const std::string & initialDir);
   void freePluginFactoryList();

   CPluginFactory<plugins::IHardwarePlugin, plugins::CHardwarePluginInformation> * getFactory(const std::string & pluginName);

private:
   // \Brief Returns all plugin libraries installed
   std::list<std::string> findAvalaiblePlugins(const std::string & initialDir);

private:
   typedef std::list<CPluginFactory<plugins::IHardwarePlugin, plugins::CHardwarePluginInformation> *> PluginFactoryList;
   PluginFactoryList m_pluginFactories;
};
