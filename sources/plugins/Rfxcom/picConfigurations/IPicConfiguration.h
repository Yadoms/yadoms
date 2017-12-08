#pragma once

namespace picConfigurations
{
   class IPicConfiguration
   {
   public:
      enum EDeviceType
      {
         kUnknown = 0,
         kPic24F = 1,
         kPic24Fj = 2
      };

      virtual ~IPicConfiguration()
      {
      }

      virtual const std::string& device() const = 0;
      virtual EDeviceType deviceType() const = 0;

      virtual unsigned long programAddressStart() const = 0;
      virtual unsigned long programAddressEnd() const = 0;

      virtual unsigned long bytesPerAddr() const = 0;

      virtual unsigned long eraseBlockSize() const = 0;
      virtual unsigned long readBlockSize() const = 0;
      virtual unsigned long writeBlockSize() const = 0;
   };
} // namespace picConfigurations
