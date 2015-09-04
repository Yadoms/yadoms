#pragma once

#include <shared/script/yScriptApi/IYScriptApi.h>

const size_t m_messageQueueMessageSize(1000);


//--------------------------------------------------------------
/// \brief Messages requests identifiers
//--------------------------------------------------------------
enum ERequestIdentifier
{
   kReqReadKeyword = 0,
   kReqWaitForAcquisition,
   kReqWaitForAcquisitions,
   kReqWriteKeyword,
   kReqSendNotification,
   kReqGetInfo,
   kReqRuleEnable,
};

//--------------------------------------------------------------
/// \brief Messages answers identifiers
//--------------------------------------------------------------
enum EAnswerIdentifier
{
   kAnsReadKeyword = 0,
   kAnsWaitForAcquisition,
   kAnsWaitForAcquisitions,
   kAnsGetInfo,
};

//--------------------------------------------------------------
/// \brief The read keyword request
//--------------------------------------------------------------
class CReqReadKeyword
{
public:
   CReqReadKeyword() {}
   virtual ~CReqReadKeyword() {}

   int m_keywordId;

private:
   friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive& ar, const unsigned int version)
   {
      ar & m_keywordId;
   }
};

//--------------------------------------------------------------
/// \brief The wait for acquisition request
//--------------------------------------------------------------
class CReqWaitForAcquisition
{
public:
   CReqWaitForAcquisition() {}
   virtual ~CReqWaitForAcquisition() {}

   int m_keywordId;
   std::string m_timeout;

private:
   friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive& ar, const unsigned int version)
   {
      ar & m_keywordId;
      ar & m_timeout;
   }
};

//--------------------------------------------------------------
/// \brief The wait for acquisitions request
//--------------------------------------------------------------
class CReqWaitForAcquisitions
{
public:
   CReqWaitForAcquisitions() {}
   virtual ~CReqWaitForAcquisitions() {}

   std::vector<int> m_keywordIdList;
   std::string m_timeout;

private:
   friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive& ar, const unsigned int version)
   {
      ar & m_keywordIdList;
      ar & m_timeout;
   }
};

//--------------------------------------------------------------
/// \brief The write keyword request
//--------------------------------------------------------------
class CReqWriteKeyword
{
public:
   CReqWriteKeyword() {}
   virtual ~CReqWriteKeyword() {}

   int m_keywordId;
   std::string m_newState;

private:
   friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive& ar, const unsigned int version)
   {
      ar & m_keywordId;
      ar & m_newState;
   }
};

//--------------------------------------------------------------
/// \brief The send notification request
//--------------------------------------------------------------
class CReqSendNotification
{
public:
   CReqSendNotification() {}
   virtual ~CReqSendNotification() {}

   int m_keywordId;
   int m_recipientId;
   std::string m_message;

private:
   friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive& ar, const unsigned int version)
   {
      ar & m_keywordId;
      ar & m_recipientId;
      ar & m_message;
   }
};

//--------------------------------------------------------------
/// \brief The get info request
//--------------------------------------------------------------
class CReqGetInfo
{
public:
   CReqGetInfo() {}
   virtual ~CReqGetInfo() {}

   shared::script::yScriptApi::IYScriptApi::EInfoKeys m_key;

private:
   friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive& ar, const unsigned int version)
   {
      ar & m_key;
   }
};

//--------------------------------------------------------------
/// \brief The rule enable request
//--------------------------------------------------------------
class CReqRuleEnable
{
public:
   CReqRuleEnable() {}
   virtual ~CReqRuleEnable() {}

   bool m_enable;

private:
   friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive& ar, const unsigned int version)
   {
      ar & m_enable;
   }
};

//--------------------------------------------------------------
/// \brief The fail request
//--------------------------------------------------------------
class CReqFail
{
public:
   CReqFail() {}
   virtual ~CReqFail() {}

   std::string m_errorMessage;

private:
   friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive& ar, const unsigned int version)
   {
      ar & m_errorMessage;
   }
};

//--------------------------------------------------------------
/// \brief The read keyword answer
//--------------------------------------------------------------
struct CAnsReadKeyword
{
public:
   CAnsReadKeyword() {}
   virtual ~CAnsReadKeyword() {}

   std::string m_returnValue;

private:
   friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive& ar, const unsigned int version)
   {
      ar & m_returnValue;
   }
};

//--------------------------------------------------------------
/// \brief The wait for acquisition answer
//--------------------------------------------------------------
class CAnsWaitForAcquisition
{
public:
   CAnsWaitForAcquisition() {}
   virtual ~CAnsWaitForAcquisition() {}

   std::string m_returnValue;

private:
   friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive& ar, const unsigned int version)
   {
      ar & m_returnValue;
   }
};

//--------------------------------------------------------------
/// \brief The wait for acquisitions answer
//--------------------------------------------------------------
class CAnsWaitForAcquisitions
{
public:
   CAnsWaitForAcquisitions() {}
   virtual ~CAnsWaitForAcquisitions() {}

   std::pair<int, std::string> m_returnValue;

private:
   friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive& ar, const unsigned int version)
   {
      ar & m_returnValue;
   }
};

//--------------------------------------------------------------
/// \brief The get info answer
//--------------------------------------------------------------
struct CAnsGetInfo
{
public:
   CAnsGetInfo() {}
   virtual ~CAnsGetInfo() {}

   std::string m_returnValue;

private:
   friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive& ar, const unsigned int version)
   {
      ar & m_returnValue;
   }
};

//TODO virer tout ce fichier et remplacer par des CDataContainer ?