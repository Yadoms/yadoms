#include "stdafx.h"
#include "HttpController.h"
#include <shared/Log.h>
#include <shared/http/HttpRestHelpers.h>
#include "shared/StringExtension.h"


void CHttpController::sendCommand(const std::string& ip, const std::string& commandMode, const std::string& commandData) const
{
   try
   {
      std::ostringstream cmd;
      cmd << "http://" << ip;
      cmd << "/control?cmd=" << commandMode;
      cmd << "," << commandData;
      std::string urlToCall = cmd.str();
      urlToCall = processText(urlToCall);
      shared::http::CHttpRestHelpers::sendGetRequest(urlToCall);
   }
   catch (std::exception& ex)
   {
      YADOMS_LOG(error) << "Fail to send command : " << ex.what();
   }
   catch (...)
   {
      YADOMS_LOG(error) << "Fail to send command : unknown error";
   }
}

std::string CHttpController::processText(const std::string& text)
{
   std::string t = boost::replace_all_copy(text, " ", "%20");
   return t;
}
