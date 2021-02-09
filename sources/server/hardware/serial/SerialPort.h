#pragma once
#include "ISerialPort.h"

namespace hardware
{
   namespace serial
   {
      class CSerialPort : public ISerialPort
      {
      public:
         CSerialPort(const std::string& lastKnownSerialPortPath,
                     const std::string& description,
                     database::entities::ESerialPortAdapterKind adapterKind,
                     boost::shared_ptr<shared::CDataContainer> adapterParameters =
                        boost::shared_ptr<shared::CDataContainer>(),
                     int databaseId = -1);
         virtual ~CSerialPort() = default;

         // ISerialPort Implementation
         int id() const override;
         void setId(int id) override;
         const std::string& lastKnownSerialPortPath() const override;
         void setLastKnownSerialPortPath(const std::string& path) override;
         database::entities::ESerialPortAdapterKind adapterKind() const override;
         void setAdapterKind(const database::entities::ESerialPortAdapterKind& adapterKind) override;
         boost::shared_ptr<const shared::CDataContainer> adapterParameters() const override;
         void setAdapterParameters(boost::shared_ptr<const shared::CDataContainer> parameters) override;
         const std::string& description() const override;
         void setDescription(const std::string& description) override;
         // [END] ISerialPort Implementation


      private:
         std::string m_lastKnownSerialPortPath;
         std::string m_description;
         database::entities::ESerialPortAdapterKind m_adapterKind;
         boost::shared_ptr<const shared::CDataContainer> m_adapterParameters;
         int m_id;
      };
   } // namespace serial
} // namespace hardware
