#pragma once
#include <string>


class IPendingMail
{
public:
   virtual ~IPendingMail() = default;

   virtual int recipientId() = 0;
   virtual std::string body() = 0;
   virtual bool retry() = 0;
};
