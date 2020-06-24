#pragma once
#include "ioInterfaces/ITemperature.h"
#include "SingleReadOnlyData.hpp"
#include <shared/StringExtension.h>

namespace owfs
{
   namespace io
   {
      //--------------------------------------------------------------
      /// \brief	OWFS temperature (°C) device reader
      //--------------------------------------------------------------
      class CTemperature : public CSingleReadOnlyData<ioInterfaces::ITemperature, double>
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in]	devicePath Device path
         /// \param[in]	valueSubPath OWFS value sub-path
         //--------------------------------------------------------------
         explicit CTemperature(const boost::filesystem::path& devicePath, const boost::filesystem::path& valueSubPath = boost::filesystem::path("temperature"))
            : CSingleReadOnlyData(devicePath, valueSubPath) {}

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CTemperature() {}
      };
   }
} // namespace owfs::io