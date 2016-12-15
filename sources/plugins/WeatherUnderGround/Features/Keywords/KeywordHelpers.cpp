#include "stdafx.h"
#include "KeywordHelpers.h"
#include <boost/lexical_cast.hpp>

bool convertDouble(double& value, const shared::CDataContainer& valueContainer, const std::string& label)
{
   try
   {
      value = valueContainer.get<double>(label);
      return true;
   }
   catch (shared::exception::CException&)
   {
      std::cout << "Value [" + label + "] could not be set" << std::endl;
      return false;
   }
}

bool convertInt(int& value, const shared::CDataContainer& valueContainer, const std::string& label)
{
   try
   {
      value = valueContainer.get<int>(label);
      return true;
   }
   catch (shared::exception::CException&)
   {
      std::cout << "Value [" + label + "] could not be set" << std::endl;
      return false;
   }
}

void convertKmhToMs(std::string& returnString, const shared::CDataContainer& valueContainer, const std::string& label)
{
   double temp = 0;

   if (!convertDouble(temp, valueContainer, label))
      returnString = "--";
   else
      returnString = boost::lexical_cast<std::string>(temp / 3.6);
}

void convertcmTomm(std::string& returnString, const shared::CDataContainer& valueContainer, const std::string& label)
{
   double temp = 0;

   if (!convertDouble(temp, valueContainer, label))
      returnString = "--";
   else
      returnString = boost::lexical_cast<std::string>(temp * 10);
}