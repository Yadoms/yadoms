#pragma once
#include "ioInterfaces/IHumidity.h"
#include "SingleReadOnlyData.hpp"
#include <shared/StringExtension.h>

namespace owfs
{
   namespace io
   {
      //--------------------------------------------------------------
      /// \brief	OWFS humidity (%) device reader
      //--------------------------------------------------------------
      class CHumidity : public CSingleReadOnlyData<ioInterfaces::IHumidity, double>
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in]	devicePath Device path
         /// \param[in]	valueSubPath OWFS value sub-path
         //--------------------------------------------------------------
         explicit CHumidity(const boost::filesystem::path& devicePath,
                   const boost::filesystem::path& valueSubPath = boost::filesystem::path("humidity"))
            : CSingleReadOnlyData(devicePath, valueSubPath)
         {
         }

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CHumidity()
         {
         }
      };
   }
} // namespace owfs::io