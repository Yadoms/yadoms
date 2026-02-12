#pragma once

namespace testCommon
{
    class filesystem
    {
    public:
        static void createDirectory(const std::filesystem::path& path);

        static void removeDirectory(const std::filesystem::path& path);

        static void createFile(const std::filesystem::path& file,
                               const std::string& content,
                               bool withBom = false);

        static void removeFile(const std::string& dir,
                               const std::string& file);
        static void removeFile(const std::string& file,
                               bool successRequired);
        static void removeFile(const std::filesystem::path& file,
                               bool successRequired);

        static void writeFile(const std::string& dir,
                              const std::string& file);

        static void writeFile(const std::string& dir,
                              const std::string& file,
                              const std::string& content);

        static void writeFile(const std::filesystem::path& file,
                              const std::string& content);

        static void renameFile(const std::string& dir,
                               const std::string& oldFile,
                               const std::string& newFile);

        static boost::filesystem::path toBoost(const std::filesystem::path& path);
    };
} // namespace testCommon
