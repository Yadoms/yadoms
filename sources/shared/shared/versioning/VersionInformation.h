#pragma once
#include "SemVer.h"
#include <shared/DataContainer.h>

namespace shared
{
   namespace versioning
   {
      //---------------------------------------------
      ///\brief Class which contains a version and a releaseType
      //---------------------------------------------
      class CVersionInformation
      {
      public:
         //---------------------------------------------
         ///\brief Constructor
         ///\param [in] version     The version
         //---------------------------------------------
         explicit CVersionInformation(const CSemVer& version);

         //---------------------------------------------
         ///\brief Get the version
         ///\return the version
         //---------------------------------------------
         CSemVer getVersion() const;

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
