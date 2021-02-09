#include "stdafx.h"
#include "SerialPort.h"

namespace hardware
{
   namespace serial
   {
      CSerialPort::CSerialPort(const std::string& lastKnownSerialPortPath,
                               const std::string& description,
                               database::entities::ESerialPortAdapterKind adapterKind,
                               boost::shared_ptr<shared::CDataContainer> adapterParameters,
                               int databaseId)
         : m_lastKnownSerialPortPath(lastKnownSerialPortPath),
           m_description(description),
           m_adapterKind(adapterKind),
           m_adapterParameters(adapterParameters),
           m_id(databaseId)
      {
      }

      int CSerialPort::id() const
      {
         return m_id;
      }

      const std::string& CSerialPort::lastKnownSerialPortPath() const
      {
         return m_lastKnownSerialPortPath;
      }

      database::entities::ESerialPortAdapterKind CSerialPort::adapterKind() const
      {
         return m_adapterKind;
      }

      boost::shared_ptr<const shared::CDataContainer> CSerialPort::adapterParameters() const
      {
         return m_adapterParameters;
      }

      const std::string& CSerialPort::description() const
      {
         return m_description;
      }

      void CSerialPort::setId(int id)
      {
         m_id = id;
      }

      void CSerialPort::setLastKnownSerialPortPath(const std::string& path)
      {
         m_lastKnownSerialPortPath = path;
      }

      void CSerialPort::setAdapterKind(const database::entities::ESerialPortAdapterKind& adapterKind)
      {
         m_adapterKind = adapterKind;
      }

      void CSerialPort::setAdapterParameters(boost::shared_ptr<const shared::CDataContainer> parameters)
      {
         m_adapterParameters = parameters;
      }

      void CSerialPort::setDescription(const std::string& description)
      {
         m_description = description;
      }
   } // namespace serial
} // namespace hardware
