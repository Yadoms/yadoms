#pragma once
#include "entities/Entities.h"


namespace database
{
   class IKeywordRequester
   {
   public:
      virtual ~IKeywordRequester() = default;

      //--------------------------------------------------------------
      /// \brief                    Add new keyword
      /// \param [in] newKeyword    New keyword information
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
      virtual std::vector<boost::shared_ptr<entities::CKeyword>> getKeywordIdFromFriendlyName(int deviceId,
                                                                                              const std::string& friendlyName) const = 0;

      //--------------------------------------------------------------
      /// \brief           List all keywords 
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CKeyword>> getAllKeywords() const = 0;

      //--------------------------------------------------------------
      /// \brief           List all keywords for a device
      /// \param [in]      deviceId   the device which own the keyword
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CKeyword>> getKeywords(int deviceId) const = 0;

      //--------------------------------------------------------------
      /// \brief           List all keywords which match a capacity
      /// \param [in]      capacity   the capacity
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CKeyword>> getKeywordsMatchingCapacity(const std::string& capacity) const = 0;

      //--------------------------------------------------------------
      /// \brief                          Get the keyword list which match some criteria
      /// \param [in] expectedKeywordTypes         The keyword type criteria
      /// \param [in] expectedCapacities           The capacity name criteria
      /// \param [in] expectedKeywordAccesses      The access mode criteria
      /// \param [in] expectedKeywordHistoryDepth  The history depth criteria
      /// \param [in] blacklisted                  The blacklisted criteria
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<database::entities::CKeyword>> getKeywordsMatchingCriteria(
         const std::vector<shared::plugin::yPluginApi::EKeywordDataType>& expectedKeywordTypes,
         const std::vector<std::string>& expectedCapacities,
         const std::vector<shared::plugin::yPluginApi::EKeywordAccessMode>& expectedKeywordAccesses,
         const std::vector<shared::plugin::yPluginApi::EHistoryDepth>& expectedKeywordHistoryDepth,
         bool blacklisted) const = 0;

      //--------------------------------------------------------------
      /// \brief           List all keywords which match capacity for a device
      /// \param [in]      deviceId             the device which own the keyword
      /// \param [in]      capacityName         the capacity name
      /// \param [in]      capacityAccessMode   the capacity access mode
      /// \return          List of registered keywords
      //--------------------------------------------------------------
      virtual std::vector<boost::shared_ptr<entities::CKeyword>> getDeviceKeywordsWithCapacity(int deviceId,
                                                                                               const std::string& capacityName,
                                                                                               const shared::plugin::yPluginApi::EKeywordAccessMode&
                                                                                               capacityAccessMode) const = 0;

      //-----------------------------------------
      ///\brief      Get the last acquisition of a keyword
      ///\param [in] keywordId  The keyword id
      ///\param [in] throwIfNotExists  true to throw exception if keyword have no data, false to get an empty shared_ptr
      ///\return     the last acquisition for the keyword
      ///\throw      CEmptyResult if no data is available
      //-----------------------------------------
      virtual boost::shared_ptr<entities::CAcquisition> getKeywordLastAcquisition(int keywordId,
                                                                                  bool throwIfNotExists = true) = 0;

      //-----------------------------------------
      ///\brief      Get the last data of a keyword
      ///\param [in] keywordId  The keyword id
      ///\param [in] throwIfNotExists  true to throw exception if keyword have no data, false to get an empty shared_ptr
      ///\return     the last acquisition for the keyword
      ///\throw      CEmptyResult if no data is available
      //-----------------------------------------
      virtual std::string getKeywordLastData(int keywordId,
                                             bool throwIfNotExists = true) = 0;

      //-----------------------------------------
      ///\brief      Get the last data of a keywords list
      ///\param [in] keywordIds  The keyword ids list
      ///\return     the last acquisition for the keywords
      //-----------------------------------------
      virtual std::vector<boost::tuple<int, std::string>> getKeywordListLastData(const std::vector<int>& keywordIds) = 0;

      //--------------------------------------------------------------
      /// \brief                          Update the keyword blacklist state
      /// \param [in] keywordId           The keyword id
      /// \param [in] blacklist           The keyword blacklist state
      /// \throw  shared::exception::CEmptyResult if fails
      //--------------------------------------------------------------
      virtual void updateKeywordBlacklistState(int keywordId,
                                               const bool blacklist) = 0;

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

      //--------------------------------------------------------------
      /// \brief                          Update the last value of a keyword
      /// \param [in] keywordId           the keyword id to update
      /// \param [in] valueDatetime       The new value date time
      /// \param [in] value               The new value
      //--------------------------------------------------------------
      virtual void updateLastValue(int keywordId, const boost::posix_time::ptime& valueDatetime,
                                   const std::string& value) = 0;
   };
} //namespace database 
