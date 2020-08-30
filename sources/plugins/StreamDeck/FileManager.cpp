#include "FileManager.h"

CFileManager::CFileManager(std::string& path)
   : m_path(path),
     m_ifs(m_path, std::ios::binary)

{
}

void CFileManager::read()
{
   if (!m_ifs.is_open())
      throw std::runtime_error("CFileManager read function : path does not exist");

   std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(m_ifs), {});
   const std::string data(buffer.begin(), buffer.end());
   m_fileDataAsString = data;
}

void CFileManager::close()
{
   m_ifs.close();
}

std::string CFileManager::getData() const
{
   return m_fileDataAsString;
}
