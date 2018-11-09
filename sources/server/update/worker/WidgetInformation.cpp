#include "stdafx.h"
#include "WidgetInformation.h"
#include <shared/DataContainer.h>

namespace update
{
   namespace worker
   {
      CWidgetInformation::CWidgetInformation(const boost::filesystem::path& path)
         : m_path(path)
      {
         shared::CDataContainer package;
         try
         {
            const auto packageFile = m_path / "package.json";
            package.deserializeFromFile(packageFile.string());
         }
         catch (std::exception& e)
         {
            throw std::runtime_error(path.stem().string() + std::string(" : Error reading package.json : ") + e.what());
         }
         try
         {
            // Get and check data

            try
            {
               m_type = package.get<std::string>("type");
            }
            catch (std::exception&)
            {
               throw shared::exception::CInvalidParameter("Error reading package.json : widget type not found");
            }

            try
            {
               m_version = shared::versioning::CVersion(package.get<std::string>("version"));
            }
            catch (std::exception&)
            {
               throw shared::exception::CInvalidParameter("Error reading package.json : widget version doesn't match expected SEMVER format (x.x.x)");
            }
         }
         catch (std::exception& e)
         {
            throw std::runtime_error(
               path.stem().string() + std::string(" : Error reading package.json : data not found : ") + e.what());
         }
      }

      CWidgetInformation::~CWidgetInformation()
      {
      }

      const shared::versioning::CVersion& CWidgetInformation::getVersion() const
      {
         return m_version;
      }

      const std::string& CWidgetInformation::getType() const
      {
         return m_type;
      }

      const boost::filesystem::path& CWidgetInformation::getPath() const
      {
         return m_path;
      }
   } // namespace worker
} // namespace update
