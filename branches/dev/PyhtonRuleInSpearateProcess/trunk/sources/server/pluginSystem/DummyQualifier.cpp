#include "stdafx.h"
#include "DummyQualifier.h"

namespace pluginSystem
{

CDummyQualifier::CDummyQualifier()
{
}

CDummyQualifier::~CDummyQualifier()
{
}

void CDummyQualifier::signalLoad(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
{
}

void CDummyQualifier::signalUnload(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
{
}

void CDummyQualifier::signalCrash(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation, const std::string& reason)
{
}

bool CDummyQualifier::isSafe(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
{
   return true;
}

int CDummyQualifier::getQualityLevel(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
{
   return kNoEnoughData;
}

} // namespace pluginSystem
