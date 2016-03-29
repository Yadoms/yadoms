#include "stdafx.h"
#include "Factory.h"
#include "Information.h"
#include <shared/Log.h>

namespace pluginSystem
{

CFactory::CFactory()
{
}

CFactory::~CFactory()
{
}

boost::shared_ptr<const shared::plugin::information::IInformation> CFactory::createInformation(const boost::filesystem::path& libraryPath) const
{
   return boost::make_shared<pluginSystem::CInformation>(libraryPath.parent_path());
}

} // namespace pluginSystem
