// Generated file, don't modify
#include "stdafx.h"
#include "4BSTeachinVariant2.h"
#include "profiles/bitsetHelpers.hpp"


C4BSTeachinVariant2::C4BSTeachinVariant2(const boost::dynamic_bitset<>& data)
   : m_data(data)
{
}

C4BSTeachinVariant2::~C4BSTeachinVariant2()
{
}

CManufacturers::EManufacturerIds C4BSTeachinVariant2::manufacturerId() const
{
   return CManufacturers::toManufacturerId(bitset_extract(m_data, 13, 11));
}

C4BSTelegram::EFuncIds C4BSTeachinVariant2::funcId() const
{
   return C4BSTelegram::toFuncId(bitset_extract(m_data, 0, 6));
}

unsigned int C4BSTeachinVariant2::typeId() const
{
   return bitset_extract(m_data, 6, 7);
}

