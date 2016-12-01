#pragma once
#include <shared/versioning/ReleaseType.h>
#include <shared//ILocation.h>

namespace shared
{
   namespace plugin
   {
      namespace information
      {
         //--------------------------------------------------------------
         /// \brief		Interface for information about Yadoms
         //--------------------------------------------------------------
         class IYadomsInformation
         {
         public:
            //--------------------------------------------------------------
            /// \brief	    Destructor
            //--------------------------------------------------------------
            virtual ~IYadomsInformation()
            {
            }

            //--------------------------------------------------------------
            /// \brief	    Return the developper mode state
            /// \return     true if developper mode is active
            //--------------------------------------------------------------
            virtual bool developperMode() const = 0;

            //--------------------------------------------------------------
            /// \brief	    get the Yadoms version
            /// \return     the Yadoms version
            /// \note       Version : Note that this value is string-comparable
            ///             to check the precedence of a version from a reference version.
            //--------------------------------------------------------------
            virtual std::string version() const = 0;

            //--------------------------------------------------------------
            /// \brief	    get the Yadoms release type
            /// \return     the Yadoms release type
            //--------------------------------------------------------------
            virtual versioning::EReleaseType releaseType() const = 0;

            //--------------------------------------------------------------
            /// \brief	    get location
            /// \return     return server location coordonates
            //--------------------------------------------------------------
            virtual boost::shared_ptr<const ILocation> location() const = 0;
         };
      }
   }
} // namespace shared::plugin::information


