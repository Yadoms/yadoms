#pragma once
#include "database/entities/Entities.h"

namespace dataAccessLayer
{
   class IKeywordManager
   {
   public:
      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IKeywordManager()
      {
      }

      //--------------------------------------------------------------
      /// \brief                          Check if keyword exists
      /// \param [in] deviceId            The device Id where to search
      /// \param [in] keywordName         The keyword name
      /// \return                         true if exist, else false
      //--------------------------------------------------------------
      virtual bool keywordExists(int deviceId, const std::string& keywordName) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Check if keyword exists
      /// \param [in] keywordId           The keyword id
      /// \return                         true if exist, else false
      //--------------------------------------------------------------
      virtual bool keywordExists(int keywordId) const = 0;

      //--------------------------------------------------------------
      /// \brief           Get a keyword
      /// \param [in]      deviceId   the device which own the keyword
      /// \param [in]      keyword   the keyword to get
      /// \throw           shared::exception::CEmptyResult if deviceId is unknown
      //--------------------------------------------------------------
      virtual boost::shared_ptr<database::entities::CKeyword> getKeyword(int deviceId, const std::string& keyword) const = 0;

      //--------------------------------------------------------------
      /// \brief           Get a keyword
      /// \param [in]      keywordId   the keyword id
      /// \return          the keyword found
      /// \throw           shared::exception::CEmptyResult if keyword is unknown
      //--------------------------------------------------------------
      virtual boost::shared_ptr<database::entities::CKeyword> getKeyword(int keywordId) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Get keywords identified by a friendly name in a device
      /// \param [in] deviceId            The device where to search
      /// \param [in] friendlyName        The keyword friendly name
      /// \return                         The list of found keywords
      /// \throw                          shared::exception::CEmptyResult if none found
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CKeyword>> getKeywordIdFromFriendlyName(int deviceId, const std::string& friendlyName) const = 0;

      //--------------------------------------------------------------
      /// \brief           List all keywords 
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CKeyword>> getAllKeywords() const = 0;

      //--------------------------------------------------------------
      /// \brief           List all keywords for a device
      /// \param [in]      deviceId   the device which own the keyword
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CKeyword>> getKeywords(int deviceId) const = 0;

      //--------------------------------------------------------------
      /// \brief           List all keywords which match a capacity
      /// \param [in]      capacity   the capacity
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CKeyword>> getKeywordsMatchingCapacity(const std::string& capacity) const = 0;

      //--------------------------------------------------------------
      /// \brief           List all keywords which match capacity for a device
      /// \param [in]      deviceId             the device which own the keyword
      /// \param [in]      capacityName         the capacity name
      /// \param [in]      capacityAccessMode   the capacity acces mode
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CKeyword>> getDeviceKeywordsWithCapacity(int deviceId,
                                                                                                         const std::string& capacityName,
                                                                                                         const shared::plugin::yPluginApi::EKeywordAccessMode& capacityAccessMode) const = 0;

      //-----------------------------------------
      ///\brief      Get the last acquisition of a keyword
      ///\param [in] keywordId  The keyword id
      ///\param [in] throwIfNotExists  true to throw exception if keyword have no data, false to get an empty shared_ptr
      ///\return     the last acquisition for the keyword
      ///\throw      CEmptyResult if no data is available
      //-----------------------------------------
      virtual boost::shared_ptr<database::entities::CAcquisition> getKeywordLastAcquisition(const int keywordId,
                                                                                            bool throwIfNotExists = true) = 0;

      //-----------------------------------------
      ///\brief      Get the last data of a keyword
      ///\param [in] keywordId  The keyword id
      ///\param [in] throwIfNotExists  true to throw exception if keyword have no data, false to get an empty shared_ptr
      ///\return     the last acquisition for the keyword
      ///\throw      CEmptyResult if no data is available
      //-----------------------------------------
      virtual std::string getKeywordLastData(const int keywordId,
                                             bool throwIfNotExists = true) = 0;

      //--------------------------------------------------------------
      /// \brief                    Add new keyword
      /// \param [in] deviceId      ID of device owner
      /// \param [in] keyword       Historizable keyword
      /// \param [in] details       Keyword details
      /// \throw                    shared::exception::CEmptyResult if device is unknown
      //--------------------------------------------------------------
      virtual void addKeyword(int deviceId,
                              const shared::plugin::yPluginApi::historization::IHistorizable& keyword,
                              const shared::CDataContainer& details = shared::CDataContainer::EmptyContainer) = 0;

      //--------------------------------------------------------------
      /// \brief                    Add new keywords to a device
      /// \param [in] deviceId      ID of device owner
      /// \param [in] keywords      Historizable keywords
      /// \throw                    shared::exception::CEmptyResult if device is unknown
      //--------------------------------------------------------------
      virtual void addKeywords(int deviceId,
                               const std::vector<boost::shared_ptr<const shared::plugin::yPluginApi::historization::IHistorizable>>& keywords) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update a keyword friendly name
      /// \param [in] deviceId            the device which own the keyword
      /// \param [in] keyword             the keyword to update
      /// \param [in] newFriendlyName     The new friendly name
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateKeywordFriendlyName(int deviceId, const std::string& keyword, const std::string& newFriendlyName) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update a keyword friendly name
      /// \param [in] keywordId           the keyword id to update
      /// \param [in] newFriendlyName     The new friendly name
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateKeywordFriendlyName(int keywordId, const std::string& newFriendlyName) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update the keyword blacklist state
      /// \param [in] deviceId            The keyword id
      /// \param [in] blacklist           The keyword blacklist state
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateKeywordBlacklistState(int keywordId, const bool blacklist) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove a keyword
      /// \param [in]      deviceId   the device which own the keyword
      /// \param [in]      keyword   the keyword to delete
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeKeyword(int deviceId, const std::string& keyword) = 0;

      //--------------------------------------------------------------
      /// \brief           Remove a keyword
      /// \param [in]      keywordId   the keyword id to delete
      /// \throw           shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void removeKeyword(int keywordId) = 0;
   };
} //namespace dataAccessLayer 


