#include "stdafx.h"
#include "UploadFileThread.h"

#include <shared/tools/Filesystem.h>
#include <shared/Log.h>

namespace web
{
   namespace rest
   {
      namespace service
      {
         CUploadFileThread::CUploadFileThread(std::function<void()> onTaskEnd,
                                              int timeoutSeconds)
            : m_timerThread(boost::make_shared<boost::thread>(&CUploadFileThread::doWork,
                                                              this,
                                                              timeoutSeconds,
                                                              onTaskEnd)),
              m_temporaryFilename(shared::tools::CFilesystem::createTemporaryFilename())
         {
         }

         void CUploadFileThread::storeChunk(boost::shared_ptr<std::string> chunkData,
                                            const boost::filesystem::path& destinationFullPath,
                                            unsigned int fileSize,
                                            int timeoutSeconds)
         {
            m_eventHandler.postEvent(kStoreChunk,
                                     std::make_tuple(chunkData,
                                                     destinationFullPath,
                                                     fileSize,
                                                     timeoutSeconds));
         }

         void CUploadFileThread::doWork(int initialTimeoutSeconds,
                                        const std::function<void()>& onTaskEnd)
         {
            try
            {
               auto timeoutSeconds = initialTimeoutSeconds;
               auto end = false;
               while (!end)
               {
                  switch (m_eventHandler.waitForEvents(boost::posix_time::seconds(timeoutSeconds)))
                  {
                  case kStoreChunk:
                     {
                        const auto data =
                           m_eventHandler.getEventData<std::tuple<boost::shared_ptr<std::string>, boost::filesystem::path, unsigned int, int>>();
                        boost::shared_ptr<std::string> chunkData;
                        boost::filesystem::path filename;
                        unsigned int fileSize;
                        std::tie(chunkData, filename, fileSize, timeoutSeconds) = data;

                        appendChunkToFile(chunkData);
                        if (shared::tools::CFilesystem::fileSize(m_temporaryFilename) >= fileSize)
                        {
                           shared::tools::CFilesystem::rename(m_temporaryFilename,
                                                              filename);
                           end = true;
                        }
                        break;
                     }
                  case shared::event::kTimeout:
                     {
                        YADOMS_LOG(information) << "Uploading file aborted : timeout";

                        // Cancel task
                        shared::tools::CFilesystem::remove(m_temporaryFilename);
                        end = true;
                        break;
                     }
                  default:
                     {
                        YADOMS_LOG(error) << "Invalid event ID " << m_eventHandler.getEventId() << ", ignored";
                        break;
                     }
                  }
               }
            }
            catch (boost::thread_interrupted&)
            {
            }

            onTaskEnd();
         }

         void CUploadFileThread::appendChunkToFile(const boost::shared_ptr<std::string>& chunkData) const
         {
            std::ofstream outFile(m_temporaryFilename.string(),
                                  std::fstream::app | std::ios::binary);
            outFile << *chunkData;
         }
      } //namespace service
   } //namespace rest
} //namespace web 
