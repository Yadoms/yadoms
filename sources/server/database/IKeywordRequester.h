#pragma once

#include "entities/Entities.h"


namespace database { 

   class IKeywordRequester
   {
   public:
      //--------------------------------------------------------------
      /// \brief                    Add new keyword
      /// \param [in] newKeywords   New keyword informations
      /// \throw                    shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void addKeyword(boost::shared_ptr<entities::CKeyword> newKeyword) = 0;

      //--------------------------------------------------------------
      /// \brief           Get a keyword
      /// \param [in]      deviceId   the device which own the keyword
      /// \param [in]      keyword   the keyword to get
      /// \throw           CInvalidParameter if keyword is unknown
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CKeyword> getKeyword(const int deviceId, const std::string & keyword) = 0;

      //--------------------------------------------------------------
      /// \brief           List all keywords 
      /// \param [in]      deviceId   the device which own the keyword
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CKeyword> > getKeywords(const int deviceId) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove a keyword
      /// \param [in]      deviceId   the device which own the keyword
      /// \param [in]      keyword   the keyword to delete
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeKeyword(const int deviceId, const std::string & keyword) = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IKeywordRequester()
      {
      }
   };
 
} //namespace database 
