#pragma once

namespace shared
{
   namespace versioning
   {
      //---------------------------------------------
      ///\brief Class which handle and parse versions
      ///       Version format: using SEMVER 2.0 http://semver.org/
      //---------------------------------------------
      class CSemVer
      {
      public:
         //---------------------------------------------
         ///\brief empty constructor (version is set to 0.0.0)
         //---------------------------------------------
         CSemVer();

         //---------------------------------------------
         ///\brief Constructor from a string version
         ///\param[in] stringVersion the version as a string. Separators between version digits must be . or ,
         //---------------------------------------------
         explicit CSemVer(const std::string& stringVersion) noexcept(false);

         //---------------------------------------------
         ///\brief Constructor
         ///\param[in] major       the major version number (when you make incompatible API changes,)
         ///\param[in] minor       the minor version number (when you add functionality in a backwards-compatible manner)
         ///\param[in] patch       the patch version number (when you make backwards-compatible bug fixes)
         //---------------------------------------------
         CSemVer(int major, int minor, int patch);

         //---------------------------------------------
         ///\brief Constructor
         ///\param[in] major          the major version number (when you make incompatible API changes,)
         ///\param[in] minor          the minor version number (when you add functionality in a backwards-compatible manner)
         ///\param[in] patch          the patch version number (when you make backwards-compatible bug fixes)
         ///\param[in] prerelease     the prerelease information
         //---------------------------------------------
         CSemVer(int major, int minor, int patch, const std::string& prerelease);

         //---------------------------------------------
         ///\brief Constructor
         ///\param[in] major          the major version number (when you make incompatible API changes,)
         ///\param[in] minor          the minor version number (when you add functionality in a backwards-compatible manner)
         ///\param[in] patch          the patch version number (when you make backwards-compatible bug fixes)
         ///\param[in] prerelease     the prerelease information
         ///\param[in] buildMetadata  the build metadata
         //---------------------------------------------
         CSemVer(int major, int minor, int patch, const std::string& prerelease, const std::string& buildMetadata);

         CSemVer(const CSemVer& rhs) = default;
         CSemVer& operator=(const CSemVer& rhs) = default;
         virtual ~CSemVer() = default;

         //---------------------------------------------
         ///\brief Compare if current Version is strictly less than parameter version
         ///\param[in] rhs the version object to compare
         ///\return  true is current Version is strictly less than parameter version
         //---------------------------------------------
         bool operator<(CSemVer const& rhs) const;

         //---------------------------------------------
         ///\brief Compare if current Version is less or equal to parameter version
         ///\param[in] rhs the version object to compare
         ///\return  true is current Version is less or equal to parameter version
         //---------------------------------------------
         bool operator<=(CSemVer const& rhs) const;

         //---------------------------------------------
         ///\brief Compare if current Version is greater or equal to parameter version
         ///\param[in] rhs the version object to compare
         ///\return  true is current Version is greater or equal to parameter version
         //---------------------------------------------
         bool operator>=(CSemVer const& rhs) const;

         //---------------------------------------------
         ///\brief Compare if current Version is strictly greater than parameter version
         ///\param[in] rhs the version object to compare
         ///\return  true is current Version is strictly greater than parameter version
         //---------------------------------------------
         bool operator>(CSemVer const& rhs) const;

         //---------------------------------------------
         ///\brief Compare if current Version is equal to parameter version
         ///\param[in] rhs the version object to compare
         ///\return  true is current Version is equal to parameter version
         //---------------------------------------------
         bool operator==(CSemVer const& rhs) const;

         //---------------------------------------------
         ///\brief Operator to print out version to a stream
         ///\param [in] oss the target stream
         ///\param [rhs] rhs  the version object to serialize
         ///\return  the target stream
         //---------------------------------------------
         friend std::ostream& operator<<(std::ostream& oss, const CSemVer& rhs);

         //---------------------------------------------
         ///\brief Get the version as a string
         ///\return  the version as a string
         //---------------------------------------------
         std::string toString() const;

         //---------------------------------------------
         ///\brief Get the MAJOR number
         ///\return  the MAJOR number
         //---------------------------------------------
         int getMajor() const;

         //---------------------------------------------
         ///\brief Get the MINOR number
         ///\return  the MINOR number
         //---------------------------------------------
         int getMinor() const;

         //---------------------------------------------
         ///\brief Get the PATCH number
         ///\return  the PATCH number
         //---------------------------------------------
         int getPatch() const;

         //---------------------------------------------
         ///\brief Get the PRERELEASE version
         ///\return  the PRERELEASE version
         //---------------------------------------------
         const std::string& getPrerelease() const;

         //---------------------------------------------
         ///\brief Get the BUILD metadata version
         ///\return  the BUILD metadata version
         //---------------------------------------------
         const std::string& getBuildMetadata() const;

      protected:
         //---------------------------------------------
         ///\brief Compare the current object to another CSemVer
         ///\param[in] rhs the version object to compare
         ///\return  0 if equals, -1 if this<rhs, +1 if this>rhs
         //---------------------------------------------
         int compare(CSemVer const& rhs) const;

      private:

         //---------------------------------------------
         ///\brief Parse a version contained in string
         ///\param[in] version Version as string (SEMVER compatible)
         ///\throw shared::exception::CException in case of bad format
         //---------------------------------------------
         void parse(const std::string& version) noexcept(false);

         //---------------------------------------------
         ///\brief  Major number
         //---------------------------------------------
         int m_major;

         //---------------------------------------------
         ///\brief  Minor number
         //---------------------------------------------
         int m_minor;

         //---------------------------------------------
         ///\brief  Patch number
         //---------------------------------------------
         int m_patch;

         //---------------------------------------------
         ///\brief  Prerelease tag
         //---------------------------------------------
         std::string m_prerelease;

         //---------------------------------------------
         ///\brief  Build metadata
         //---------------------------------------------
         std::string m_buildMetadata;
      };
   }
} // namespace shared::versioning
