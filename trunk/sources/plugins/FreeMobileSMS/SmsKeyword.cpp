#include "stdafx.h"
#include "SmsKeyword.h"
#include <shared/plugin/yPluginApi/StandardCapacity.h>

CSmsKeyword::CSmsKeyword(const std::string & keywordName)
   :yApi::historization::CSingleHistorizableData < std::string >(keywordName, yApi::CStandardCapacities::Message, yApi::EKeywordAccessMode::kGetSet)
{

}

CSmsKeyword::~CSmsKeyword()
{

}
