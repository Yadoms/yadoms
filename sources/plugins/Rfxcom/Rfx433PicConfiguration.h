#pragma once

#include "IPicConfiguration.h"

class CRfx433PicConfiguration : public IPicConfiguration
{
public:
   CRfx433PicConfiguration();
   virtual ~CRfx433PicConfiguration();

   // IPicConfiguration Implementation
   virtual const std::string& device() const;
   virtual unsigned long deviceType() const;
   virtual unsigned long programAddressStart() const;
   virtual unsigned long programAddressEnd() const;
   virtual unsigned long bytesPerAddr() const;
   virtual unsigned long eraseBlockSize() const;
   virtual unsigned long readBlockSize() const;
   virtual unsigned long writeBlockSize() const;
   // [END] IPicConfiguration Implementation
};

