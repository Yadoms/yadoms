#pragma once

#include <shared/script/yScriptApi/IYScriptApi.h>

//-----------------------------------------------------
///\brief Create the yScriptApi instance
///\param[in] yScriptApiAccessorId The script API accessor ID used to dialog with Yadoms-side API implementation
///\return IYScriptApi pointer on the created instance
//-----------------------------------------------------
shared::script::yScriptApi::IYScriptApi* createScriptApiInstance(const std::string& yScriptApiAccessorId);

//-----------------------------------------------------
///\brief Delete the yScriptApi instance
///\param[in] pointer on the IYScriptApi pointer
//-----------------------------------------------------
void deleteScriptApiInstance(shared::script::yScriptApi::IYScriptApi* yApi);