#pragma once

#include "IPicConfiguration.h"

namespace picConfigurations
{
   class CRfxLan : public IPicConfiguration
   {
   public:
      CRfxLan();
      virtual ~CRfxLan();

      // IPicConfiguration Implementation
      const std::string& device() const override;
      EDeviceType deviceType() const override;
      unsigned long programAddressStart() const override;
      unsigned long programAddressEnd() const override;
      unsigned long bytesPerAddr() const override;
      unsigned long eraseBlockSize() const override;
      unsigned long readBlockSize() const override;
      unsigned long writeBlockSize() const override;
      // [END] IPicConfiguration Implementation
   };
} // namespace picConfigurations


