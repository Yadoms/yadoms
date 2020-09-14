#pragma once
#include "Http.h"
#include <shared/http/FileDownloader.h>

class COpenZWaveHttpClient : public OpenZWave::Internal::i_HttpClient
{
public:
   bool StartDownload(OpenZWave::Internal::HttpDownload* transfer) override;

private:
   void downloadAsync(OpenZWave::Internal::HttpDownload* transfer);
};

