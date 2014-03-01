#include "stdafx.h"

#include "DecimalParameter.h"

namespace shared { namespace plugin { namespace configuration
{

CDecimalParameter::CDecimalParameter(const std::string& name, const std::string& description, double defaultValue)
   :CSimpleParameter<double>("decimal", name, description, defaultValue)
{
}

boost::shared_ptr<CParameter> CDecimalParameter::clone() const
{
   boost::shared_ptr<CParameter> p(new CDecimalParameter(getName(), getDescription(), m_value));
   return p;
}

} } } // namespace shared::plugin::configuration
