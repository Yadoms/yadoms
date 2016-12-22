#pragma once

//--------------------------------------------------------------
/// \brief	An EnOcean profile helper
//--------------------------------------------------------------
class CProfileHelper
{
public:
   //--------------------------------------------------------------
   /// \brief	Constructor
   /// \param[in] profile     Profile as string (ex: "A5-02-05")
   //--------------------------------------------------------------
   explicit CProfileHelper(const std::string& profile);
   CProfileHelper(unsigned int rorg,
                  unsigned int func,
                  unsigned int type);

   virtual ~CProfileHelper();

   unsigned int rorg() const;
   unsigned int func() const;
   unsigned int type() const;
   std::string profile() const;

   static std::string byteToHexString(unsigned int value);

private:
   unsigned int m_rorg;
   unsigned int m_func;
   unsigned int m_type;
};
