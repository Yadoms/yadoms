#include "stdafx.h"
#include "DataContainer.h"
#include <shared/exception/JSONParse.hpp>
#include "exception/EmptyResult.hpp"
#include <boost/json.hpp>

namespace shared
{

	boost::shared_ptr<CDataContainer> CDataContainer::make()
	{
		return boost::make_shared<CDataContainer>();
	}

	boost::shared_ptr<CDataContainer> CDataContainer::make(const std::string& initialData)
	{
		return boost::make_shared<CDataContainer>(initialData);
	}
	boost::shared_ptr<CDataContainer> CDataContainer::make(const std::map<std::string, std::string>& initialData)
	{
		return boost::make_shared<CDataContainer>(initialData);
	}

	boost::shared_ptr<CDataContainer> CDataContainer::make(const boost::json::object& d)
	{
		return boost::make_shared<CDataContainer>(d);
	}
	
	boost::shared_ptr<CDataContainer> CDataContainer::make(const boost::json::object* d)
	{
		return boost::make_shared<CDataContainer>(d);
	}
	

	const boost::shared_ptr<CDataContainer> CDataContainer::EmptyContainerSharedPtr = CDataContainer::make();
	const CDataContainer CDataContainer::EmptyContainer = CDataContainer();

	CDataContainer::CDataContainer()
	{

	}

	CDataContainer::CDataContainer(const std::string & initialData)
	{
      CDataContainer::deserialize(initialData);
	}

	CDataContainer::CDataContainer(const boost::json::object& d)
		: m_tree(d)
	{
	}

	CDataContainer::CDataContainer(const boost::json::object* d)
		: m_tree(*d)
	{
	}


	CDataContainer::CDataContainer(const std::map<std::string, std::string> & initialData)
	{
	   for (const auto& i : initialData)
	      set(i.first, i.second);
	}


	CDataContainer::~CDataContainer()
	{
	}

	boost::shared_ptr<CDataContainer> CDataContainer::getChild(const std::string& parameterName, char pathChar) const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);

		const boost::json::value* found = findValue(parameterName, pathChar);
		if (found)
			return boost::make_shared<CDataContainer>(found->as_object());
		throw exception::CInvalidParameter(parameterName + " : is not found");
	}

	CDataContainer::CDataContainer(const CDataContainer & initialData)
	   :m_tree(initialData.m_tree)
	{
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

	std::ostream& operator << (std::ostream& os, const boost::shared_ptr<CDataContainer>& dc)
	{
		os << dc->serialize();
		return os;
	}

	std::istream& operator >> (std::istream& is, CDataContainer & dc)
	{
		boost::lock_guard<boost::mutex> lock(dc.m_treeMutex);

      boost::json::stream_parser p;
		std::string line;
		boost::json::error_code ec;
		while (std::getline(is, line))
		{
			p.write(line, ec);
			if (ec)
				return is;
		}
		p.finish(ec);
		if (ec)
			return is;
		dc.m_tree = p.release().as_object();
		return is;
	}

	std::istream& operator>>(std::istream& is, boost::shared_ptr<CDataContainer>& dc)
	{
		boost::lock_guard<boost::mutex> lock(dc->m_treeMutex);

		boost::json::stream_parser p;
		std::string line;
		boost::json::error_code ec;
		while (std::getline(is, line))
		{
			p.write(line, ec);
			if (ec)
				return is;
		}
		p.finish(ec);
		if (ec)
			return is;
		dc->m_tree = p.release().as_object();
		return is;
	}


	bool CDataContainer::exists(const std::string& parameterName, const char pathChar) const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);

		auto p = generatePath(parameterName, pathChar);
		boost::json::value const * item = nullptr;
		for(size_t i=0; i<p.size()-1; ++i)
		{
			if (item == nullptr)
				item = m_tree.if_contains(p[i]);
			else if(item->is_object())
				item = item->as_object().if_contains(p[i]);
		}
		
		if (item == nullptr)
			return m_tree.contains(p.back());
		else if (item->is_object())
			return item->as_object().contains(p.back());

		return false;
	}


	bool CDataContainer::containsChild(const std::string& parameterName, const char pathChar) const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);
		const boost::json::value* found = findValue(parameterName, pathChar);
		if (found)
			return !found->is_null() && found->is_object();
		return false;
	}

	bool CDataContainer::containsChildArray(const std::string& parameterName, const char pathChar) const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);
		const boost::json::value* found = findValue(parameterName, pathChar);
		if (found)
			return !found->is_null() && found->is_array();
		return false;
	}

	bool CDataContainer::containsValue(const std::string& parameterName, const char pathChar) const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);

		const boost::json::value* found = findValue(parameterName, pathChar);

		if (found != nullptr && !found->is_null())
		{
			return found->is_bool() || found->is_double() || found->is_number() || found->is_int64() ||
				found->is_uint64() || found->is_string();
		}
		return false;
	}


	std::string CDataContainer::serialize() const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);

		return boost::json::serialize(m_tree);
	}

	void CDataContainer::deserialize(const std::string & data)
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);

		try
		{
			boost::json::error_code ec;
			boost::json::value jv = boost::json::parse(data, ec);
			if (ec)
				throw exception::CJSONParse(ec.message());
			if(!jv.is_object())
				throw exception::CJSONParse("root item is not a json object");
			m_tree = jv.as_object();
		}
		catch (std::bad_alloc const& e)
		{
			throw exception::CJSONParse(e.what());
		}
	}

	void CDataContainer::serializeToFile(const std::string & filename) const
	{
		std::ofstream os(filename);
		// Create a serializer
		boost::json::serializer sr;

		// Set the serializer up for our value
		sr.reset(&m_tree);

		// Loop until all output is produced.
		while (!sr.done())
		{
			// Use a local buffer to avoid allocation.
			char buf[BOOST_JSON_STACK_BUFFER_SIZE];

			// Fill our buffer with serialized characters and write it to the output stream.
			os << sr.read(buf);
		}
	}

	void CDataContainer::deserializeFromFile(const std::string & file)
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);
		std::ifstream is(file);

		boost::json::stream_parser p;
		std::string line;
		boost::json::error_code ec;
		while (std::getline(is, line))
		{
			p.write(line, ec);
			if (ec)
				throw exception::CJSONParse(ec.message());;
		}
		p.finish(ec);
		if (ec)
			throw exception::CJSONParse(ec.message());;
		m_tree = p.release().as_object();
	}

	void CDataContainer::extractContent(CDataContainer & container) const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);
		container.m_tree = m_tree;
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
		m_tree = rhs.m_tree;
	}


   void CDataContainer::initializeWith(const boost::shared_ptr<CDataContainer> &rhs)
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);
		m_tree = rhs->m_tree;
	}


	bool CDataContainer::empty() const
	{
		boost::lock_guard<boost::mutex> lock(m_treeMutex);
		return m_tree.empty();
	}

	void
		pretty_print(std::ostream& os, boost::json::value const& jv, std::string* indent = nullptr)
	{
		std::string indent_;
		if (!indent)
			indent = &indent_;
		switch (jv.kind())
		{
		case boost::json::kind::object:
		{
			os << "{\n";
			indent->append(4, ' ');
			auto const& obj = jv.get_object();
			if (!obj.empty())
			{
				auto it = obj.begin();
				for (;;)
				{
					os << *indent << boost::json::serialize(it->key()) << " : ";
					pretty_print(os, it->value(), indent);
					if (++it == obj.end())
						break;
					os << ",\n";
				}
			}
			os << "\n";
			indent->resize(indent->size() - 4);
			os << *indent << "}";
			break;
		}

		case boost::json::kind::array:
		{
			os << "[\n";
			indent->append(4, ' ');
			auto const& arr = jv.get_array();
			if (!arr.empty())
			{
				auto it = arr.begin();
				for (;;)
				{
					os << *indent;
					pretty_print(os, *it, indent);
					if (++it == arr.end())
						break;
					os << ",\n";
				}
			}
			os << "\n";
			indent->resize(indent->size() - 4);
			os << *indent << "]";
			break;
		}

		case boost::json::kind::string:
		{
			os << boost::json::serialize(jv.get_string());
			break;
		}

		case boost::json::kind::uint64:
			os << jv.get_uint64();
			break;

		case boost::json::kind::int64:
			os << jv.get_int64();
			break;

		case boost::json::kind::double_:
			os << jv.get_double();
			break;

		case boost::json::kind::bool_:
			if (jv.get_bool())
				os << "true";
			else
				os << "false";
			break;

		case boost::json::kind::null:
			os << "null";
			break;
		}

		if (indent->empty())
			os << "\n";
	}

	void CDataContainer::printToLog(std::ostream& os) const
	{
		pretty_print(os, m_tree);
	}

	void CDataContainer::printSizeToLog(std::ostream& os) const
	{
		
	}

	std::vector<std::string> CDataContainer::generatePath(const std::string & parameterName, const char pathChar)
   {
		std::vector<std::string> fullPath;
		boost::split(fullPath, parameterName, boost::is_any_of(std::string(1, pathChar)));
		return fullPath;
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
		const boost::json::value* found = findValue(parameterName, pathChar);
		if (found)
		{
			
			for (auto i = found->as_object().begin(); i != found->as_object().end(); ++i)
			{
				result.push_back(i->key_c_str());
			}
		}
		return result;
	}

	boost::json::value * CDataContainer::findValue(const std::string& parameterName, const char pathChar) const
	{		
      const auto path = generatePath(parameterName, pathChar);
		auto i = path.begin();
		boost::json::value const * found = nullptr;
		do
		{
			if (found == nullptr)
				found = m_tree.if_contains(*i);
			else
				found = found->as_object().if_contains(*i);
		} while (++i != path.end() && found != nullptr);

		return (boost::json::value * )found;
	}


	boost::json::value* CDataContainer::findValueOrCreate(const std::string& parameterName, const char pathChar)
	{
		const auto path = generatePath(parameterName, pathChar);
		auto i = path.begin();
		boost::json::value* found = nullptr;
		do
		{
			if (found == nullptr)
			{
				if (m_tree.contains(*i))
					found = m_tree.if_contains(*i);
				else
				{
					m_tree[*i].emplace_object();
					found = &(m_tree[*i]);
				}
			}
			else
			{
				if (found->as_object().contains(*i))
					found = found->as_object().if_contains(*i);
				else
					found = &(found->as_object()[*i]);
			}
		} while (++i != path.end() && found != nullptr);

		return found;
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


	boost::shared_ptr<CDataContainer> CDataContainer::copy() const
	{
		return boost::make_shared<CDataContainer>(m_tree);
	}

	void CDataContainer::mergeObjects(boost::json::object&dstObject, const boost::json::object&srcObject)
	{
		for (auto srcIt = srcObject.begin(); srcIt != srcObject.end(); ++srcIt)
		{
			const auto dstIt = dstObject.if_contains(srcIt->key());
			if (dstIt != nullptr)
			{
				if (srcIt->value().is_array())
				{
					for (boost::json::object::const_iterator arrayIt = srcIt->value().as_object().begin(); arrayIt != srcIt->value().as_object().begin(); ++arrayIt)
					{
						dstIt->as_array().emplace_back(arrayIt->value());
					}
				}
				else if (srcIt->value().is_object())
				{
					mergeObjects(dstIt->as_object(), srcIt->value().as_object());
				}
				else
				{
					//check type match, but also check some specific cases (kTrueType and kFalseType are booleans;
					//and many number combination can also match)
					//string cases : string should be copied (not just reference copy)
					if ( (srcIt->value().kind() == dstIt->kind() && !srcIt->value().is_string()) ||
						(srcIt->value().is_bool() && dstIt->is_bool()) ||
						(srcIt->value().is_number() && dstIt->is_number()))
					{
						*dstIt = srcIt->value();
					}
					else
					{
						//types don't match, try to make concordance
						switch (dstIt->kind())
						{
						
						case boost::json::kind::bool_:
							*dstIt = (convertToBool(srcIt->value()));
							break;
						case boost::json::kind::string:
							*dstIt = (convertToString(srcIt->value()));
							break;
						case boost::json::kind::int64:
						case boost::json::kind::uint64:
							*dstIt = srcIt->value();
						   break;
						default:
							throw exception::CInvalidParameter("Value is not a valid type");
						}
					}
				}
			}
			else
			{
				dstObject[srcIt->key()] = srcIt->value();
			}
		}
	}

	void CDataContainer::mergeFrom(const CDataContainer& source)
	{
		mergeObjects(m_tree, source.m_tree);
	}

	void CDataContainer::mergeFrom(boost::shared_ptr<CDataContainer>& from)
	{
		mergeObjects(m_tree, from.get()->m_tree);
	}

	void CDataContainer::setNull(const std::string& parameterName, const char pathChar)
   {
		auto* l = findValue(parameterName, pathChar);
		if(l != nullptr)
		   l->emplace_null();

		throw exception::CInvalidParameter(parameterName + " : is not found");
	}

	bool CDataContainer::isNull(const std::string& parameterName, const char pathChar) const
	{
	   const auto v = this->findValue(parameterName, pathChar);
		if (v)
			return v->is_null();
		throw exception::CInvalidParameter(parameterName + " : is not found");
	}

	bool CDataContainer::createArray(const std::string& parameterName, const char pathChar)
	{
		auto* ptr = findValueOrCreate(parameterName, pathChar);
		if (ptr)
			ptr->emplace_array();
		return true;
	}

	bool CDataContainer::isArray(const std::string& parameterName, char pathChar)
	{
		auto* ptr = findValue(parameterName, pathChar);
		if (ptr && ptr->is_array())
			return true;
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

	std::string CDataContainer::convertToString(const boost::json::value& v)
	{
		if (v.is_string())
			return v.get_string().c_str();

		if (v.is_bool())
			return v.get_bool() ? "true" : "false";

		if (v.is_double())
			return boost::lexical_cast<std::string>(v.get_double());
		if (v.is_int64())
			return boost::lexical_cast<std::string>(v.get_int64());
		if (v.is_uint64())
			return boost::lexical_cast<std::string>(v.get_uint64());
		if (v.is_number())
			return boost::lexical_cast<std::string>(v.get_int64());

		throw exception::CInvalidParameter("Value is not a valid type");
	}

	bool CDataContainer::convertToBool(const boost::json::value & v)
	{
		if (v.is_string()) {
			const std::string s = v.get_string().c_str();
			return boost::iequals(s, "true") || boost::iequals(s, "1");
		}
		if (v.is_bool())
			return v.get_bool();
		if (v.is_double())
			return v.get_double() != 0.0f;
		if (v.is_int64())
			return v.get_int64() != 0;
		if (v.is_uint64())
			return v.get_uint64() != 0;
		if (v.is_number())
		{
			try
			{
				return v.to_number<int>() != 0;
			}
			catch (std::exception& ec)
			{
				throw exception::COutOfRange((boost::format("data is not assignable to bool (%1%)") % ec.what()).str());
			}
		}
		throw exception::CInvalidParameter("Value is not a valid type");
	}


	int CDataContainer::convertToInt(const boost::json::value& v)
	{
		if (v.is_number())
		{
			try
			{
				return v.to_number<int>();
			}
			catch (std::exception& ec)
			{
				throw exception::COutOfRange((boost::format("data is not assignable to int (%1%)") % ec.what()).str());
			}
		}

		if (v.is_int64())
		{
			const int64_t b = v.get_int64();
			if (b< INT32_MIN || b>INT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to int32") % b).str());
			return static_cast<int>(b);
		}

		if (v.is_uint64())
		{
			const uint64_t b = v.get_uint64();
			if (b>INT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to int32") % b).str());
			return static_cast<int>(b);
		}

		if (v.is_double())
		{
			const double b = v.get_double();
			if ( b < static_cast<double>(INT32_MIN) || b>INT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to int32") % b).str());
			return static_cast<int>(b);
		}


		if (v.is_string()) {
			const std::string s = v.get_string().c_str();
			return boost::lexical_cast<int>(s);
		}

		if (v.is_bool())
			return v.get_bool() ? 1 : 0;

		throw exception::CInvalidParameter("Value is not a valid type");
	}

	int64_t CDataContainer::convertToInt64(const boost::json::value & v)
	{
		if (v.is_int64())
		{
			return v.get_int64();
		}

		if (v.is_uint64())
		{
			const uint64_t b = v.get_uint64();
			if (b > INT64_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to int64") % b).str());
			return static_cast<int64_t>(b);
		}

		if (v.is_number())
		{
			try
			{
				return v.to_number<int64_t>();
			}
			catch (std::exception& ec)
			{
				throw exception::COutOfRange((boost::format("data is not assignable to int64_t (%1%)") % ec.what()).str());
			}
		}

		if (v.is_double())
		{
			const double b = v.get_double();
			if (b< INT64_MIN || b>INT64_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to int64") % b).str());
			if (b > nextafter(INT64_MAX, 0))
				throw exception::COutOfRange((boost::format("%1% is not assignable to int64 : overflow") % b).str());
			return static_cast<int64_t>(b);
		}

		if (v.is_string()) {
			const std::string s = v.get_string().c_str();
			return boost::lexical_cast<int64_t>(s);
		}
		if (v.is_bool())
			return v.get_bool() ? 1 : 0;

		throw exception::CInvalidParameter("Value is not a valid type");
	}

	char CDataContainer::convertToByte(const boost::json::value& v)
	{
		if (v.is_double())
		{
			const double b = v.get_double();
			if (b< (double)INT8_MIN || b>INT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to char/int8") % b).str());
			return static_cast<char>(b);
		}

		if (v.is_number())
		{
			try
			{
				return v.to_number<char>();
			}
			catch (std::exception& ec)
			{
				throw exception::COutOfRange((boost::format("data is not assignable to char (%1%)") % ec.what()).str());
			}

		}
		

		if (v.is_int64())
		{
			const int64_t b = v.get_int64();
			if (b< INT8_MIN || b>INT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to char/int8") % b).str());
			return static_cast<char>(b);
		}

		if (v.is_uint64())
		{
			const uint64_t b = v.get_uint64();
			if (b>INT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to char/int8") % b).str());
			return static_cast<char>(b);
		}

		if (v.is_string()) {
			std::string s = v.get_string().c_str();
			return s[0];
		}

		if (v.is_bool())
			return v.get_bool() ? 1 : 0;

		throw exception::CInvalidParameter("Value is not a valid type");
	}

	short CDataContainer::convertToShort(const boost::json::value& v)
	{
		if (v.is_number())
		{
			try
			{
				return v.to_number<short>();
			}
			catch (std::exception& ec)
			{
				throw exception::COutOfRange((boost::format("data is not assignable to short (%1%)") % ec.what()).str());
			}
		}

		if (v.is_int64())
		{
			const int64_t b = v.get_int64();
			if (b< INT16_MIN || b>INT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to signed Short/Int16") % b).str());
			return static_cast<short>(b);
		}
		if (v.is_uint64())
		{
			const uint64_t b = v.get_uint64();
			if (b>INT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to signed Short/Int16") % b).str());
			return static_cast<short>(b);
		}
		if (v.is_double())
		{
			const double b = v.get_double();
			if (b< (double)INT16_MIN || b>INT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to signed Short/Int16") % b).str());
			return static_cast<short>(b);
		}

		if (v.is_string())
		{
			const std::string s = v.get_string().c_str();
			return boost::lexical_cast<short>(s);
		}

		if (v.is_bool())
			return v.get_bool() ? 1 : 0;

		throw exception::CInvalidParameter("Value is not a valid type");
	}

	unsigned int CDataContainer::convertToUInt(const boost::json::value& v)
	{
		if (v.is_uint64())
		{
			const uint64_t b = v.get_uint64();
			if (b>UINT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int/UInt32") % b).str());
			return static_cast<unsigned int>(b);
		}

		if (v.is_int64())
		{
			const int64_t b = v.get_int64();
			if (b< 0 || b>UINT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int/UInt32") % b).str());
			return static_cast<unsigned int>(b);
		}

		if (v.is_number())
		{
			try
			{
				return v.to_number<unsigned int>();
			}
			catch (std::exception& ec)
			{
				throw exception::COutOfRange((boost::format("data is not assignable to unsigned int (%1%)") % ec.what()).str());
			}
		}

		if (v.is_double())
		{
			const double b = v.get_double();
			if (b< 0 || b>UINT32_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int/UInt32") % b).str());
			return static_cast<unsigned int>(b);
		}

		if (v.is_string()) {
			const std::string s = v.get_string().c_str();
			return boost::lexical_cast<unsigned int>(s);
		}

		if (v.is_bool())
			return v.get_bool() ? 1 : 0;

		throw exception::CInvalidParameter("Value is not a valid type");
	}
	uint64_t CDataContainer::convertToUInt64(const boost::json::value& v)
	{
		if (v.is_uint64())
			return v.get_uint64();		

		if (v.is_int64())
		{
			const int64_t b = v.get_int64();
			if (b< 0 || b>INT64_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int 64/UInt64") % b).str());
			return static_cast<uint64_t>(b);
		}

		if (v.is_number())
		{
			try
			{
				return v.to_number<uint64_t>();
			}
			catch (std::exception& ec)
			{
				throw exception::COutOfRange((boost::format("data is not assignable to unsigned uint64_t (%1%)") % ec.what()).str());
			}
		}

		if (v.is_double())
		{
			const double b = v.get_double();
			if (b< 0 || b>UINT64_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int 64/UInt64") % b).str());
			if (b > nextafter(UINT64_MAX, 0))
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned int 64/UInt64 : overflow") % b).str());
			return static_cast<uint64_t>(b);
		}


		if (v.is_string()) 
		{
			const std::string s = v.get_string().c_str();
			return boost::lexical_cast<uint64_t>(s);
		}
			
		if (v.is_bool())
			return v.get_bool() ? 1 : 0;

		throw exception::CInvalidParameter("Value is not a valid type");
	}

	unsigned char CDataContainer::convertToUByte(const boost::json::value& v)
	{
		if (v.is_number())
		{
			try
			{
				return v.to_number<unsigned char>();
			}
			catch (std::exception& ec)
			{
				throw exception::COutOfRange((boost::format("data is not assignable to unsigned char (%1%)") % ec.what()).str());
			}
		}
		if (v.is_int64())
		{
			const int64_t b = v.get_int64();
			if (b< 0 || b>UINT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned char/uint8") % b).str());
			return static_cast<unsigned char>(b);
		}
	

		if (v.is_uint64())
		{
			const uint64_t b = v.get_uint64();
			if (b>UINT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned char/uint8") % b).str());
			return static_cast<unsigned char>(b);
		}

		if (v.is_double())
		{
			const double b = v.get_double();
			if (b< 0 || b>UINT8_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned char/uint8") % b).str());
			return static_cast<unsigned char>(b);
		}
		
		if (v.is_string()) {
			const std::string s = v.get_string().c_str();
			return boost::lexical_cast<unsigned char>(s);
		}
		if (v.is_bool())
			return v.get_bool() ? 1 : 0;

		throw exception::CInvalidParameter("Value is not a valid type");
	}


	unsigned short CDataContainer::convertToUShort(const boost::json::value& v)
	{
		if (v.is_uint64())
		{
			const uint64_t b = v.get_uint64();
			if (b>UINT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned short/uint16") % b).str());
			return static_cast<unsigned short>(b);
		}

		if (v.is_number())
		{
			try
			{
				return v.to_number<unsigned short>();
			}
			catch (std::exception& ec)
			{
				throw exception::COutOfRange((boost::format("data is not assignable to unsigned short (%1%)") % ec.what()).str());
			}

		}
		if (v.is_int64())
		{
			const int64_t b = v.get_int64();
			if (b< 0 || b>UINT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned short/uint16") % b).str());
			return static_cast<unsigned short>(b);
		}

		if (v.is_double())
		{
			const double b = v.get_double();
			if (b< 0 || b>UINT16_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to unsigned short/uint16") % b).str());
			return static_cast<unsigned short>(b);
		}

		if (v.is_string()) {
			const std::string s = v.get_string().c_str();
			return boost::lexical_cast<unsigned short>(s);
		}

		if (v.is_bool())
			return v.get_bool() ? 1 : 0;

		throw exception::CInvalidParameter("Value is not a valid type");
	}

	float CDataContainer::convertToFloat(const boost::json::value& v)
	{
		if (v.is_double())
		{
			const double b = v.get_double();
			if (b< -FLT_MAX || b>FLT_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to float/single") % b).str());
			return static_cast<float>(b);
		}

		if (v.is_number())
		{
			try
			{
				return v.to_number<float>();
			}
			catch (std::exception& ec)
			{
				throw exception::COutOfRange((boost::format("data is not assignable to float (%1%)") % ec.what()).str());
			}
		}

		if (v.is_int64())
		{
			const int64_t b = v.get_int64();
			if (b< -FLT_MAX || b>FLT_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to float/single") % b).str());
			return static_cast<float>(b);
		}

		if (v.is_uint64())
		{
			const uint64_t b = v.get_uint64();
			if (b< -FLT_MAX || b>FLT_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to float/single") % b).str());
			return static_cast<float>(b);
		}
					   
		if (v.is_string()) {
			const std::string s = v.get_string().c_str();
			return boost::lexical_cast<float>(s);
		}

		if (v.is_bool())
			return v.get_bool() ? 1.0f : 0.0f;
			
		throw exception::CInvalidParameter("Value is not a valid type");
	}

	double CDataContainer::convertToDouble(const boost::json::value& v)
	{
		if (v.is_double())
			return v.get_double();

		if (v.is_number())
		{
			return v.to_number<double>();
		}

		if (v.is_int64())
		{
			const int64_t b = v.get_int64();
			if (b< -DBL_MAX || b>DBL_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to double") % b).str());
			return static_cast<double>(b);
		}

		if (v.is_uint64())
		{
			const uint64_t b = v.get_uint64();
			if (b< -DBL_MAX || b>DBL_MAX)
				throw exception::COutOfRange((boost::format("%1% is not assignable to double") % b).str());
			return static_cast<double>(b);
		}

		if (v.is_string()) {
			const std::string s = v.get_string().c_str();
			return boost::lexical_cast<double>(s);
		}
		if (v.is_bool())
			return v.get_bool() ? 1.0f : 0.0f;

		throw exception::CInvalidParameter("Value is not a valid type");
	}
} // namespace shared
