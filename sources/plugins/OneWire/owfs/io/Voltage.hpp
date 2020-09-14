#pragma once
#include "ioInterfaces/IVoltage.h"
#include "SingleReadOnlyData.hpp"
#include <shared/StringExtension.h>

namespace owfs
{
   namespace io
   {
      //--------------------------------------------------------------
      /// \brief	OWFS voltage (V) device reader
      //--------------------------------------------------------------
      class CVoltage : public CSingleReadOnlyData<ioInterfaces::IVoltage, double>
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in]	devicePath Device path
         /// \param[in]	valueSubPath OWFS value sub-path
         //--------------------------------------------------------------
         explicit CVoltage(const boost::filesystem::path& devicePath,
                  const boost::filesystem::path& valueSubPath = boost::filesystem::path("voltage"))
            : CSingleReadOnlyData(devicePath, valueSubPath)
         {
         }

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CVoltage()
         {
         }
      };
   }
} // namespace owfs::io