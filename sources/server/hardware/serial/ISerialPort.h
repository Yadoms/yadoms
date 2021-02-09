#pragma once
#include "shared/DataContainer.h"
#include "database/entities/Entities.h"

namespace hardware
{
   namespace serial
   {
      class ISerialPort
      {
      public:
         virtual ~ISerialPort() = default;

         // Database values
         virtual int id() const = 0;
         virtual void setId(int id) = 0;

         virtual const std::string& lastKnownSerialPortPath() const = 0;
         virtual void setLastKnownSerialPortPath(const std::string& path) = 0;

         virtual database::entities::ESerialPortAdapterKind adapterKind() const = 0;
         virtual void setAdapterKind(const database::entities::ESerialPortAdapterKind& adapterKind) = 0;

         virtual boost::shared_ptr<const shared::CDataContainer> adapterParameters() const = 0;
         virtual void setAdapterParameters(boost::shared_ptr<const shared::CDataContainer> parameters) = 0;

         // From hardware
         virtual const std::string& description() const = 0;
         virtual void setDescription(const std::string& description) = 0;
      };
   } // namespace serial
} // namespace hardware
