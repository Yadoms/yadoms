#pragma once
#include <shared/versioning/SemVer.h>

namespace update
{
   namespace worker
   {
      class IWidgetInformation
      {
      public:
         virtual ~IWidgetInformation()
         {
         }
         
         virtual const std::string& getType() const = 0;

         virtual const shared::versioning::CSemVer& getVersion() const = 0;

         virtual const boost::filesystem::path& getPath() const = 0;
      };
   } // namespace worker
} // namespace update
