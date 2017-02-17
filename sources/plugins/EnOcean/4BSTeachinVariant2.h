#pragma once
#include "manufacturers.h"
#include "profiles/eep.h"

class C4BSTeachinVariant2
{
public:
   explicit C4BSTeachinVariant2(const boost::dynamic_bitset<>& data);
   virtual ~C4BSTeachinVariant2();

   CManufacturers::EManufacturerIds manufacturerId() const;
   C4BSTelegram::EFuncIds funcId() const;
   unsigned int typeId() const;

private:
   const boost::dynamic_bitset<>& m_data;
};

