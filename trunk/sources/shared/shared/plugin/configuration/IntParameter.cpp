#include "stdafx.h"

#include "IntParameter.h"

namespace shared { namespace plugin { namespace configuration
{

CIntParameter::CIntParameter(const std::string& name, const std::string& description, int defaultValue)
   :CSimpleParameter<int>("int", name, description, defaultValue)
{
}

boost::shared_ptr<CParameter> CIntParameter::clone() const
{
   boost::shared_ptr<CParameter> p(new CIntParameter(getName(), getDescription(), m_value));
   return p;
}

} } } // namespace shared::plugin::configuration
