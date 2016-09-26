#include "stdafx.h"
#include "Manufacturers.h"


const std::map<int, std::string> ManufacturersMap = boost::assign::map_list_of(CManufacturers::kMulti_user_Manufacturer_ID, "Multi user Manufacturer ID");


CManufacturers::CManufacturers()
{
}

CManufacturers::~CManufacturers()
{
}

const std::string& CManufacturers::name(unsigned int id)
{
   return ManufacturersMap.at(id);
}