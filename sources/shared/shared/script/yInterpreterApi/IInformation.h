#pragma once
#include <shared/DataContainer.h>
#include <shared/versioning/SemVer.h>

namespace shared
{
   namespace script
   {
      namespace yInterpreterApi
      {
         //--------------------------------------------------------------
         /// \brief		Interface for information about the interpreter
         //--------------------------------------------------------------
         class IInformation
         {
         public:
            virtual ~IInformation() = default;

            //--------------------------------------------------------------
            /// \brief	    get the interpreter type
            /// \return     the interpreter type
            //--------------------------------------------------------------
            virtual const std::string& getType() const = 0;

            //--------------------------------------------------------------
            /// \brief	    get the interpreter name
            /// \return     the interpreter name
            //--------------------------------------------------------------
            virtual const std::string& getName() const = 0;

            //--------------------------------------------------------------
            /// \brief	    get the interpreter description
            /// \return     the interpreter description
            //--------------------------------------------------------------
            virtual const std::string& getDescription() const = 0;

            //--------------------------------------------------------------
            /// \brief	    get the interpreter version
            /// \return     the interpreter version
            /// \note       Version : Note that this value is string-comparable
            ///             to check the precedence of a version from a reference version.
            //--------------------------------------------------------------
            virtual const versioning::CSemVer& getVersion() const = 0;

            //--------------------------------------------------------------
            /// \brief	    get the interpreter author
            /// \return     the interpreter author
            //--------------------------------------------------------------
            virtual const std::string& getAuthor() const = 0;

            //--------------------------------------------------------------
            /// \brief	    get the interpreter URL
            /// \return     the interpreter URL
            //--------------------------------------------------------------
            virtual const std::string& getUrl() const = 0;

            //--------------------------------------------------------------
            /// \brief	    get the interpreter credits
            /// \return     the interpreter credits
            //--------------------------------------------------------------
            virtual const std::string& getCredits() const = 0;


            //--------------------------------------------------------------
            /// \brief	    Check if interpreter is supported on this platform
            /// \return     true if supported, false if not
            //--------------------------------------------------------------
            virtual bool isSupportedOnThisPlatform() const = 0;


            //--------------------------------------------------------------
            /// \brief	    Provide the package.json content
            /// \return     the package.json content
            //--------------------------------------------------------------
            virtual boost::shared_ptr<const CDataContainer> getPackage() const = 0;

            //--------------------------------------------------------------
            /// \brief	    Get the interpreter path
            /// \return     the interpreter path (folder)
            //--------------------------------------------------------------
            virtual const boost::filesystem::path& getPath() const = 0;
         };
      }
   }
} // namespace shared::script::yInterpreterApi



