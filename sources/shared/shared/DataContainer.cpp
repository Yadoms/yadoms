#include "stdafx.h"
#include "DataContainer.h"
#include <shared/exception/JSONParse.hpp>
#include "exception/EmptyResult.hpp"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/pointer.h"
#include "rapidjson/error/error.h"
#include "rapidjson/error/en.h"

namespace shared
{
	const CDataContainerSharedPtr CDataContainer::EmptyContainerSharedPtr = boost::make_shared<CDataContainer>();
	const CDataContainer CDataContainer::EmptyContainer = CDataContainer();


	CDataContainer::CDataContainer()
	{
		m_tree.SetObject();

	}

	CDataContainer::CDataContainer(const std::string & initialData)
	{
		m_tree.SetObject();
		CDataContainer::deserialize(initialData);
	}

	CDataContainer::CDataContainer(rapidjson::Value & d)
	{
		m_tree.SetObject();
		rapidjson::Document::AllocatorType& a = m_tree.GetAllocator();
		m_tree.CopyFrom(d, a, true);
	}

	CDataContainer::CDataContainer(rapidjson::Value * d)
	{
		m_tree.SetObject();
		m_tree.CopyFrom(*d, m_tree.GetAllocator(), true);
	}

	CDataContainer::CDataContainer(rapidjson::Document & d)
	{
		m_tree.SetObject();
		m_tree.CopyFrom(d, m_tree.GetAllocator(), true);
	}


	CDataContainer::CDataContainer(const std::map<std::string, std::string> & initialData)
	{
		m_tree.SetObject();
	   for (const auto& i : initialData)
	      set(i.first, i.second);
	}


	CDataContainer::~CDataContainer()
	{
		//m_tree.GetAllocator().Clear();
	}

	//CDataContainer::CDataContainer(const CDataContainer&& initialData)
	//{
	   
	//}

	CDataContainerSharedPtr CDataContainer::getChild(const std::string& parameterName, const char pathChar) const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);

		rapidjson::Value* found = findValue(parameterName, pathChar);
		if (found)
			return boost::make_shared<CDataContainer>(found);
		throw exception::CInvalidParameter(parameterName + " : is not found");
	}

	CDataContainer::CDataContainer(const CDataContainer & initialData)
	{
		 m_tree.SetObject();
		 m_tree.CopyFrom(initialData.m_tree, m_tree.GetAllocator(), true);
	}

	CDataContainer::CDataContainer(const rapidjson::Document & initialTree)
	{
		m_tree.SetObject();
		m_tree.CopyFrom(initialTree, m_tree.GetAllocator(), true);
	}

	CDataContainer & CDataContainer::operator =(const CDataContainer & rhs)
	{
		initializeWith(rhs);
		return *this;
	}


	CDataContainer & CDataContainer::operator=(const std::string &rhs)
	{
		deserialize(rhs);
		return *this;
	}



	std::ostream& operator << (std::ostream& os, const CDataContainer & dc)
	{
		os << dc.serialize();
		return os;
	}

	std::istream& operator >> (std::istream& is, CDataContainer & dc)
	{
		boost::lock_guard<boost::mutex> lock(dc.m_treeMutex);

		dc.m_tree.RemoveAllMembers();

		rapidjson::IStreamWrapper isw(is);

		dc.m_tree.ParseStream(isw);
		return is;
	}


	bool CDataContainer::exists(const std::string& parameterName, const char pathChar) const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);

		rapidjson::Value* found = findValue(parameterName, pathChar);
		return (found != nullptr);
	}


	bool CDataContainer::containsChild(const std::string& parameterName, const char pathChar) const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);
		rapidjson::Value* found = findValue(parameterName, pathChar);
		if (found)
			return !found->IsNull() && found->IsObject();
		return false;
	}

	bool CDataContainer::containsChildArray(const std::string& parameterName, const char pathChar) const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);
		rapidjson::Value* found = findValue(parameterName, pathChar);
		if (found)
			return !found->IsNull() && found->IsArray();
		return false;
	}

	bool CDataContainer::containsValue(const std::string& parameterName, const char pathChar) const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);

		rapidjson::Value* found = findValue(parameterName, pathChar);

		if (found != nullptr && !found->IsNull())
		{
			return found->IsBool() || found->IsDouble() || found->IsFloat() || found->IsInt() || found->IsInt64() ||
				found->IsNumber() || found->IsString() || found->IsUint() || found->IsUint64();
		}
		return false;
	}


	std::string CDataContainer::serialize() const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);

		rapidjson::StringBuffer sb;
		rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
		m_tree.Accept(writer);

		return sb.GetString();
	}

	void CDataContainer::deserialize(const std::string & data)
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);

		m_tree.RemoveAllMembers();
      const rapidjson::ParseResult parseError = m_tree.Parse(data.c_str());
		if (!parseError)
			throw exception::CJSONParse(rapidjson::GetParseError_En(parseError.Code()), parseError.Offset());
	}

	void CDataContainer::serializeToFile(const std::string & filename) const
	{
		std::ofstream ofs(filename);
		rapidjson::OStreamWrapper osw(ofs);
		rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
		m_tree.Accept(writer);
	}

	void CDataContainer::deserializeFromFile(const std::string & file)
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);

		m_tree.RemoveAllMembers();

      const auto fileSize(static_cast<size_t>(boost::filesystem::file_size(file)));
      const auto buffer(boost::make_shared<char[]>(fileSize));

	   const auto fp = fopen(file.c_str(), "rb");
	   rapidjson::FileReadStream bis(fp, static_cast<char*>(buffer.get()), fileSize);
	   rapidjson::AutoUTFInputStream<unsigned, rapidjson::FileReadStream> isw(bis);

		m_tree.ParseStream<0, rapidjson::AutoUTF<unsigned>>(isw);

      fclose(fp);
	}

	void CDataContainer::extractContent(CDataContainer & container) const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);
		container.m_tree.CopyFrom(m_tree, container.m_tree.GetAllocator(), true);
	}

	void CDataContainer::fillFromContent(const CDataContainer & initialData)
	{
		initializeWith(initialData);
	}




	bool CDataContainer::operator ==(const CDataContainer &rhs) const
	{
		return serialize() == rhs.serialize();
	}

	bool CDataContainer::operator !=(const CDataContainer &rhs) const
	{
		return serialize() != rhs.serialize();
	}

	void CDataContainer::initializeWith(const CDataContainer &rhs)
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);
		m_tree.CopyFrom(rhs.m_tree, m_tree.GetAllocator(), true);
	}


   void CDataContainer::initializeWith(CDataContainerSharedPtr &rhs)
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);
		m_tree.CopyFrom(rhs->m_tree, m_tree.GetAllocator(), true);
	}


	bool CDataContainer::empty() const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);
		return m_tree.MemberCount() == 0;
	}

	void CDataContainer::printToLog(std::ostream& os) const
	{
		os << std::endl;

		rapidjson::OStreamWrapper osw(os);
		rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer(osw);
		m_tree.Accept(writer);
	}

	void CDataContainer::printSizeToLog(std::ostream& os) const
	{
		//os << "Size=" << getPointer()->GetAllocator().Size() << " ChunkSize=" << getPointer()->GetAllocator().Capacity() << " StackCapcity=" << getPointer()->GetStackCapacity() << std::endl;
	}

	std::string CDataContainer::generatePath(const std::string & parameterName, const char pathChar)
   {
		std::string res = "/"; //pointer is still starting with /
	   const int c = parameterName.size();
		const char * s = parameterName.c_str();
		for (int i = 0; i < c; ++i)
		{
			//the slash is used as path separator in rapidjson
			//to use it in keys, it must be replaced by ~1 
			if (s[i] == '/')
				res += "~1";
			else
				res += (s[i] == pathChar) ? '/' : s[i];
		}
		return res;
	}

	void CDataContainer::set(const char* parameterName, const char* value, const char pathChar)
	{
	   const std::string strParamName(parameterName);
	   const std::string strValue(value);
		set<std::string>(strParamName, strValue, pathChar);
	}


	void CDataContainer::set(const std::string & parameterName, const char* value, const char pathChar)
	{
	   const std::string strValue(value);
		set<std::string>(parameterName, strValue, pathChar);
	}

	std::string CDataContainer::get(const std::string & parameterName, const char pathChar) const
	{
		return get<std::string>(parameterName, pathChar);
	}

	std::vector<std::string> CDataContainer::getKeys(const std::string& parameterName, const char pathChar) const
	{
		std::vector<std::string> result;
		rapidjson::Value* found = findValue(parameterName, pathChar);
		if (found)
		{
			for (rapidjson::Value::ConstMemberIterator i = found->MemberBegin(); i != found->MemberEnd(); ++i)
			{
				result.push_back(i->name.GetString());
			}
		}
		return result;
	}

	rapidjson::Value* CDataContainer::findValue(const std::string& parameterName, const char pathChar) const
	{
		if (parameterName.empty())
		{
			return const_cast<rapidjson::Document *>(&m_tree);
		}


      const auto path = generatePath(parameterName, pathChar);
		return const_cast<rapidjson::Value*>(rapidjson::Pointer(path.c_str()).Get(m_tree));
	}

	CDataContainer CDataContainer::find(const std::string& parameterName, const boost::function<bool(const CDataContainer&)> where_fct, const char pathChar) const
	{
		for (const auto& key : getKeys(parameterName, pathChar))
		{
			const auto path = parameterName + pathChar + key;
			const auto container = get<CDataContainer>(path);
			if (where_fct(container))
			{
				CDataContainer result;
				result.set(key, container);
				return result;
			}
		}
		throw exception::CEmptyResult("No parameter matches criteria");
	}

	void CDataContainer::mergeObjects(rapidjson::Value &dstObject, const rapidjson::Value &srcObject, rapidjson::Document::AllocatorType &allocator)
	{
		for (auto srcIt = srcObject.MemberBegin(); srcIt != srcObject.MemberEnd(); ++srcIt)
		{
			auto dstIt = dstObject.FindMember(srcIt->name);
			if (dstIt != dstObject.MemberEnd())
			{
				if (srcIt->value.IsArray())
				{
					for (auto arrayIt = srcIt->value.Begin(); arrayIt != srcIt->value.End(); ++arrayIt)
					{
                  rapidjson::Value v(*arrayIt, allocator);
						dstIt->value.PushBack(v, allocator);
					}
				}
				else if (srcIt->value.IsObject())
				{
					mergeObjects(dstIt->value, srcIt->value, allocator);
				}
				else
				{
					//check type match, but also check some specific cases (kTrueType and kFalseType are booleans;
					//and many number combination can also match)
					//string cases : string should be copied (not just reference copy)
					if ( (srcIt->value.GetType() == dstIt->value.GetType() && !srcIt->value.IsString()) ||
						(srcIt->value.IsBool() && dstIt->value.IsBool()) ||
						(srcIt->value.IsNumber() && dstIt->value.IsNumber()))
					{
						dstIt->value.CopyFrom(srcIt->value, allocator);
					}
					else
					{
						//types don't match, try to make concordance
						switch (dstIt->value.GetType())
						{
						case rapidjson::kFalseType:
						case rapidjson::kTrueType:
							dstIt->value.SetBool(convertToBool(srcIt->value));
							break;
						case rapidjson::kStringType:
							dstIt->value.SetString(convertToString(srcIt->value), allocator);
							break;
						case rapidjson::kNumberType:
							if (dstIt->value.IsInt())
								dstIt->value.SetInt(convertToInt(srcIt->value));
							else if (dstIt->value.IsInt64())
								dstIt->value.SetInt64(convertToInt64(srcIt->value));
							else if (dstIt->value.IsUint())
								dstIt->value.SetUint(convertToUInt(srcIt->value));
							else if (dstIt->value.IsUint64())
								dstIt->value.SetUint64(convertToUInt64(srcIt->value));
							else if (dstIt->value.IsDouble())
								dstIt->value.SetDouble(convertToDouble(srcIt->value));
							else if (dstIt->value.IsFloat())
								dstIt->value.SetFloat(convertToFloat(srcIt->value));
							else
								throw exception::CInvalidParameter("Value is not a valid type");
							break;
						default:
							throw exception::CInvalidParameter("Value is not a valid type");
						}
					}
				}
			}
			else
			{
				//make local Values (which are copies of key and value)
				//-> ensure data is correctly copied (AddMember takes key and value ownership to dstObject)
				rapidjson::Value key; 
				key.CopyFrom(srcIt->name, allocator);

				rapidjson::Value val;                             
				val.CopyFrom(srcIt->value, allocator);

				dstObject.AddMember(key, val, allocator);
			}
		}
	}

	void CDataContainer::mergeFrom(const CDataContainer& source)
	{
		auto& allocator = m_tree.GetAllocator();
		mergeObjects(m_tree, source.m_tree, allocator);
	}

	void CDataContainer::mergeFrom(CDataContainerSharedPtr& from)
	{
		auto& allocator = m_tree.GetAllocator();
		mergeObjects(m_tree, from.get()->m_tree, allocator);
	}

	void CDataContainer::setNull(const std::string& parameterName, const char pathChar) const
   {
		auto v = this->findValue(parameterName, pathChar);
		if (v)
			v->SetNull();
		throw exception::CInvalidParameter(parameterName + " : is not found");
	}

	bool CDataContainer::isNull(const std::string& parameterName, const char pathChar) const
	{
	   const auto v = this->findValue(parameterName, pathChar);
		if (v)
			return v->IsNull();
		throw exception::CInvalidParameter(parameterName + " : is not found");
	}

	bool CDataContainer::createArray(const std::string& parameterName, const char pathChar)
	{
		rapidjson::Value& v = rapidjson::Pointer(generatePath(parameterName, pathChar).c_str()).Create(m_tree).SetArray();
		return v.IsArray();
	}

	bool CDataContainer::isArray(const std::string& parameterName, char pathChar)
	{
		const auto ptr = rapidjson::Pointer(generatePath(parameterName, pathChar)).Get(m_tree);
		if (ptr != NULL && ptr->IsArray())
		{
			return true;
		}
		return false;
	}

	void CDataContainer::appendArray(const char* parameterName, const char* value, const char pathChar)
	{
		const std::string strParamName(parameterName);
		const std::string strValue(value);
		appendArray<std::string>(strParamName, strValue, pathChar);
	}


	void CDataContainer::appendArray(const std::string& parameterName, const char* value, const char pathChar)
	{
		const std::string strValue(value);
		appendArray<std::string>(parameterName, strValue, pathChar);
	}

	rapidjson::Document * CDataContainer::getPointer() const
	{
		return const_cast<rapidjson::Document *>(&m_tree);
	}

	std::string CDataContainer::convertToString(const rapidjson::Value& v)
	{
		if (v.IsString())
			return std::string(v.GetString());

		if (v.IsBool())
			return v.GetBool() ? "true" : "false";

		if (v.IsDouble())
			return boost::lexical_cast<std::string>(v.GetDouble());
		if (v.IsFalse())
			return "false";
		if (v.IsFloat())
			return boost::lexical_cast<std::string>(v.GetFloat());
		if (v.IsInt())
			return boost::lexical_cast<std::string>(v.GetInt());
		if (v.IsInt64())
			return boost::lexical_cast<std::string>(v.GetInt64());
		if (v.IsTrue())
			return "true";
		if (v.IsUint())
			return boost::lexical_cast<std::string>(v.GetUint());
		if (v.IsUint64())
			return boost::lexical_cast<std::string>(v.GetUint64());

		throw exception::CInvalidParameter("Value is not a valid type");
	}

	bool CDataContainer::convertToBool(const rapidjson::Value & v)
	{
		if (v.IsString()) {
			const std::string s = v.GetString();
			return boost::iequals(s, "true") || boost::iequals(s, "1");
		}
		if (v.IsBool())
			return v.GetBool();
		if (v.IsDouble())
			return v.GetDouble() != 0.0f;
		if (v.IsFalse())
			return false;
		if (v.IsFloat())
			return v.GetFloat() != 0.0f;
		if (v.IsInt())
			return v.GetInt() != 0;
		if (v.IsInt64())
			return v.GetInt64() != 0;
		if (v.IsTrue())
			return true;
		if (v.IsUint())
			return v.GetUint() != 0;
		if (v.IsUint64())
			return v.GetUint64() != 0;

		throw exception::CInvalidParameter("Value is not a valid type");
	}


	int CDataContainer::convertToInt(const rapidjson::Value& v)
	{
		if (v.IsInt())
			return v.GetInt();

		if (v.IsUint())
		{
			const unsigned int b = v.GetUint();
			if (b< 0 || b>INT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to int32") % b).str());
			return static_cast<int>(b);
		}

		if (v.IsInt64())
		{
			const int64_t b = v.GetInt64();
			if (b< INT32_MIN || b>INT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to int32") % b).str());
			return static_cast<int>(b);
		}

		if (v.IsUint64())
		{
			const uint64_t b = v.GetUint64();
			if (b< 0 || b>INT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to int32") % b).str());
			return static_cast<int>(b);
		}

		if (v.IsDouble())
		{
			const double b = v.GetDouble();
			if (b< INT32_MIN || b>INT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to int32") % b).str());
			return static_cast<int>(b);
		}

		if (v.IsFloat())
		{
			const float b = v.GetFloat();
			if (b< INT32_MIN || b>INT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to int32") % b).str());
			return static_cast<int>(b);
		}


		if (v.IsString()) {
			const std::string s = v.GetString();
			return boost::lexical_cast<int>(s);
		}

		if (v.IsBool())
			return v.GetBool() ? 1 : 0;

		if (v.IsFalse())
			return 0;

		if (v.IsTrue())
			return 1;

		throw exception::CInvalidParameter("Value is not a valid type");
	}

	int64_t CDataContainer::convertToInt64(const rapidjson::Value & v)
	{
		if (v.IsInt64())
			return v.GetInt64();

		if (v.IsUint64())
		{
			const uint64_t b = v.GetUint64();
			if (b > INT64_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to int64") % b).str());
			return static_cast<int64_t>(b);
		}

		if (v.IsUint())
		{
			const unsigned int b = v.GetUint();
			return static_cast<int64_t>(b);
		}

		if (v.IsInt())
		{
			const int b = v.GetInt();
			return static_cast<int64_t>(b);
		}

		if (v.IsDouble())
		{
			const double b = v.GetDouble();
			if (b< INT64_MIN || b>INT64_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to int64") % b).str());
			if (b > nextafter(INT64_MAX, 0))
				throw exception::COutOfRange((boost::format("%1% is not assignable to int64 : overflow") % b).str());
			return static_cast<int64_t>(b);
		}

		if (v.IsFloat())
		{
			const float b = v.GetFloat();
			if (b< INT64_MIN || b>INT64_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to int64") % b).str());
			return static_cast<int64_t>(b);
		}

		if (v.IsString()) {
			const std::string s = v.GetString();
			return boost::lexical_cast<int64_t>(s);
		}
		if (v.IsBool())
			return v.GetBool() ? 1 : 0;

		if (v.IsFalse())
			return 0;

		if (v.IsTrue())
			return 1;

		throw exception::CInvalidParameter("Value is not a valid type");
	}

	char CDataContainer::convertToByte(const rapidjson::Value& v)
	{
		if (v.IsDouble())
		{
			const double b = v.GetDouble();
			if (b< INT8_MIN || b>INT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to char/int8") % b).str());
			return static_cast<char>(b);
		}

		if (v.IsFloat())
		{
			const float b = v.GetFloat();
			if (b< INT8_MIN || b>INT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to char/int8") % b).str());
			return static_cast<char>(b);
		}

		if (v.IsInt())
		{
			const int b = v.GetInt();
			if (b< INT8_MIN || b>INT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to char/int8") % b).str());
			return static_cast<char>(b);
		}

		if (v.IsUint())
		{
			const unsigned int b = v.GetUint();
			if (b > INT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to char/int8") % b).str());
			return static_cast<char>(b);
		}

		if (v.IsInt64())
		{
			const int64_t b = v.GetInt64();
			if (b< INT8_MIN || b>INT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to char/int8") % b).str());
			return static_cast<char>(b);
		}

		if (v.IsUint64())
		{
			const uint64_t b = v.GetUint64();
			if (b>INT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to char/int8") % b).str());
			return static_cast<char>(b);
		}

		if (v.IsString()) {
			std::string s = v.GetString();
			return s[0];
		}

		if (v.IsBool())
			return v.GetBool() ? 1 : 0;

		if (v.IsFalse())
			return 0;

		if (v.IsTrue())
			return 1;

		throw exception::CInvalidParameter("Value is not a valid type");
	}

	short CDataContainer::convertToShort(const rapidjson::Value& v)
	{
		if (v.IsInt())
		{
			const int b = v.GetInt();
			if (b< INT16_MIN || b>INT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to signed Short/Int16") % b).str());
			return static_cast<short>(b);
		}
		if (v.IsUint())
		{
			const unsigned int b = v.GetUint();
			if (b>INT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to signed Short/Int16") % b).str());
			return static_cast<short>(b);
		}
		if (v.IsInt64())
		{
			const int64_t b = v.GetInt64();
			if (b< INT16_MIN || b>INT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to signed Short/Int16") % b).str());
			return static_cast<short>(b);
		}
		if (v.IsUint64())
		{
			const uint64_t b = v.GetUint64();
			if (b>INT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to signed Short/Int16") % b).str());
			return static_cast<short>(b);
		}
		if (v.IsDouble())
		{
			const double b = v.GetDouble();
			if (b< INT16_MIN || b>INT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to signed Short/Int16") % b).str());
			return static_cast<short>(b);
		}
		if (v.IsFloat())
		{
			const float b = v.GetFloat();
			if (b< INT16_MIN || b>INT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to signed Short/Int16") % b).str());
			return static_cast<short>(b);
		}

		if (v.IsString())
		{
			const std::string s = v.GetString();
			return boost::lexical_cast<short>(s);
		}

		if (v.IsBool())
			return v.GetBool() ? 1 : 0;
		if (v.IsFalse())
			return 0;
		if (v.IsTrue())
			return 1;

		throw exception::CInvalidParameter("Value is not a valid type");
	}

	unsigned int CDataContainer::convertToUInt(const rapidjson::Value& v)
	{
		if (v.IsUint())
			return v.GetUint();

		if (v.IsUint64())
		{
			const uint64_t b = v.GetUint64();
			if (b< 0 || b>UINT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int/UInt32") % b).str());
			return static_cast<unsigned int>(b);
		}

		if (v.IsInt64())
		{
			const int64_t b = v.GetInt64();
			if (b< 0 || b>UINT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int/UInt32") % b).str());
			return static_cast<unsigned int>(b);
		}

		if (v.IsInt())
		{
			const int b = v.GetInt();
			if (b< 0 || b>INT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int/UInt32") % b).str());
			return static_cast<unsigned int>(b);
		}

		if (v.IsDouble())
		{
			const double b = v.GetDouble();
			if (b< 0 || b>UINT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int/UInt32") % b).str());
			return static_cast<unsigned int>(b);
		}

		if (v.IsFloat())
		{
			const float b = v.GetFloat();
			if (b< 0 || b>UINT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int/UInt32") % b).str());
			return static_cast<unsigned int>(b);
		}

		if (v.IsString()) {
			const std::string s = v.GetString();
			return boost::lexical_cast<unsigned int>(s);
		}

		if (v.IsBool())
			return v.GetBool() ? 1 : 0;

		if (v.IsFalse())
			return 0;

		if (v.IsTrue())
			return 1;

		throw exception::CInvalidParameter("Value is not a valid type");
	}
	uint64_t CDataContainer::convertToUInt64(const rapidjson::Value& v)
	{
		if (v.IsUint64())
			return v.GetUint64();		

		if (v.IsInt64())
		{
			const int64_t b = v.GetInt64();
			if (b< 0 || b>INT64_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int 64/UInt64") % b).str());
			return static_cast<uint64_t>(b);
		}

		if (v.IsUint())
		{
			const unsigned int b = v.GetUint();
			return static_cast<uint64_t>(b);
		}

		if (v.IsInt())
		{
			const int b = v.GetInt();
			if (b< 0)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int 64/UInt64") % b).str());
			return static_cast<uint64_t>(b);
		}

		if (v.IsDouble())
		{
			const double b = v.GetDouble();
			if (b< 0 || b>UINT64_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int 64/UInt64") % b).str());
			if (b > nextafter(UINT64_MAX, 0))
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int 64/UInt64 : overflow") % b).str());
			return static_cast<uint64_t>(b);
		}

		if (v.IsFloat())
		{
			const float b = v.GetFloat();
			if (b< 0 || b>UINT64_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int 64/UInt64") % b).str());
			return static_cast<uint64_t>(b);
		}

		if (v.IsString()) 
		{
			const std::string s = v.GetString();
			return boost::lexical_cast<uint64_t>(s);
		}
			
		if (v.IsBool())
			return v.GetBool() ? 1 : 0;
			
		if (v.IsFalse())
			return 0;
			
		if (v.IsTrue())
			return 1;
		throw exception::CInvalidParameter("Value is not a valid type");
	}

	unsigned char CDataContainer::convertToUByte(const rapidjson::Value& v)
	{
		if (v.IsInt())
		{
			const int b = v.GetInt();
			if (b< 0 || b>UINT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned char/uint8") % b).str());
			return static_cast<unsigned char>(b);
		}
		if (v.IsInt64())
		{
			const int64_t b = v.GetInt64();
			if (b< 0 || b>UINT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned char/uint8") % b).str());
			return static_cast<unsigned char>(b);
		}
		if (v.IsUint())
		{
			const unsigned int b = v.GetUint();
			if (b< 0 || b>UINT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned char/uint8") % b).str());
			return static_cast<unsigned char>(b);
		}

		if (v.IsUint64())
		{
			const uint64_t b = v.GetUint64();
			if (b< 0 || b>UINT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned char/uint8") % b).str());
			return static_cast<unsigned char>(b);
		}

		if (v.IsDouble())
		{
			const double b = v.GetDouble();
			if (b< 0 || b>UINT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned char/uint8") % b).str());
			return static_cast<unsigned char>(b);
		}
		if (v.IsFloat())
		{
			const float b = v.GetFloat();
			if (b< 0 || b>UINT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned char/uint8") % b).str());
			return static_cast<unsigned char>(b);
		}

		if (v.IsString()) {
			const std::string s = v.GetString();
			return boost::lexical_cast<unsigned char>(s);
		}
		if (v.IsBool())
			return v.GetBool() ? 1 : 0;
		if (v.IsFalse())
			return 0;
		if (v.IsTrue())
			return 1;

		throw exception::CInvalidParameter("Value is not a valid type");
	}


	unsigned short CDataContainer::convertToUShort(const rapidjson::Value& v)
	{
		if (v.IsUint())
		{
			const unsigned int b = v.GetUint();
			if (b< 0 || b>UINT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned short/uint16") % b).str());
			return static_cast<unsigned short>(b);
		}

		if (v.IsUint64())
		{
			const uint64_t b = v.GetUint64();
			if (b< 0 || b>UINT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned short/uint16") % b).str());
			return static_cast<unsigned short>(b);
		}

		if (v.IsInt())
		{
			const int b = v.GetInt();
			if (b< 0 || b>UINT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned short/uint16") % b).str());
			return static_cast<unsigned short>(b);
		}
		if (v.IsInt64())
		{
			const int64_t b = v.GetInt64();
			if (b< 0 || b>UINT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned short/uint16") % b).str());
			return static_cast<unsigned short>(b);
		}

		if (v.IsDouble())
		{
			const double b = v.GetDouble();
			if (b< 0 || b>UINT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned short/uint16") % b).str());
			return static_cast<unsigned short>(b);
		}
		if (v.IsFloat())
		{
			const float b = v.GetFloat();
			if (b< 0 || b>UINT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned short/uint16") % b).str());
			return static_cast<unsigned short>(b);
		}

		if (v.IsString()) {
			const std::string s = v.GetString();
			return boost::lexical_cast<unsigned short>(s);
		}

		if (v.IsBool())
			return v.GetBool() ? 1 : 0;

		if (v.IsTrue())
			return 1;

		if (v.IsFalse())
			return 0;

		throw exception::CInvalidParameter("Value is not a valid type");
	}

	float CDataContainer::convertToFloat(const rapidjson::Value& v)
	{
		if (v.IsFloat())
			return v.GetFloat();

		if (v.IsDouble())
		{
			const double b = v.GetDouble();
			if (b< -FLT_MAX || b>FLT_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to float/single") % b).str());
			return static_cast<float>(b);
		}

		if (v.IsInt())
		{
			const int b = v.GetInt();
			if (b< -FLT_MAX || b > FLT_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to float/single") % b).str());
			return static_cast<float>(b);
		}

		if (v.IsUint())
		{
			const unsigned int b = v.GetUint();
			if (b< -FLT_MAX || b>FLT_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to float/single") % b).str());
			return static_cast<float>(b);
		}

		if (v.IsInt64())
		{
			const int64_t b = v.GetInt64();
			if (b< -FLT_MAX || b>FLT_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to float/single") % b).str());
			return static_cast<float>(b);
		}

		if (v.IsUint64())
		{
			const uint64_t b = v.GetUint64();
			if (b< -FLT_MAX || b>FLT_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to float/single") % b).str());
			return static_cast<float>(b);
		}
					   
		if (v.IsString()) {
			const std::string s = v.GetString();
			return boost::lexical_cast<float>(s);
		}

		if (v.IsBool())
			return v.GetBool() ? 1.0f : 0.0f;
			
		if (v.IsFalse())
			return 0.0f;
			
		if (v.IsTrue())
			return 1.0f;

		throw exception::CInvalidParameter("Value is not a valid type");
	}

	double CDataContainer::convertToDouble(const rapidjson::Value& v)
	{
		if (v.IsDouble())
			return v.GetDouble();

		if (v.IsFloat())
		{
			const float b = v.GetFloat();
			if (b< -DBL_MAX || b>DBL_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to double") % b).str());
			return static_cast<double>(b);
		}

		if (v.IsInt())
		{
			const int b = v.GetInt();
			if (b< -DBL_MAX || b>DBL_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to double") % b).str());
			return static_cast<double>(b);
		}

		if (v.IsUint())
		{
			const unsigned int b = v.GetUint();
			if (b< -DBL_MAX || b>DBL_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to double") % b).str());
			return static_cast<double>(b);
		}

		if (v.IsInt64())
		{
			const int64_t b = v.GetInt64();
			if (b< -DBL_MAX || b>DBL_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to double") % b).str());
			return static_cast<double>(b);
		}

		if (v.IsUint64())
		{
			const uint64_t b = v.GetUint64();
			if (b< -DBL_MAX || b>DBL_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to double") % b).str());
			return static_cast<double>(b);
		}

		if (v.IsString()) {
			const std::string s = v.GetString();
			return boost::lexical_cast<double>(s);
		}
		if (v.IsBool())
			return v.GetBool() ? 1.0f : 0.0f;

		if (v.IsFalse())
			return 0.0f;
			
		if (v.IsTrue())
			return 1.0f;

		throw exception::CInvalidParameter("Value is not a valid type");
	}
} // namespace shared
