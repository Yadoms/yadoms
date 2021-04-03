#pragma once
#include <string>

namespace web
{
   namespace rest
   {
      namespace service
      {
         //-----------------------------------------------------------------------------
         /// \brief  Upload file manager interface
         //-----------------------------------------------------------------------------      
         class IUploadFileManager
         {
         public:
            virtual ~IUploadFileManager() = default;

            //-----------------------------------------------------------------------------
            /// \brief  Store a chunk
            /// \param requestGuid  The request GUID to concate all chunks in a single file
            /// \param destinationFullPath The expected file name
            /// \param fileSize The total file size
            /// \param chunkData The data to add to corresponding GUID chunk
            /// \param timeoutSeconds The timeout between 2 chunks (transfer is cancelled if timeout reached)
            //-----------------------------------------------------------------------------   
            virtual void storeChunk(const std::string& requestGuid,
                                    const boost::filesystem::path& destinationFullPath,
                                    unsigned int fileSize,
                                    const boost::shared_ptr<std::string>& chunkData,
                                    int timeoutSeconds = 60000) = 0;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
