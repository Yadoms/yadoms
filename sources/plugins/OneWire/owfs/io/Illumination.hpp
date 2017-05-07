#pragma once
#include "ioInterfaces/IIllumination.h"
#include "SingleReadOnlyData.hpp"
#include <shared/StringExtension.h>

namespace owfs
{
   namespace io
   {
      //--------------------------------------------------------------
      /// \brief	OWFS illumination (lux) device reader
      //--------------------------------------------------------------
      class CIllumination : public CSingleReadOnlyData<ioInterfaces::IIllumination, double>
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in]	devicePath Device path
         /// \param[in]	valueSubPath OWFS value sub-path
         //--------------------------------------------------------------
         CIllumination(const boost::filesystem::path& devicePath,
                       const boost::filesystem::path& valueSubPath = boost::filesystem::path("illuminance"))
            : CSingleReadOnlyData(devicePath, valueSubPath)
         {
         }

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CIllumination()
         {
         }
      };
   }
} // namespace owfs::io
