#pragma once
#include "IWidgetInformation.h"

namespace update
{
   namespace worker
   {
      class CWidgetInformation : public IWidgetInformation
      {
      public:
         explicit CWidgetInformation(const boost::filesystem::path& path);
         virtual ~CWidgetInformation();

         // IWidgetInformation Implementation
         const std::string& getType() const override;
         const shared::versioning::CVersion& getVersion() const override;
         const boost::filesystem::path& getPath() const override;
         // [END] IWidgetInformation Implementation

      private:
         const boost::filesystem::path m_path;
         std::string m_type;
         shared::versioning::CVersion m_version;
      };
   } // namespace worker
} // namespace update
