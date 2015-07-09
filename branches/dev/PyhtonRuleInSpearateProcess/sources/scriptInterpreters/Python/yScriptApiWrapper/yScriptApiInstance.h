#pragma once

#include <shared/script/yScriptApi/IYScriptApi.h>

//-----------------------------------------------------
///\brief Create the yScriptApi instance
//-----------------------------------------------------
shared::script::yScriptApi::IYScriptApi* createScriptApiInstance(const std::string& yScriptApiAccessorId);
