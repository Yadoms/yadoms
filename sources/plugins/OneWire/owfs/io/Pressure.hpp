#pragma once
#include "ioInterfaces/IPressure.h"
#include "SingleReadOnlyData.hpp"
#include <shared/StringExtension.h>

namespace owfs
{
   namespace io
   {
      //--------------------------------------------------------------
      /// \brief	OWFS pressure (hPa) device reader
      //--------------------------------------------------------------
      class CPressure : public CSingleReadOnlyData<ioInterfaces::IPressure, double>
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in]	devicePath Device path
         /// \param[in]	valueSubPath OWFS value sub-path
         //--------------------------------------------------------------
         explicit CPressure(const boost::filesystem::path& devicePath,
                   const boost::filesystem::path& valueSubPath = boost::filesystem::path("pressure"))
            : CSingleReadOnlyData(devicePath, valueSubPath)
         {
         }

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CPressure()
         {
         }
      };
   }
} // namespace owfs::io