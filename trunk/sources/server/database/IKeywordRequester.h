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
      /// \brief           Get a keyword
      /// \param [in]      keywordId   the keyword id
      /// \return          the keyword found
      /// \throw           CInvalidParameter if keyword is unknown
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CKeyword> getKeyword(const int keywordId) = 0;

      //--------------------------------------------------------------
      /// \brief           List all keywords 
      /// \param [in]      deviceId   the device which own the keyword
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CKeyword> > getKeywords(const int deviceId) = 0;  
      
      //--------------------------------------------------------------
      /// \brief           List all keywords which match capacity for a device
      /// \param [in]      deviceId             the device which own the keyword
      /// \param [in]      capacityName         the capacity name
      /// \param [in]      capacityAccessMode   the capacity acces mode
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CKeyword> > getDeviceKeywordsWithCapacity(const int deviceId, const std::string & capacityName, const database::entities::ECapacityAccessMode capacityAccessMode) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove a keyword
      /// \param [in]      deviceId   the device which own the keyword
      /// \param [in]      keyword   the keyword to delete
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeKeyword(const int deviceId, const std::string & keyword) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove a keyword
      /// \param [in]      keywordId   the keyword id to delete
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeKeyword(const int keywordId) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update a keyword friendly name
      /// \param [in] deviceId            the device which own the keyword
      /// \param [in] keyword             the keyword to update
      /// \param [in] newFriendlyName     The new friendly name
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateKeywordFriendlyName(const int deviceId, const std::string & keyword, const std::string & newFriendlyName) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update a keyword friendly name
      /// \param [in] keywordId           the keyword id to update
      /// \param [in] newFriendlyName     The new friendly name
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateKeywordFriendlyName(const int keywordId, const std::string & newFriendlyName) = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IKeywordRequester()
      {
      }
   };
 
} //namespace database 
