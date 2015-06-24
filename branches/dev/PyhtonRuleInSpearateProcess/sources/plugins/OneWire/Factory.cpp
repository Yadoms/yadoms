#include "stdafx.h"
#include "Factory.h"
#include "owfs/Engine.h"
#include "kernel/Engine.h"
#include <shared/exception/NotImplemented.hpp>


CFactory::CFactory()
{
}

CFactory::~CFactory()
{
}

boost::shared_ptr<IEngine> CFactory::createEngine(boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<const IConfiguration> configuration)
{
   if (configuration->isOwfsMode())
      return boost::make_shared<owfs::CEngine>(context, configuration);

   if (configuration->isKernelMode())
      return boost::make_shared<kernel::CEngine>(context, configuration);

   throw shared::exception::CNotImplemented("Selected 1-wire mode not supported");
}
