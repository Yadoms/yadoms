#include "stdafx.h"
#include "OpenZWaveHttpClient.h"
#include <shared/web/FileDownloader.h>

bool COpenZWaveHttpClient::StartDownload(OpenZWave::Internal::HttpDownload* transfer)
{
   boost::thread(&COpenZWaveHttpClient::downloadAsync, this, transfer);
   return true;
}

void COpenZWaveHttpClient::downloadAsync(OpenZWave::Internal::HttpDownload* transfer)
{

   try
   {
      shared::web::CFileDownloader::downloadFile(transfer->url, boost::filesystem::path(transfer->filename),
         [&](const std::string& file, float progress) -> void
         {

         });
      transfer->transferStatus = OpenZWave::Internal::HttpDownload::Ok;
   }
   catch (...)
   {
      transfer->transferStatus = OpenZWave::Internal::HttpDownload::Failed;
   }

   this->FinishDownload(transfer);
}