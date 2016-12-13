#pragma once
#include "entities/Entities.h"


namespace database
{
   class IKeywordRequester
   {
   public:
      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IKeywordRequester()
      {
      }

      //--------------------------------------------------------------
      /// \brief                    Add new keyword
      /// \param [in] newKeywords   New keyword informations
      /// \throw                    shared::exception::CEmptyResult if deviceId is unknown
      //--------------------------------------------------------------
      virtual void addKeyword(const entities::CKeyword& newKeyword) = 0;

      //--------------------------------------------------------------
      /// \brief           Get a keyword
      /// \param [in]      deviceId   the device which own the keyword
      /// \param [in]      keyword   the keyword to get
      /// \throw           shared::exception::CEmptyResult if deviceId is unknown
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CKeyword> getKeyword(int deviceId, const std::string& keyword) const = 0;

      //--------------------------------------------------------------
      /// \brief           Get a keyword
      /// \param [in]      keywordId   the keyword id
      /// \return          the keyword found
      /// \throw           shared::exception::CEmptyResult if keyword is unknown
      //--------------------------------------------------------------
      virtual boost::shared_ptr<entities::CKeyword> getKeyword(int keywordId) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Get keywords identified by a friendly name in a device
      /// \param [in] deviceId            The device where to search
      /// \param [in] friendlyName        The keyword friendly name
      /// \return                         The list of found keywords
      /// \throw                          shared::exception::CEmptyResult if none found
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CKeyword> > getKeywordIdFromFriendlyName(int deviceId, const std::string& friendlyName) const = 0;

      //--------------------------------------------------------------
      /// \brief           List all keywords 
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CKeyword> > getAllKeywords() const = 0;

      //--------------------------------------------------------------
      /// \brief           List all keywords for a device
      /// \param [in]      deviceId   the device which own the keyword
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CKeyword> > getKeywords(int deviceId) const = 0;

      //--------------------------------------------------------------
      /// \brief           List all keywords which match a capacity
      /// \param [in]      capacity   the capacity
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CKeyword> > getKeywordsMatchingCapacity(const std::string& capacity) const = 0;

      //--------------------------------------------------------------
      /// \brief           List all keywords which match capacity for a device
      /// \param [in]      deviceId             the device which own the keyword
      /// \param [in]      capacityName         the capacity name
      /// \param [in]      capacityAccessMode   the capacity acces mode
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CKeyword> > getDeviceKeywordsWithCapacity(int deviceId, const std::string& capacityName, const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode) const = 0;


      //--------------------------------------------------------------
      /// \brief                          Update the keyword blacklist state
      /// \param [in] deviceId            The keyword id
      /// \param [in] blacklist           The keyword blacklist state
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateKeywordBlacklistState(int keywordId, const bool blacklist) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove a keyword
      /// \param [in]      keywordId   the keyword id to delete
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeKeyword(int keywordId) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update a keyword friendly name
      /// \param [in] keywordId           the keyword id to update
      /// \param [in] newFriendlyName     The new friendly name
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateKeywordFriendlyName(int keywordId, const std::string& newFriendlyName) = 0;
   };
} //namespace database 


