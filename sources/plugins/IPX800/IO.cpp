#include "stdafx.h"
#include "IO.h"

CIO::CIO(const std::string& keywordName,
         const yApi::EKeywordAccessMode& accessMode)
   : m_value(boost::make_shared<yApi::historization::CSwitch>(keywordName, accessMode))
{}

CIO::~CIO()
{}

void CIO::set(bool state)
{
   m_value->set( state );
   std::cout << m_value->getKeyword() << " set to " << state << std::endl;
}

bool CIO::get(void)
{
   bool ret = false;

   m_value->set(ret);
   std::cout << "read " << m_value->getKeyword() << " at " << ret << std::endl;

   return ret;
}

boost::shared_ptr<const yApi::historization::IHistorizable> CIO::historizable() const 
{
  return m_value;
}