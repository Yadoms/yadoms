#pragma once

namespace kernel
{
   namespace io
   {
      //--------------------------------------------------------------
      /// \brief	I/O common methods
      //--------------------------------------------------------------
      class CCommon
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CCommon() {}

         //--------------------------------------------------------------
         /// \brief	Send and receive a message to 1-wire network
         /// \param[in] deviceFile File representing the device
         /// \param[in] cmd Command to send
         /// \param[in] cmdSize Command size
         /// \param[in] answer Buffer to get answer
         /// \param[in] answerSize Answer max size
         /// \throw shared::exception::CException if error accessing device file
         //--------------------------------------------------------------
         static void sendAndReceive(const boost::filesystem::path& deviceFile,
                                    const unsigned char* cmd,
                                    size_t cmdSize,
                                    unsigned char* answer,
                                    size_t answerSize);

         //--------------------------------------------------------------
         /// \brief	Compute the CRC of a 1-wire message
         /// \param[in] byteArray Buffer
         /// \param[in] arraySize Buffer size
         /// \return Computed CRC
         //--------------------------------------------------------------
         static unsigned char crc16(const unsigned char* byteArray,
                                    size_t arraySize);

      private:
         //--------------------------------------------------------------
         /// \brief	Add byte to CRC16 computation
         /// \param[in] crc Current CRC
         /// \param[in] a Byte to add
         /// \return New CRC
         //--------------------------------------------------------------
         static unsigned short crc16AddByte(unsigned short crc,
                                            unsigned char a);
      };
   }
} // namespace kernel::io