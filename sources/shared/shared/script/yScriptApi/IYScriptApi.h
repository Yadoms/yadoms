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
      ///\brief Read the last known state of the keyword
      ///\param[in] keywordId The keyword ID we are interesting in
      ///\return The last known keyword state (empty if no known state)
      //-----------------------------------------------------
      virtual std::string readKeyword(int keywordId) const = 0;

      //-----------------------------------------------------
      ///\brief Wait for a new acquisition on a keyword
      ///\param[in] keywordId The keyword ID to watch
      ///\param[in] timeout Timeout to wait. Format is "hh:mm:ss.xxx". No timeout if empty (default).
      ///\return newState The keyword new state (empty if timeout)
      //-----------------------------------------------------
      virtual std::string waitForAcquisition(int keywordId, const std::string& timeout = std::string()) const = 0;

      //-----------------------------------------------------
      ///\brief Wait for a new acquisition on a keyword list
      ///\param[in] keywordIdList The keyword IDs list to watch
      ///\param[in] timeout Timeout to wait. Format is "hh:mm:ss.xxx". No timeout if empty (default).
      ///\return Returned value is a pair of the keyword Id who changed, and its new value. The keyword Id is kTimeout if timeout.
      //-----------------------------------------------------
      enum { kTimeout = -1 };//TODO à commenter aussi, et voir si la constante est exportée dans Python
      virtual std::pair<int, std::string> waitForAcquisitions(const std::vector<int> keywordIdList, const std::string& timeout = std::string()) const = 0;

      //-----------------------------------------------------
      ///\brief Change state of a keyword
      ///\param[in] keywordId The keyword ID to change state
      ///\param[in] newState The keyword new state
      ///\note Do nothing if keyword is not found or not writable
      //-----------------------------------------------------
      virtual void writeKeyword(int keywordId, const std::string& newState) = 0;

      //-----------------------------------------------------
      ///\brief Send a notification
      ///\param[in] keywordId The keyword ID to use to send notification
      ///\param[in] recipientId The recipient ID
      ///\param[in] message The message to send
      ///\note Do nothing if keyword or recipient is not found or not writable
      //-----------------------------------------------------
      virtual void sendNotification(int keywordId, int recipientId, const std::string& message) = 0;

      //-----------------------------------------------------
      ///\brief Get general information
      ///\param[in] key Information key
      ///\return Information as string (empty string if not found)
      ///\details Supported value :
      ///    - sunrise : get sunrise hour : returns hour in the day (as double encoded in string)
      ///    - sunset : get sunset hour : returns hour in the day (as double encoded in string)
      ///    - latitude : get latitude : returns latitude configured by user (or approximative latitude if not configured)
      ///    - longitude : get longitude : returns latitude configured by user (or approximative longitude if not configured)
      ///    - altitude : get altitude : returns latitude configured by user (or approximative altitude if not configured)
      ///    - yadomsServerOS : get yadoms server OS : returns a string containing the OS name where Yadoms is running
      ///    - yadomsServerVersion : get yadoms application version : returns a string containing the Yadoms application version
      //-----------------------------------------------------
      //TODO voir si on peut exporter les valeurs supportées sous forme d'enum
      virtual std::string getInfo(const std::string& key) const = 0;

      //-----------------------------------------------------
      ///\brief Can be used to prevent a rule for auto-restart
      ///\param[in] enable true to active auto-restart, false to disable the rule (after the current execution is complete)
      ///\note : by default, a rule auto-restart at its end
      //-----------------------------------------------------
      virtual void ruleEnable(bool enable = true) = 0;
   };
	
} } } // namespace shared::script::yScriptApi
	
	