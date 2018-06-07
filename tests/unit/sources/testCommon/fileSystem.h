#pragma once

namespace testCommon
{
   class filesystem
   {
   public:
      static void CreateDirectory(const std::string& name);

      static void RemoveDirectory(const std::string& name);

      static void CreateFile(const std::string& dir,
                             const std::string& file);

      static void RemoveFile(const std::string& dir,
                             const std::string& file);
      static void RemoveFile(const std::string& file,
                             bool successRequired);
      static void RemoveFile(const boost::filesystem::path& file,
                             bool successRequired);

      static void WriteFile(const std::string& dir,
                            const std::string& file);

      static void WriteFile(const std::string& dir,
                            const std::string& file,
                            const std::string& content);

      static void WriteFile(const boost::filesystem::path& file,
                            const std::string& content);

      static void RenameFile(const std::string& dir,
                             const std::string& oldFile,
                             const std::string& newFile);
   };
} // namespace testCommon
