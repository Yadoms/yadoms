#pragma once

namespace shared
{
   namespace process
   {
      //--------------------------------------------------------------
      /// \brief	Command line generator
      //--------------------------------------------------------------
      class ICommandLine
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~ICommandLine() {}

         //--------------------------------------------------------------
         /// \brief	                     Get the executable full path
         /// \return                      The executable full path (with extension)
         /// \note Can contain folders or not (if in environment PATH or local file)
         //--------------------------------------------------------------
         virtual const boost::filesystem::path& executable() const = 0;

         //--------------------------------------------------------------
         /// \brief	                     Get the working directory to use to call executable
         /// \return                      The working directory (relative to current module)
         //--------------------------------------------------------------
         virtual const boost::filesystem::path& workingDirectory() const = 0;

         //--------------------------------------------------------------
         /// \brief	                     Get the command line args
         /// \return                      The args
         //--------------------------------------------------------------
         virtual const std::vector<std::string>& args() const = 0;
      };
   }
} // namespace shared::process
