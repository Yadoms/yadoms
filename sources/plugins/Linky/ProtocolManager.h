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
   ///\brief                     return if the protocol is validated
   //-----------------------------------------------------
   bool isValidated();

   //-----------------------------------------------------
   ///\brief                     get the protocol used
   //-----------------------------------------------------
   EProtocolType getProtocol();

   //-----------------------------------------------------
   ///\brief                     change the protocol to test into the list
   //-----------------------------------------------------
   void changeProtocol();

   //-----------------------------------------------------
   ///\brief                     return true, if end of the list
   //-----------------------------------------------------
   bool end();

private:
   bool m_firstRun;
   EProtocolType m_protocoleSelected;
};