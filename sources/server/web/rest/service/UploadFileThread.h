#pragma once
#include <shared/event/EventHandler.hpp>

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CUploadFileThread final
         {
         public:
            enum { kDefaultTimeout = 60000 };

            explicit CUploadFileThread(std::function<void()> onTaskEnd,
                                       int timeoutSeconds = kDefaultTimeout);
            ~CUploadFileThread() = default;

            CUploadFileThread(const CUploadFileThread&) = delete;
            CUploadFileThread& operator=(const CUploadFileThread&) = delete;
            CUploadFileThread(const CUploadFileThread&&) = delete;
            CUploadFileThread& operator=(const CUploadFileThread&&) = delete;

            void storeChunk(boost::shared_ptr<std::string> chunkData,
                            const boost::filesystem::path& destinationFullPath,
                            unsigned int fileSize,
                            int timeoutSeconds = kDefaultTimeout);

         private:
            enum
            {
               kStoreChunk = shared::event::kUserFirstId
            };

            void doWork(int initialTimeoutSeconds,
                        const std::function<void()>& onTaskEnd);
            void appendChunkToFile(const boost::shared_ptr<std::string>& chunkData) const;

            boost::shared_ptr<boost::thread> m_timerThread;
            shared::event::CEventHandler m_eventHandler;
            boost::filesystem::path m_temporaryFilename;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
