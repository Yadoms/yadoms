#pragma once
#include <string>

namespace web
{
   namespace rest
   {
      namespace service
      {
         //----------------------------
         ///\brief   Interface for unique long task handling
         //----------------------------
         class ITaskInProgressHandler
         {
         public:
            virtual ~ITaskInProgressHandler() = default;

            virtual std::string inProgressTaskUid() = 0;
            virtual void setInProgressTaskUid(const std::string& taskUid) = 0;
         };
      } //namespace service
   } //namespace rest
} //namespace web 
