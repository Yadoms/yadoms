#pragma once
#include <shared/Export.h>
#include "Version.h"
#include <shared/DataContainer.h>

namespace shared
{
   namespace versioning
   {
      //---------------------------------------------
      ///\brief Class which contains a version and a releaseType
      //---------------------------------------------
      class YADOMS_SHARED_EXPORT CVersionInformation
      {
      public:
         //---------------------------------------------
         ///\brief Constructor
         ///\param [in] version     The version
         //---------------------------------------------
         explicit CVersionInformation(const shared::versioning::CVersion& version);

         //---------------------------------------------
         ///\brief Get the version
         ///\return the version
         //---------------------------------------------
         CVersion getVersion() const;

         //---------------------------------------------
         ///\brief Serialize the object as string
         ///\return the serialized object as string (json format)
         //---------------------------------------------
         std::string serialize() const;

         //---------------------------------------------
         ///\brief Return the container
         ///\return the data container
         //---------------------------------------------
         const CDataContainer& getContainer() const;
      private:
         //---------------------------------------------
         ///\brief  The object container
         //---------------------------------------------
         CDataContainer m_container;
      };
   }
} // namespace shared::versioning
