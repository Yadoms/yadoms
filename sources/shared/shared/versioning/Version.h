#pragma once
#include <shared/Export.h>

namespace shared { namespace versioning {

   //---------------------------------------------
   ///\brief Class which handle and parse versions
   ///       Version format:
   ///         major.minor.build.revision
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
      ///\param [in] major       the major version number
      ///\param [in] minor       the minor version number
      ///\param [in] buildNumber the buildNumber
      ///\param [in] revision    the revision number
      //---------------------------------------------
      CVersion(int major, int minor, int buildNumber = 0, int revision = 0);

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
      ///\brief Set the version values
      ///\param [in] major       the major version number
      ///\param [in] minor       the minor version number
      ///\param [in] buildNumber the buildNumber
      ///\param [in] revision    the revision number
      //---------------------------------------------
      void setValues(int major, int minor, int buildNumber, int revision);

      //---------------------------------------------
      ///\brief  Container for version digits
      //---------------------------------------------
      std::vector<int>    m_versionInfo;
   };

} } // namespace shared::versioning
