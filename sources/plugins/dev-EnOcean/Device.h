#pragma once
#include "enOceanDescriptors/generated-manufacturers.h"


//--------------------------------------------------------------
/// \brief	An EnOcean device representation
//--------------------------------------------------------------
class CDevice
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   //--------------------------------------------------------------
   CDevice(unsigned int id,
           unsigned int manufacturer,
           unsigned int func,
           unsigned int type);

   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~CDevice();

   unsigned int id() const;
   CManufacturers::EManufacturerIds manufacturer() const;
   unsigned int func() const;
   unsigned int type() const;

private:
   unsigned int m_id;
   CManufacturers::EManufacturerIds m_manufacturer;
   unsigned int m_func;
   unsigned int m_type;
};

