#include "stdafx.h"
#include "ProfileHelper.h"


CProfileHelper::CProfileHelper(const std::string& profile)
{
   boost::regex pattern("([[:xdigit:]]{2})-([[:xdigit:]]{2})-([[:xdigit:]]{2})");
   boost::smatch result;
   if (!boost::regex_search(profile, result, pattern))
      throw std::invalid_argument("Unsupported profile " + profile);

   m_rorg = std::stoul(std::string(result[1].first, result[1].second), nullptr, 16);
   m_func = std::stoul(std::string(result[2].first, result[2].second), nullptr, 16);
   m_type = std::stoul(std::string(result[3].first, result[3].second), nullptr, 16);
}

CProfileHelper::CProfileHelper(unsigned rorg,
                               unsigned func,
                               unsigned type):
   m_rorg(rorg),
   m_func(func),
   m_type(type)
{
}

CProfileHelper::~CProfileHelper()
{
}

unsigned int CProfileHelper::rorg() const
{
   return m_rorg;
}

unsigned int CProfileHelper::func() const
{
   return m_func;
}

unsigned int CProfileHelper::type() const
{
   return m_type;
}

std::string CProfileHelper::profile() const
{
   std::ostringstream ss;
   ss << std::uppercase
      << std::setfill('0') << std::setw(2) << std::hex << m_rorg << "-"
      << std::setfill('0') << std::setw(2) << std::hex << m_func << "-"
      << std::setfill('0') << std::setw(2) << std::hex << m_type;
   return ss.str();
}

