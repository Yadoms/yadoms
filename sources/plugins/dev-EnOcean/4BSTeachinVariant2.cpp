// Generated file, don't modify
#include "stdafx.h"
#include "4BSTeachinVariant2.h"
#include "enOceanDescriptors/bitsetHelpers.hpp"
#include "enOceanDescriptors/generated-manufacturers.h"


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

unsigned int C4BSTeachinVariant2::funcId() const
{
   return CManufacturers::toManufacturerId(bitset_extract(m_data, 0, 6));
}

unsigned int C4BSTeachinVariant2::typeId() const
{
   return CManufacturers::toManufacturerId(bitset_extract(m_data, 6, 7));
}
