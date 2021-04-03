#pragma once
#include "IPathProvider.h"
#include "IUploadFileManager.h"
#include "UploadFileThread.h"
#include <shared/Log.h>

namespace web
{
   namespace rest
   {
      namespace service
      {
         //-----------------------------------------------------------------------------
         /// \brief  Upload file manager
         //-----------------------------------------------------------------------------      
         class CUploadFileManager final : public IUploadFileManager
         {
         public:
            CUploadFileManager() = default;
            ~CUploadFileManager() override = default;

            CUploadFileManager(const CUploadFileManager&) = delete;
            CUploadFileManager& operator=(const CUploadFileManager&) = delete;
            CUploadFileManager(const CUploadFileManager&&) = delete;
            CUploadFileManager& operator=(const CUploadFileManager&&) = delete;

            // IUploadFileManager Implementation
            void storeChunk(const std::string& requestGuid,
                            const boost::filesystem::path& destinationFullPath,
                            unsigned int fileSize,
                            const boost::shared_ptr<std::string>& chunkData,
                            int timeoutSeconds = 60000) override;
            // [END] IUploadFileManager Implementation

         private:
            void closeFile(const std::string& requestGuid,
                           const boost::filesystem::path& destinationFullPath);

            const boost::shared_ptr<IPathProvider> m_pathManager;
            std::map<std::string, boost::shared_ptr<CUploadFileThread>> m_uploadingFileThread;
            boost::recursive_mutex m_uploadingFileThreadMutex;

            enum { kUploadTimeout = shared::event::kUserFirstId };
         };
      } //namespace service
   } //namespace rest
} //namespace web 
