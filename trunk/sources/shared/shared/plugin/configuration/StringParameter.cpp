#include "stdafx.h"

#include "StringParameter.h"

namespace shared { namespace plugin { namespace configuration
{

CStringParameter::CStringParameter(const std::string& name,
   const std::string& description, const std::string& defaultValue)
   :CParameter("string", name, description), m_defaultValue(defaultValue), m_value(defaultValue)
{
}

boost::shared_ptr<CParameter> CStringParameter::clone() const
{
   boost::shared_ptr<CParameter> p(new CStringParameter(getName(), getDescription(), m_value));
   return p;
}

const std::string& CStringParameter::get() const
{
   return m_value;
}

void CStringParameter::build(boost::property_tree::ptree& pt) const
{
   CParameter::build(pt);
   pt.put(getName() + ".default", m_defaultValue);
}

void CStringParameter::setValue(const boost::property_tree::ptree& pt)
{
   m_value = pt.get<std::string>(getName() + ".value");
}

} } } // namespace shared::plugin::configuration
