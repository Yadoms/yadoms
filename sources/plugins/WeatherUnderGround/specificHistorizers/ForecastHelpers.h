#pragma once

#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/plugin/yPluginApi/historization/SingleHistorizableData.hpp>

namespace weatherunderground { namespace helper
{

//-----------------------------------------------------
///\brief  All periods
//-----------------------------------------------------   
DECLARE_ENUM_HEADER(EPeriod,
    ((NoPeriod))
    ((Day))
    ((Hour))
);

//--------------------------------------------------------------
/// \brief	The map of returned icons from the web site
//--------------------------------------------------------------
typedef std::map<std::string, unsigned int> EnumValuesNames;

extern const EnumValuesNames EEnumTypeNames;

}}