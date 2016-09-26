#pragma once


class CManufacturers
{
public:
   enum EManufacturerIds
   {
      kMulti_user_Manufacturer_ID = 0x7FF,
   };

   CManufacturers();
   virtual ~CManufacturers();

   static const std::string& name(unsigned int id);
};

//
//   static const CManufacturer Multi_user_Manufacturer_ID(0x7FF, "Multi user Manufacturer ID");


