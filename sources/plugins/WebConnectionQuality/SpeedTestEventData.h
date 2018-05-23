#pragma once

class CSpeedTestEventData
{
public:
   CSpeedTestEventData(int returnCode,
                       const std::string& error);
   virtual ~CSpeedTestEventData();

   bool succes() const;
   int returnCode() const;
   const std::string& error() const;

private:
   int m_returnCode;
   const std::string m_error;
};
