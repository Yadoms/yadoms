#pragma once
#include <shared/Export.h>

namespace shared { namespace versioning {

   //---------------------------------------------
   ///\brief Class which handle and parse versions
   ///       Version format: using SEMVER 2.0 http://semver.org/
   //---------------------------------------------
   class YADOMS_SHARED_EXPORT CVersion
   {
   public:
      //---------------------------------------------
      ///\brief empty contructor (version is set to 0.0.0.0)
      //---------------------------------------------
      CVersion();

      //---------------------------------------------
      ///\brief Contructor from a string version
      ///\param [in] stringVersion the version as a string. Separtors between version digits must be . or ,
      //---------------------------------------------
      explicit CVersion(const std::string & stringVersion);

      //---------------------------------------------
      ///\brief Contructor
      ///\param [in] major       the major version number (when you make incompatible API changes,)
      ///\param [in] minor       the minor version number (when you add functionality in a backwards-compatible manner)
      ///\param [in] revision    the patch version number (when you make backwards-compatible bug fixes)
      //---------------------------------------------
      CVersion(int major, int minor, int patch);

      //---------------------------------------------
      ///\brief Contructor
      ///\param [in] major          the major version number (when you make incompatible API changes,)
      ///\param [in] minor          the minor version number (when you add functionality in a backwards-compatible manner)
      ///\param [in] revision       the patch version number (when you make backwards-compatible bug fixes)
      ///\param [in] prerelease     the prerelease information
      ///\param [in] buildMetaData  the build metadata
      //---------------------------------------------
      CVersion(int major, int minor, int patch, const std::string & prerelease, const std::string & buildMetaData);

      //---------------------------------------------
      ///\brief Copy contructor
      //---------------------------------------------
      CVersion(const CVersion & rhs);

      //---------------------------------------------
      ///\brief Destructor
      //---------------------------------------------
      virtual ~CVersion();

      //---------------------------------------------
      ///\brief Compare if current Version is strictly less than parameter version
      ///\param [in] rhs the version object to compare
      ///\return  true is current Version is strictly less than parameter version
      //---------------------------------------------
      bool operator<(CVersion const& rhs) const;

      //---------------------------------------------
      ///\brief Compare if current Version is less or equal to parameter version
      ///\param [in] rhs the version object to compare
      ///\return  true is current Version is less or equal to parameter version
      //---------------------------------------------
      bool operator<=(CVersion const& rhs) const;

      //---------------------------------------------
      ///\brief Compare if current Version is greater or equal to parameter version
      ///\param [in] rhs the version object to compare
      ///\return  true is current Version is greater or equal to parameter version
      //---------------------------------------------
      bool operator>=(CVersion const& rhs) const;

      //---------------------------------------------
      ///\brief Compare if current Version is strictly greater than parameter version
      ///\param [in] rhs the version object to compare
      ///\return  true is current Version is strictly greater than parameter version
      //---------------------------------------------
      bool operator>(CVersion const& rhs) const;

      //---------------------------------------------
      ///\brief Compare if current Version is equal to parameter version
      ///\param [in] rhs the version object to compare
      ///\return  true is current Version is equal to parameter version
      //---------------------------------------------
      bool operator==(CVersion const& rhs) const;

      //---------------------------------------------
      ///\brief Get the version as a string
      ///\return  the version as a string
      //---------------------------------------------
      const std::string toString() const;

   protected:
      //---------------------------------------------
      ///\brief Compare the current object to another CVersion
      ///\param [in] rhs the version object to compare
      ///\return  0 if equals, -1 if this<rhs, +1 if this>rhs
      //---------------------------------------------
      int compare(CVersion const& rhs) const;

   private:
      //---------------------------------------------
      ///\brief Extract information 
      ///\param [out] major          the major version number
      ///\param [out] minor          the minor version number
      ///\param [out] patch          the patch version number
      ///\param [out] prerelease     the prerelease
      ///\param [out] buildMetadata  the build metadata
      ///\return true if the version is a valid SEMVER format
      //---------------------------------------------
      bool extractValues(int & major, int & minor, int & patch, std::string & prerelease, std::string & buildMetadata) const;

      //---------------------------------------------
      ///\brief  Container for version
      //---------------------------------------------
      std::string    m_version;
   };

} } // namespace shared::versioning
