#pragma once


class IPicConfiguration
{
public:
   virtual ~IPicConfiguration()
   {
   }

   virtual const std::string& device() const = 0;
   virtual unsigned long deviceType() const = 0;

   virtual unsigned long programAddressStart() const = 0;
   virtual unsigned long programAddressEnd() const = 0;

   virtual unsigned long bytesPerAddr() const = 0;

   virtual unsigned long eraseBlockSize() const = 0;
   virtual unsigned long readBlockSize() const = 0;
   virtual unsigned long writeBlockSize() const = 0;
};

