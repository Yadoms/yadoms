#pragma once
#include <shared/event/EventHandler.hpp>
#include "IInformation.h"

namespace shared
{
   namespace script
   {
      namespace yInterpreterApi
      {
         //-----------------------------------------------------
         ///\brief The API used by the interpreters to interact with Yadoms
         //-----------------------------------------------------
         class IYInterpreterApi
         {
         public:
            //-----------------------------------------------------
            ///\brief Destructor
            //-----------------------------------------------------
            virtual ~IYInterpreterApi()
            {
            }

            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //--
            //-- Events
            //--
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------

            //-----------------------------------------------------
            ///\brief Events Id from Yadoms received by the interpreter
            ///\note The events described above are sent by Yadoms to the interpreter.
            ///\note They may or not contain data (see description of each event).
            ///\note Data is read from event by a call to api->getEventHandler().getEventData<DataType>();
            ///\note
            ///\note The usage indicates if interpreter must process this event or not
            //-----------------------------------------------------
            enum
            {
               //-----------------------------------------------------
               ///\brief Yadoms requests the interpreter to stop
               ///\usage Needed
               ///\note Data : none
               ///\note Plugin must exit as soon as possible when receiving this event
               //-----------------------------------------------------
               kEventStopRequested = event::kUserFirstId,

               //-----------------------------------------------------
               ///\brief Yadoms requests the interpreter for its avalaibility (ressources fully installed and working)
               ///\usage Required
               ///\note Data : none
               ///\note Interpreter must process the specified request, and return result within 10 seconds, by calling request->sendSuccess or sendError
               //-----------------------------------------------------
               kEventAvalaibleRequest,

               //-----------------------------------------------------
               ///\brief Yadoms requests a script content
               ///\usage Required
               ///\note Data : a boost::shared_ptr<yApi::ILoadScriptContentRequest> object containing the request
               ///\note Interpreter must process the specified request, and return result within 10 seconds, by calling request->sendSuccess or sendError
               //-----------------------------------------------------
               kLoadScriptContentRequest,

               //-----------------------------------------------------
               ///\brief Yadoms requests to save the script content
               ///\usage Required
               ///\note Data : a boost::shared_ptr<yApi::ISaveScriptContent> object containing the request
               ///\note Interpreter must process the specified request
               //-----------------------------------------------------
               kSaveScriptContent,

               //-----------------------------------------------------
               ///\brief First event ID to use for user-defined events
               ///\note Define interpreter events like :
               ///\note enum
               ///\note {
               ///\note    userDefinedFirstEvent = kPluginFirstEventId,
               ///\note    userDefinedSecondEvent,
               ///\note    userDefinedThirdEvent,
               ///\note    ...
               ///\note };
               //-----------------------------------------------------
               kPluginFirstEventId = event::kUserFirstId + 100 // The next usable event ID for the interpreter code
            };


            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //--
            //-- Methods
            //--
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------
            //----------------------------------------------------------------------------------------------------------------

            //-----------------------------------------------------
            ///\brief Get the current plugin information (extracted from package.json file)
            ///\return The current plugin information
            //-----------------------------------------------------      
            virtual boost::shared_ptr<const IInformation> getInformation() const = 0;

            //-----------------------------------------------------
            /// \brief	    Get the interpreter instance data path
            /// \return     The interpreter instance data path (folder)
            /// \note       This folder can be used by interpreter to store any data it needs.
            ///             This folder is unique per each interpreter instance.
            ///             Note that this is folder is not backuped.
            //-----------------------------------------------------      
            virtual const boost::filesystem::path& getDataPath() const = 0;

            //-----------------------------------------------------
            ///\brief Get the event handler associated to the interpreter. The event handler is used
            ///       to received, wait for, or post events from/to Yadoms
            ///\return The interpreter event handler
            //-----------------------------------------------------   
            virtual event::CEventHandler& getEventHandler() = 0;
         };
      }
   }
} // namespace shared::script::yInterpreterApi	


