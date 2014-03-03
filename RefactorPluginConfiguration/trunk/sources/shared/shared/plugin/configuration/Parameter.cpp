#include "stdafx.h"

#include "Parameter.h"


namespace shared { namespace plugin { namespace configuration
{

CParameter::CParameter(const std::string& type, const std::string& name, const std::string& description)
   :m_type (type), m_name(name), m_description(description)
{
}

CParameter::~CParameter()
{
    
}

void CParameter::build(boost::property_tree::ptree& pt) const
{
   pt.put(getName() + ".type", m_type);
   pt.put(getName() + ".description", m_description);
}

} } } // namespace shared::plugin::configuration
