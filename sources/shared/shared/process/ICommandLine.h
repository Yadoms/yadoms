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
         /// \brief	                     Get the executable filename
         /// \return                      The executable filename (with extension)
         //--------------------------------------------------------------
         virtual std::string executable() const = 0;

         //--------------------------------------------------------------
         /// \brief	                     Get the working directory to use to call executable
         /// \return                      The working directory (full path)
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
