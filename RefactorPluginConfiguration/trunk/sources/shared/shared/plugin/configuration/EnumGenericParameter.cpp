#include "stdafx.h"

#include "EnumGenericParameter.h"

namespace shared { namespace plugin { namespace configuration
{

CEnumGeneric::CEnumGeneric(const std::string& name, const std::string& description, const ValuesNames& valuesNames)
   :CParameter("enum", name, description), m_valuesNames(valuesNames)
{
}

CEnumGeneric::~CEnumGeneric()
{
}

void CEnumGeneric::build(boost::property_tree::ptree& pt) const
{
   CParameter::build(pt);
   BOOST_FOREACH(ValuesNames::value_type valueName, m_valuesNames)
   {
      pt.put(getName() + ".availableValues." + valueName.second, valueName.first);
   }
}

} } } // namespace shared::plugin::configuration
