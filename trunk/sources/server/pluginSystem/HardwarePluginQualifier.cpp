#include "stdafx.h"
#include "HardwarePluginQualifier.h"

CHardwarePluginQualifier::CHardwarePluginQualifier()
{
}

CHardwarePluginQualifier::~CHardwarePluginQualifier()
{
}

void CHardwarePluginQualifier::signalLoad(const boost::shared_ptr<const CHardwarePluginFactory> plugin)
{
   //TODO
}

void CHardwarePluginQualifier::signalUnload(const boost::shared_ptr<const CHardwarePluginFactory> plugin)
{
   //TODO
}

void CHardwarePluginQualifier::signalCrash(const boost::shared_ptr<const CHardwarePluginFactory> plugin, const std::string& exceptionName)
{
   //TODO
}

bool CHardwarePluginQualifier::isSafe(const boost::shared_ptr<const CHardwarePluginFactory> plugin)
{
   //TODO
   return true;
}

int CHardwarePluginQualifier::getQualityLevel(const boost::shared_ptr<const CHardwarePluginFactory> plugin)
{
   //TODO
   return 100;
}
