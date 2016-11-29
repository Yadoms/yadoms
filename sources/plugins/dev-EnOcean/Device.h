#pragma once
#include "profiles/generated-manufacturers.h"


//--------------------------------------------------------------
/// \brief	An EnOcean device representation
//--------------------------------------------------------------
class CDevice//TODO virer ?
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CDevice(const std::string& id,
           unsigned int manufacturer,
           unsigned int func,
           unsigned int type);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CDevice();

   std::string id() const;
   CManufacturers::EManufacturerIds manufacturer() const;
   unsigned int func() const;
   unsigned int type() const;

private:
   const std::string m_id;
   CManufacturers::EManufacturerIds m_manufacturer;
   unsigned int m_func;
   unsigned int m_type;
};

