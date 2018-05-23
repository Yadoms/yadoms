#pragma once

class CSpeedTestEventData
{
public:
   CSpeedTestEventData(int returnCode,
                       const std::string& error,
                       const std::string& result);
   virtual ~CSpeedTestEventData();

   bool success() const;
   int returnCode() const;
   const std::string& error() const;
   const std::string& result() const;

private:
   int m_returnCode;
   const std::string m_error;
   const std::string m_result;
};
