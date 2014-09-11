#pragma once

#include <shared/enumeration/EnumHelpers.hpp>
#include <shared/DataContainer.h>

namespace web {   namespace ws {

      //-----------------------------
      ///\brief A base class for frames
      //-----------------------------
      class CFrameBase
      {
      public:
         //-----------------------------
         ///\brief Destructor
         //-----------------------------
         virtual ~CFrameBase();

         //-----------------------------
         ///\brief Enumeration for all frame type
         //-----------------------------
         DECLARE_ENUM_HEADER(EFrameType,
            ((AcquisitionFilter)(0))
            ((AcquisitionUpdate)(1))
         );

      protected:
         //-----------------------------
         ///\brief Constructor
         ///\param [in]  content    A string that will be deserialized
         //-----------------------------
         CFrameBase(const std::string & content);

         //-----------------------------
         ///\brief Constructor
         ///\param [in]  type    The type of frame
         //-----------------------------
         CFrameBase(const CFrameBase::EFrameType & type);


      public:
         //-----------------------------
         ///\brief Get the frame type
         //-----------------------------
         EFrameType getType();
            
         //-----------------------------
         ///\brief Serialize the frame
         //-----------------------------
         const std::string serialize();

         //-----------------------------
         ///\brief Get the type field name
         ///\return  The type field name
         //-----------------------------
         static const std::string & getTypeFieldName();

      protected:
         //-----------------------------
         ///\brief Internal container
         //-----------------------------
         shared::CDataContainer m_internalContainer;

         //-----------------------------
         ///\brief The TYPE field name
         //-----------------------------
         static const std::string m_typeFieldName;
         //-----------------------------
         ///\bruef The DATA field name
         //-----------------------------
         static const std::string m_dataFieldName;
      };



} //namespace ws
} //namespace web