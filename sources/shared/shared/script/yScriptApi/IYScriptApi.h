#pragma once

namespace shared { namespace script { namespace yScriptApi
{

   //-----------------------------------------------------
   ///\brief The API used by the scripts to interact with Yadoms
   //-----------------------------------------------------
   class IYScriptApi
   {
   public:
      //-----------------------------------------------------
      ///\brief Destructor
      //-----------------------------------------------------
      virtual ~IYScriptApi() {}

      //-----------------------------------------------------
      ///\brief Read the keyword state
      ///\param[in] keywordId The keyword ID we are interesting it
      ///\return The last known keyword state (can be empty, if no known state)
      //-----------------------------------------------------
      virtual std::string readKeyword(int keywordId) const = 0;

      //-----------------------------------------------------
      ///\brief Wait for events on a keyword
      ///\param[in] keywordId The keyword ID to watch
      ///\param[in] timeout Timeout of wait. Format is "hh:mm:ss.xxx". No timeout if empty (default).
      ///\return newState The keyword new state (empty if timeout)
      //-----------------------------------------------------
      virtual std::string waitForEvent(int keywordId, const std::string& timeout = std::string()) const = 0;

      //-----------------------------------------------------
      ///\brief Wait for events on a keywords list
      ///\param[in] keywordIdList The keyword IDs list to watch
      ///\param[in] timeout Timeout of wait. Format is "hh:mm:ss.xxx". No timeout if empty (default).
      ///\return Pair of <keywordId, keywordNewState> for the received event. Timeout if keywordId == kTimeout.
      //-----------------------------------------------------
      enum { kTimeout = -1 };
      virtual std::pair<int, std::string> waitForEvents(std::vector<int> keywordIdList, const std::string& timeout = std::string()) const = 0;

      //-----------------------------------------------------
      ///\brief Change state of keyword
      ///\param[in] keywordId The keyword ID to change state
      ///\param[in] newState The keyword new state
      ///\note Do nothing if keyword is not found or not writtable
      //-----------------------------------------------------
      virtual void writeKeyword(int keywordId, const std::string& newState) = 0;

      //-----------------------------------------------------
      ///\brief Send notification
      ///\param[in] keywordId The keyword ID to use to send notification
      ///\param[in] recipientId The recipient ID
      ///\param[in] message The message to send
      ///\note Do nothing if keyword or recipient is not found or not writtable
      //-----------------------------------------------------
      virtual void sendNotification(int keywordId, int recipientId, const std::string& message) = 0;

      //-----------------------------------------------------
      ///\brief Get general informations
      ///\param[in] key Information key
      ///\return Information as string (empty string if not found)
      ///\details : Supported value :
      ///    - sunrise : get sunrise hour : returns hour in the day (as double encoded in string)
      ///    - sunset : get sunset hour : returns hour in the day (as double encoded in string)
      //-----------------------------------------------------
      virtual std::string getInfo(const std::string& key) const = 0;
   };
	
} } } // namespace shared::script::yScriptApi
	
	