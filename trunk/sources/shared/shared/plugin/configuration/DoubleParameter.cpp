#include "stdafx.h"

#include "DoubleParameter.h"

namespace shared { namespace plugin { namespace configuration
{

CDoubleParameter::CDoubleParameter(const std::string& name, const std::string& description, double defaultValue)
   :CSimpleParameter<double>("double", name, description, defaultValue)
{
}

boost::shared_ptr<CParameter> CDoubleParameter::clone() const
{
   boost::shared_ptr<CParameter> p(new CDoubleParameter(getName(), getDescription(), m_value));
   return p;
}

} } } // namespace shared::plugin::configuration
