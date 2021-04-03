#include "stdafx.h"
#include "UploadFileManager.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         void CUploadFileManager::storeChunk(const std::string& requestGuid,
                                             const boost::filesystem::path& destinationFullPath,
                                             unsigned int fileSize,
                                             const boost::shared_ptr<std::string>& chunkData,
                                             int timeoutSeconds)
         {
            boost::lock_guard<boost::recursive_mutex> lock(m_uploadingFileThreadMutex);

            if (m_uploadingFileThread.find(requestGuid) == m_uploadingFileThread.end())
            {
               m_uploadingFileThread[requestGuid] = boost::make_shared<CUploadFileThread>(
                  [this, &requestGuid]()
                  {
                     boost::lock_guard<boost::recursive_mutex> lock(m_uploadingFileThreadMutex);
                     m_uploadingFileThread.erase(requestGuid);
                  },
                  timeoutSeconds);
            }

            m_uploadingFileThread[requestGuid]->storeChunk(chunkData,
                                                           destinationFullPath,
                                                           fileSize,
                                                           timeoutSeconds);
         }
      } //namespace service
   } //namespace rest
} //namespace web 
