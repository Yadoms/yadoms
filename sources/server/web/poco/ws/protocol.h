#pragma once

#include <shared/enumeration/EnumHelpers.hpp>

namespace web {
   namespace ws {

      class CFrame
      {
      public:
         //-----------------------------
         ///\brief Enumeration for all frame type
         //-----------------------------
         DECLARE_ENUM_HEADER(EFrameType,
            ((AcquisitionFilter)(0))
            ((AcquisitionUpdate)(1))
         );

         //-----------------------------
         ///\brief Get the TYPE field name
         ///\return the TYPE field name
         //-----------------------------
         static const std::string & getFieldNameType();

         //-----------------------------
         ///\brief Get the DATA field name
         ///\returnthe DATA field name
         //-----------------------------
         static const std::string & getFieldNameData();

      private:
         //-----------------------------
         ///\return The TYPE field name
         //-----------------------------
         const std::string m_typeFieldName;
         //-----------------------------
         ///\return The DATA field name
         //-----------------------------
         const std::string m_dataFieldName;

      };


   } //namespace ws
} //namespace web