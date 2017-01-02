#include "stdafx.h"
#include "yScriptApiImplementation.h"


shared::script::yScriptApi::IYScriptApi* createScriptApiInstance(const std::string& yScriptApiAccessorId)
{
   return new CYScriptApiImplementation(yScriptApiAccessorId);
}


void deleteScriptApiInstance(shared::script::yScriptApi::IYScriptApi* yApi)
{
   delete yApi;
}