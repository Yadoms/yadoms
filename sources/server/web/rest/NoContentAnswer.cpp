#include "stdafx.h"
#include "NoContentAnswer.h"

using namespace web::rest;

shared::http::ECodes CNoContentAnswer::code() const
{
   return shared::http::ECodes::kNoContent;
}

std::string CNoContentAnswer::body() const
{
   return {};
}

bool CNoContentAnswer::bodyIsFile() const
{
   return false;
}

EContentType CNoContentAnswer::contentType() const
{
   return EContentType::kNone;
}

std::shared_ptr<std::map<std::string, std::string>> CNoContentAnswer::customHeaders() const
{
   return nullptr;
}
