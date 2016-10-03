#include "stdafx.h"
#include "Device.h"

CDevice::CDevice(unsigned int id,
                 unsigned int manufacturer,
                 unsigned int func,
                 unsigned int type)
   : m_id(id),
     m_manufacturer(CManufacturers::toManufacturerId(manufacturer)),
     m_func(func),
     m_type(type)
{
}

CDevice::~CDevice()
{
}

unsigned int CDevice::id() const
{
   return m_id;
}

CManufacturers::EManufacturerIds CDevice::manufacturer() const
{
   return m_manufacturer;
}

unsigned int CDevice::func() const
{
   return m_func;
}

unsigned int CDevice::type() const
{
   return m_type;
}

