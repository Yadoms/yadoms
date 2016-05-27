#pragma once

namespace owfs
{
   namespace io
   {
      //--------------------------------------------------------------
      /// \brief	OWFS I/O common methods
      //--------------------------------------------------------------
      class CCommon
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CCommon() {}

         //--------------------------------------------------------------
         /// \brief	Read value
         /// \param[in] valuePath The value path
         /// \return read value, or empty string if not found or error
         //--------------------------------------------------------------
         static std::string read(const boost::filesystem::path& valuePath);

         //--------------------------------------------------------------
         /// \brief	Write value
         /// \param[in] valuePath The value path
         /// \param[in] value The value to write
         //--------------------------------------------------------------
         static void write(const boost::filesystem::path& valuePath, const std::string& value);
      };
   }
} // namespace owfs::io
