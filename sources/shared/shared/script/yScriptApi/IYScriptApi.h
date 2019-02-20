#pragma once
#include "WaitForEventResult.h"

namespace shared
{
   namespace script
   {
      namespace yScriptApi
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
            virtual ~IYScriptApi()
            {
            }

            //-----------------------------------------------------
            ///\brief Get a keyword ID
            ///\param[in] deviceName Device name containing the keyword
            ///\param[in] keywordName Keyword name to search for ID
            ///\return The keyword ID
            ///\throw std::out_of_range if several (some have to be renamed) or none keyword ID found
            //-----------------------------------------------------
            virtual int getKeywordId(const std::string& deviceName,
                                     const std::string& keywordName) const = 0;

            //-----------------------------------------------------
            ///\brief Get a recipient ID
            ///\param[in] firstName Recipient first name
            ///\param[in] lastName Recipient last name
            ///\return The recipient ID
            ///\throw std::out_of_range if not found
            //-----------------------------------------------------
            virtual int getRecipientId(const std::string& firstName,
                                       const std::string& lastName) const = 0;

            //-----------------------------------------------------
            ///\brief Read the last known state of the keyword
            ///\param[in] keywordId The keyword ID we are interesting in
            ///\return The last known keyword state (empty if no known state)
            ///\throw std::out_of_range if keyword not found
            //-----------------------------------------------------
            virtual std::string readKeyword(int keywordId) const = 0;

            //-----------------------------------------------------
            ///\brief Read keyword details (last state, properties...)
            ///\param[in] keywordId The keyword ID we are interesting in
            ///\return JSON string containig all details about the keyword. Some value can be missing (if no relevant).
            ///\throw std::out_of_range if keyword not found
            //-----------------------------------------------------
            virtual std::string readKeywordDetails(int keywordId) const = 0;

            //-----------------------------------------------------
            ///\brief Wait for a new acquisition on a keyword
            ///\param[in] keywordId The keyword ID to watch
            ///\param[in] timeout Timeout to wait.
            ///\return newState The keyword new state (empty if timeout)
            ///\throw std::out_of_range if keyword not found
            //-----------------------------------------------------
            virtual std::string waitForNextAcquisition(int keywordId,
                                                       const std::string& timeout = std::string()) const = 0;

            //-----------------------------------------------------
            ///\brief Wait for a new acquisition on a keyword list
            ///\param[in] keywordIdList The keyword IDs list to watch
            ///\param[in] timeout Timeout to wait.
            ///\return Returned value is a pair of the keyword Id who changed, and its new value. The keyword Id is kTimeout if timeout.
            ///\throw std::out_of_range if one of the keyword is not found
            //-----------------------------------------------------
            enum
            {
               kTimeout = -1
            };

            virtual std::pair<int, std::string> waitForNextAcquisitions(const std::vector<int>& keywordIdList,
                                                                        const std::string& timeout = std::string()) const = 0;


            //-----------------------------------------------------
            ///\brief Wait for an event (acquisition, time event...)
            ///\param[in] keywordIdList The keyword IDs list to watch
            ///\param[in] receiveDateTimeEvent true to receive date/time events (one per minute)
            ///\param[in] timeout Timeout to wait.
            ///\return Returned value is a pair of the keyword Id who changed, and its new value. The keyword Id is kTimeout if timeout.
            ///\throw std::out_of_range if one of the keyword is not found
            //-----------------------------------------------------
            // ReSharper disable once CppRedundantQualifier, needed by Swig for script API generation
            virtual shared::script::yScriptApi::CWaitForEventResult waitForEvent(const std::vector<int>& keywordIdList,
                                                                                 bool receiveDateTimeEvent,
                                                                                 const std::string& timeout = std::string()) const = 0;


            //-----------------------------------------------------
            ///\brief Get all keywords id matching a capacity name
            ///\param[in] capacity The capacity name
            ///\return The keyword Id list
            //-----------------------------------------------------
            virtual std::vector<int> getKeywordsByCapacity(const std::string& capacity) const = 0;

            //-----------------------------------------------------
            ///\brief Change state of a keyword
            ///\param[in] keywordId The keyword ID to change state
            ///\param[in] newState The keyword new state
            ///\throw std::out_of_range if keyword not found
            ///\note Do nothing if keyword is not found or not writable
            //-----------------------------------------------------
            virtual void writeKeyword(int keywordId, const std::string& newState) = 0;

            //-----------------------------------------------------
            ///\brief Send a notification
            ///\param[in] keywordId The keyword ID to use to send notification
            ///\param[in] recipientId The recipient ID
            ///\param[in] message The message to send
            ///\note Do nothing if keyword or recipient is not found or not writtable
            ///\throw std::out_of_range if keyword or recipient not found
            //-----------------------------------------------------
            virtual void sendNotification(int keywordId,
                                          int recipientId,
                                          const std::string& message) = 0;

            //-----------------------------------------------------
            ///\brief Get a keyword friendly name
            ///\param[in] keywordId The keyword ID
            ///\return The friendly name of the keyword
            //-----------------------------------------------------
            virtual std::string getKeywordName(int keywordId) const = 0;

            //-----------------------------------------------------
            ///\brief Get a device friendly name
            ///\param[in] keywordId The keyword ID attached to the device
            ///\return The friendly name of the device to which is attached the keyword
            //-----------------------------------------------------
            virtual std::string getKeywordDeviceName(int keywordId) const = 0;

            //-----------------------------------------------------
            ///\brief Get general information keys
            //-----------------------------------------------------
            enum EInfoKeys
            {
               kSunrise, ///< Sunrise time : get current day sunrise time (format is "HH:MM")
               kSunset, ///< Sunset time : get current day sunset time (format is "HH:MM")

               kLatitude, ///< Latitude : returns latitude configured by user (or approximative latitude if not configured)
               kLongitude, ///< Longitude : returns latitude configured by user (or approximative longitude if not configured)
               kAltitude, ///< Altitude : returns latitude configured by user (or approximative altitude if not configured)

               kYadomsServerOS, ///< Yadoms server OS : returns a string containing the OS name where Yadoms is running
               kYadomsServerVersion, ///< Yadoms application version : returns a string containing the Yadoms application version
            };

            //-----------------------------------------------------
            ///\brief Get general information
            ///\param[in] key Information key
            ///\return Information as string (empty string if not found)
            ///\throw std::out_of_range if key not found
            //-----------------------------------------------------
            virtual std::string getInfo(EInfoKeys key) const = 0;
         };
      }
   }
} // namespace shared::script::yScriptApi
