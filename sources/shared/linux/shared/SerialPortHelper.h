#pragma once
#include <boost/asio.hpp>

namespace shared
{
   class CSerialPortHelper
   {
   public:
      virtual ~CSerialPortHelper() = default;

      //--------------------------------------------------------------
      /// \brief			Flush serial port buffers (input & output)
      /// \param[in]    port The port to flush
      /// \return       error code
      //--------------------------------------------------------------
      static boost::system::error_code flushSerialPort(boost::asio::serial_port& port);

   private:
      CSerialPortHelper() = default;
   };

} // namespace shared