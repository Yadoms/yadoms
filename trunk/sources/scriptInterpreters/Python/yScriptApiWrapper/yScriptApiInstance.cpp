#include "stdafx.h"
#include "yScriptApiImplementation.h"
#include <shared/currentTime/Provider.h>
#include <shared/currentTime/Local.h>


shared::currentTime::Provider timeProvider(boost::make_shared<shared::currentTime::Local>());

shared::script::yScriptApi::IYScriptApi* createScriptApiInstance(const std::string& yScriptApiAccessorId)
{
   return new CYScriptApiImplementation(yScriptApiAccessorId);
}


void deleteScriptApiInstance(shared::script::yScriptApi::IYScriptApi* yApi)
{
   delete yApi;
}