#pragma once
#include <shared/script/yScriptApi/IYScriptApi.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <shared/script/yScriptApi/WaitForEventResult.h>


//-----------------------------------------------------
///\brief                              Create the yScriptApi instance
///\param[in] yScriptApiAccessorId     The script API accessor ID used to dialog with
///                                    Yadoms-side API implementation (provided by Yadoms at script startup)
///\return IYScriptApi                 Pointer on the created instance
//-----------------------------------------------------
shared::script::yScriptApi::IYScriptApi* createScriptApiInstance(const std::string& yScriptApiAccessorId);

//-----------------------------------------------------
///\brief                              Delete the yScriptApi instance
///\param[in] yApi                     Pointer on the IYScriptApi pointer
//-----------------------------------------------------
void deleteScriptApiInstance(shared::script::yScriptApi::IYScriptApi* yApi);