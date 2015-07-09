#include "stdafx.h"
#include "yScriptApiImplementation.h"


shared::script::yScriptApi::IYScriptApi* createScriptApiInstance(const std::string& yScriptApiAccessorId)
{
   return new CYScriptApiImplementation(yScriptApiAccessorId); // TODO prévoir le delete
}
