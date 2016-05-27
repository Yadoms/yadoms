#pragma once
#include "ioInterfaces/IMultiCounter.h"

namespace owfs
{
   namespace io
   {
      //--------------------------------------------------------------
      /// \brief	OWFS multi counter device read/write class
      //--------------------------------------------------------------
      class CMultiCounter : public ioInterfaces::IMultiCounter
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in]	devicePath Device path
         //--------------------------------------------------------------
         explicit CMultiCounter(const boost::filesystem::path& devicePath);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CMultiCounter();

      protected:
         // ioInterfaces::IMultiCounter implementation
         virtual int read(unsigned char unit) const;
         // [END] ioInterfaces::IMultiCounter implementation


         //--------------------------------------------------------------
         /// \brief	Read value
         /// \param[in]	filenamePrefix filename prefix (without unit) to use to retrieve data
         /// \param[in] unit Unit number (0..n)
         /// \return Counter value as string, directly read from file
         //--------------------------------------------------------------
         std::string read(const std::string& filenamePrefix, unsigned char unit) const;

      private:
         //--------------------------------------------------------------
         /// \brief	OWFS device path
         //--------------------------------------------------------------
         const boost::filesystem::path m_devicePath;
      };
   }
} // namespace owfs::io