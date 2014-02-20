#include "stdafx.h"

#include "BoolParameter.h"

namespace shared { namespace plugin { namespace configuration
{

CBoolParameter::CBoolParameter(const std::string& name, const std::string& description, bool defaultValue)
   :CSimpleParameter<bool>("bool", name, description, defaultValue)
{
}

boost::shared_ptr<CParameter> CBoolParameter::clone() const
{
   boost::shared_ptr<CParameter> p(new CBoolParameter(getName(), getDescription(), m_value));
   return p;
}

} } } // namespace shared::plugin::configuration
