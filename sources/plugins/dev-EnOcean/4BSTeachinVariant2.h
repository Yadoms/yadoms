// Generated file, don't modify
#pragma once
#include "enOceanDescriptors/generated-manufacturers.h"

class C4BSTeachinVariant2
{
public:
   explicit C4BSTeachinVariant2(const boost::dynamic_bitset<>& data);
   virtual ~C4BSTeachinVariant2();
public:
   CManufacturers::EManufacturerIds manufacturer() const;
   unsigned int funcId() const;
   unsigned int typeId() const;
private:
   const boost::dynamic_bitset<>& m_data;
};
