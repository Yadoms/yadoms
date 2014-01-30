#include "stdafx.h"
#include "PTreeToJsonSerializer.h"
#include "../Exceptions/InvalidParameterException.hpp"

PtreeToJsonSerializer::~PtreeToJsonSerializer()
{
}

const std::string PtreeToJsonSerializer::serialize(const boost::property_tree::ptree& pt) const
{
   std::ostringstream buf; 
   boost::property_tree::json_parser::write_json (buf, pt, false);
   return buf.str();
}

void PtreeToJsonSerializer::unserialize(const std::string& str, boost::property_tree::ptree& pt)
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

