#pragma once
#include "FrameBase.h"
#include "database/entities/Entities.h"

namespace web
{
   namespace poco
   {
      namespace ws
      {
         //-----------------------------
         ///\brief Frame which can be send to GUI to notify for a deleted frame
         //-----------------------------
         class CKeywordDeletedFrame : public CFrameBase
         {
         public:
            //-----------------------------
            ///\brief Constructor
            ///\param [in]    content     The keyword to send to GUI (shared_ptr)
            //-----------------------------
            explicit CKeywordDeletedFrame(boost::shared_ptr<const database::entities::CKeyword> content);

            //-----------------------------
            ///\brief Destructor
            //-----------------------------
            virtual ~CKeywordDeletedFrame();

         protected:
            //-----------------------------
            ///\bruef The keyword field name
            //-----------------------------
            static const std::string m_keyword;
         };
      } //namespace ws
   } //namespace poco
} //namespace web
