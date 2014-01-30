#include "stdafx.h"
#include "JsonSerializer.h"
#include "../../Exceptions/InvalidParameterException.hpp"

CJsonSerializer::~CJsonSerializer()
{
}

const std::string CJsonSerializer::serialize(const boost::property_tree::ptree& pt) const
{
   std::ostringstream buf; 
   boost::property_tree::json_parser::write_json (buf, pt, false);
   return buf.str();
}

void CJsonSerializer::unserialize(const std::string& str, boost::property_tree::ptree& pt)
{
   if (str.empty())
      return;

   std::istringstream is(str);
   try
   {
      boost::property_tree::json_parser::read_json(is, pt);
   }
   catch (boost::property_tree::json_parser::json_parser_error& e)
   {
      throw CInvalidParameterException(e.what());   	
   }
}

