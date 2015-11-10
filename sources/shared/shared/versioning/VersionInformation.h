#pragma once
#include <shared/Export.h>
#include "Version.h"
#include "ReleaseType.h"
#include <shared/DataContainer.h>

namespace shared { namespace versioning {

   //---------------------------------------------
   ///\brief Class which contains a version and a releaseType
   //---------------------------------------------
   class YADOMS_SHARED_EXPORT CVersionInformation
   {
   public:
      //---------------------------------------------
      ///\brief Constructor
      ///\param [in] version     The version
      ///\param [in] releaseType The releaseType
      //---------------------------------------------
      CVersionInformation(const shared::versioning::CVersion & version, const shared::versioning::EReleaseType & releaseType);

      //---------------------------------------------
      ///\brief Get the version
      ///\return the version
      //---------------------------------------------
      const shared::versioning::CVersion getVersion() const;

      //---------------------------------------------
      ///\brief Get the releaseType
      ///\return the releaseType
      //---------------------------------------------
      const shared::versioning::EReleaseType getReleaseType() const;

      //---------------------------------------------
      ///\brief Serialize the object as string
      ///\return the serialized object as string (json format)
      //---------------------------------------------
      const std::string serialize() const;


      //---------------------------------------------
      ///\brief Return the container
      ///\return the data container
      //---------------------------------------------
      const shared::CDataContainer & getContainer() const;
   private:
      //---------------------------------------------
      ///\brief  The object container
      //---------------------------------------------
      shared::CDataContainer m_container;
   };

} } // namespace shared::versioning
