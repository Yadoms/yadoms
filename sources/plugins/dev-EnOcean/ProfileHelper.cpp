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

