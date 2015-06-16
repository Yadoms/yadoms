#include "stdafx.h"
#include "Factory.h"
#include "owfs/Engine.h"
#include "kernel/Engine.h"


CFactory::CFactory()
{
}

CFactory::~CFactory()
{
}

boost::shared_ptr<IEngine> CFactory::createEngine(boost::shared_ptr<yApi::IYPluginApi> context, boost::shared_ptr<const IConfiguration> configuration)
{
   // TODO add here other 1-wire supports : linux kernel, Windows
   return boost::make_shared<owfs::CEngine>(context, configuration);
}