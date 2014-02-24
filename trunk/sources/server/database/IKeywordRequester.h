#pragma once

#include "entities/Keyword.h"


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
      /// \param [in]      keyword   the keyword to get
      /// \throw           CInvalidParameterException if keyword is unknown
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CKeyword> getKeyword(const std::string & keyword) = 0;

      //--------------------------------------------------------------
      /// \brief           List all keywords 
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CKeyword> > getKeywords() = 0;

      //--------------------------------------------------------------
      /// \brief           Remove a keyword
      /// \param [in]      keyword   the keyword to delete
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeKeyword(const std::string & keyword) = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IKeywordRequester()
      {
      }
   };
 
} //namespace database 
