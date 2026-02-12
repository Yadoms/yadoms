#include "stdafx.h"
#include "fileSystem.h"
#include <filesystem>
#include <boost/test/unit_test.hpp>

using namespace testCommon;

void filesystem::createDirectory(const std::filesystem::path& path)
{
    BOOST_REQUIRE(std::filesystem::create_directory(path)) ;
}

void filesystem::removeDirectory(const std::filesystem::path& path)
{
    std::filesystem::remove_all(path);
}

void filesystem::createFile(const std::filesystem::path& file,
                            const std::string& content,
                            bool withBom)
{
    std::filesystem::remove(file);

    if (!file.parent_path().empty() && !std::filesystem::exists(file.parent_path()))
        std::filesystem::create_directories(file.parent_path());

    if (withBom)
    {
        std::ofstream binaryStream;
        binaryStream.open(file.string(), std::ios::out | std::ios::binary);
        unsigned char bom[3] = {0xEF, 0xBB, 0xBF};
        binaryStream.write(reinterpret_cast<char*>(bom), sizeof bom);
        binaryStream.close();
    }

    std::ofstream textStream;
    if (withBom)
        textStream.open(file.string(), std::ios::out | std::ios::app);
    else
        textStream.open(file.string(), std::ios::out);
    textStream << content;
    textStream.close();
}

void filesystem::removeFile(const std::string& dir,
                            const std::string& file)
{
    const auto fullPath = std::filesystem::path(dir) / file;
    BOOST_REQUIRE(std::filesystem::remove(fullPath)) ;
}

void filesystem::removeFile(const std::string& file,
                            bool successRequired)
{
    if (successRequired)
        BOOST_REQUIRE(std::filesystem::remove(file)) ;
    else
        std::filesystem::remove(file);
}

void filesystem::removeFile(const std::filesystem::path& file,
                            bool successRequired)
{
    if (successRequired)
        BOOST_REQUIRE(std::filesystem::remove(file.string())) ;
    else
        std::filesystem::remove(file.string());
}

void filesystem::writeFile(const std::string& dir,
                           const std::string& file)
{
    writeFile(dir, file, "some text...");
}

void filesystem::writeFile(const std::string& dir,
                           const std::string& file,
                           const std::string& content)
{
    std::filesystem::path fullPath;
    if (dir.empty())
        fullPath = file;
    else
    {
        std::filesystem::create_directories(dir);
        fullPath = std::filesystem::path(dir) / file;
    }

    std::ofstream outfile(fullPath.string().c_str(), std::ios_base::out);
    outfile << content;
}

void filesystem::writeFile(const std::filesystem::path& file,
                           const std::string& content)
{
    if (!file.parent_path().empty() && !std::filesystem::exists(file.parent_path()))
        std::filesystem::create_directories(file.parent_path());

    std::ofstream outfile(file.string(), std::ios_base::out);
    outfile << content;
}

void filesystem::renameFile(const std::string& dir,
                            const std::string& oldFile,
                            const std::string& newFile)
{
    const auto oldFullPath = std::filesystem::path(dir) / oldFile;
    const auto newFullPath = std::filesystem::path(dir) / newFile;
    std::filesystem::rename(oldFullPath, newFullPath);
}

boost::filesystem::path filesystem::toBoost(const std::filesystem::path& path)
{
    return boost::filesystem::path(path.string());
}
