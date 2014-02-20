#include "stdafx.h"

#include "BitsFieldParameter.h"
#include "../../StringExtension.h"

namespace shared { namespace plugin { namespace configuration
{

CBitsFieldParameter::CBitsFieldParameter(const std::string& name, const std::string& description, const Items& items)
   :CParameter("bitsField", name, description), m_defaultValue(items), m_value(items)
{
}

boost::shared_ptr<CParameter> CBitsFieldParameter::clone() const
{
   boost::shared_ptr<CParameter> p(new CBitsFieldParameter(getName(), getDescription(), m_value));
   return p;
}

void CBitsFieldParameter::setValue(const boost::property_tree::ptree& pt)
{
   boost::property_tree::ptree itemsTree = pt.get_child(getName() + ".value");
   for (boost::property_tree::ptree::const_iterator itValue = itemsTree.begin() ; itValue != itemsTree.end() ; itValue++)
      m_value[itValue->first.data()] = itValue->second.data()=="true";
}

void CBitsFieldParameter::build(boost::property_tree::ptree& pt) const
{
   CParameter::build(pt);
   BOOST_FOREACH(Items::value_type item, m_defaultValue)
   {
      pt.put(getName() + ".default." + item.first, item.second);
   }
}

} } } // namespace shared::plugin::configuration
