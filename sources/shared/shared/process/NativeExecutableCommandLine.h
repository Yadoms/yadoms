#pragma once
#include <shared/process/ICommandLine.h>

namespace shared
{
   namespace process
   {
      //--------------------------------------------------------------
      /// \brief	Command line generator
      //--------------------------------------------------------------
      class CNativeExecutableCommandLine : public ICommandLine
      {
      public:
         //--------------------------------------------------------------
         /// \brief	Constructor
         /// \param[in] executablePath    The executable path (without directory if in system path), in native format (ie : with extension under Windows)
         /// \param[in] workingDirectory  From where to start executable
         /// \param[in] parameters        Command line arguments
         //--------------------------------------------------------------
         CNativeExecutableCommandLine(const boost::filesystem::path& executablePath,
                                      const boost::filesystem::path& workingDirectory,
                                      const std::vector<std::string>& parameters);

         //--------------------------------------------------------------
         /// \brief	Destructor
         //--------------------------------------------------------------
         virtual ~CNativeExecutableCommandLine();

         // ICommandLine Implementation
         const boost::filesystem::path& executable() const override;
         const boost::filesystem::path& workingDirectory() const override;
         const std::vector<std::string>& args() const override;
         // [END] ICommandLine Implementation

      private:
         const boost::filesystem::path m_executablePath;
         const boost::filesystem::path m_workingDirectory;
         const std::vector<std::string> m_args;
      };
   }
} // namespace shared::process


