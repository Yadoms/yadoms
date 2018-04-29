#pragma once

typedef enum {
   NoProtocol,
   Standard,
   Historic
}EProtocolType;

//-----------------------------------------------------
///\brief Protocol Selection Manager
//-----------------------------------------------------
class CProtocolManager
{
public:
   //-----------------------------------------------------
   ///\brief                     Constructor
   ///\param[in] keywordName     Yadoms keyword name
   //-----------------------------------------------------
   explicit CProtocolManager();

   //-----------------------------------------------------
   ///\brief                     Destructor
   //-----------------------------------------------------
   virtual ~CProtocolManager();

   //-----------------------------------------------------
   ///\brief                     validate Protocol
   //-----------------------------------------------------
   void validateProtocol();

   //-----------------------------------------------------
   ///\brief                     get the protocol used
   //-----------------------------------------------------
   EProtocolType getProtocol();

   //-----------------------------------------------------
   ///\brief                     change the protocol to test into the list
   //-----------------------------------------------------
   void changeProtocol();

private:
   bool m_firstRun;
   EProtocolType m_protocoleSelected;
};