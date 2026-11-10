#include "stdafx.h"
#include "AcceptedAnswer.h"

using namespace web::rest;

CAcceptedAnswer::CAcceptedAnswer(const std::string& resourceCreatedPath)
   : m_customHeaders(std::make_shared<std::map<std::string, std::string>>())
{
   if (!resourceCreatedPath.empty())
      (*m_customHeaders)["Location"] = resourceCreatedPath;
}

shared::http::ECodes CAcceptedAnswer::code() const
{
   return shared::http::ECodes::kAccepted;
}

std::string CAcceptedAnswer::body() const
{
   return {};
}

bool CAcceptedAnswer::bodyIsFile() const
{
   return false;
}

EContentType CAcceptedAnswer::contentType() const
{
   return EContentType::kNone;
}

std::shared_ptr<std::map<std::string, std::string>> CAcceptedAnswer::customHeaders() const
{
   return m_customHeaders;
}
