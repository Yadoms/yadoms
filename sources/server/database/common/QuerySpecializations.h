//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//==== This file must not be included (only Query.h)
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================
//============================================================================

//==== This contains all template specializations methods for CQuery class

//--------------------------------------------------------------
/// \brief	    Helper structure for converting values
//--------------------------------------------------------------
template <typename T, class Enable = void>
struct queryhelper
{
   static std::string format(CQuery* obj, const T& anyValue)
   {
      //dont use queryhelper <std::string> because Clang issue it
      return obj->formatStringToSql(boost::lexical_cast<std::string>(anyValue));
   }
};

//--------------------------------------------------------------
/// \brief	    Helper structure for converting std::string to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<std::string>
{
   static std::string format(CQuery* obj, const std::string& anyValue)
   {
      return obj->formatStringToSql(anyValue);
   }
};


//--------------------------------------------------------------
/// \brief	    Helper structure for converting Poco::Int8 (or any int) to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<Poco::Int8>
{
   static std::string format(CQuery* obj, const Poco::Int8& anyValue)
   {
      return obj->formatInt8ToSql(anyValue);
   }
};

//--------------------------------------------------------------
/// \brief	    Helper structure for converting Poco::UInt8 (or any unsigned int) to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<Poco::UInt8>
{
   static std::string format(CQuery* obj, const Poco::UInt8& anyValue)
   {
      return obj->formatUInt8ToSql(anyValue);
   }
};


//--------------------------------------------------------------
/// \brief	    Helper structure for converting Poco::Int16 (or any int) to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<Poco::Int16>
{
   static std::string format(CQuery* obj, const Poco::Int16& anyValue)
   {
      return obj->formatInt16ToSql(anyValue);
   }
};

//--------------------------------------------------------------
/// \brief	    Helper structure for converting Poco::UInt16 (or any unsigned int) to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<Poco::UInt16>
{
   static std::string format(CQuery* obj, const Poco::UInt16& anyValue)
   {
      return obj->formatUInt16ToSql(anyValue);
   }
};


//--------------------------------------------------------------
/// \brief	    Helper structure for converting Poco::Int32 (or any int) to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<Poco::Int32>
{
   static std::string format(CQuery* obj, const Poco::Int32& anyValue)
   {
      return obj->formatInt32ToSql(anyValue);
   }
};

//--------------------------------------------------------------
/// \brief	    Helper structure for converting Poco::UInt32 (or any unsigned int) to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<Poco::UInt32>
{
   static std::string format(CQuery* obj, const Poco::UInt32& anyValue)
   {
      return obj->formatUInt32ToSql(anyValue);
   }
};


//--------------------------------------------------------------
/// \brief	    Helper structure for converting Poco::Int64 (or any int) to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<Poco::Int64>
{
   static std::string format(CQuery* obj, const Poco::Int64& anyValue)
   {
      return obj->formatInt64ToSql(anyValue);
   }
};

//--------------------------------------------------------------
/// \brief	    Helper structure for converting Poco::UInt64 (or any unsigned int) to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<Poco::UInt64>
{
   static std::string format(CQuery* obj, const Poco::UInt64& anyValue)
   {
      return obj->formatUInt64ToSql(anyValue);
   }
};

//--------------------------------------------------------------
/// \brief	    Helper structure for converting Boost DateTime to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<boost::posix_time::ptime>
{
   static std::string format(CQuery* obj, const boost::posix_time::ptime& anyValue)
   {
      return obj->formatDateToSql(anyValue);
   }
};

//--------------------------------------------------------------
/// \brief	    Helper structure for converting Poco DateTime to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<Poco::DateTime>
{
   static std::string format(CQuery* obj, const Poco::DateTime& anyValue)
   {
      return obj->formatDateToSql(anyValue);
   }
};

//--------------------------------------------------------------
/// \brief	    Helper structure for converting Poco Timestamp to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<Poco::Timestamp>
{
   static std::string format(CQuery* obj, const Poco::Timestamp& anyValue)
   {
      return obj->formatDateToSql(anyValue);
   }
};

//--------------------------------------------------------------
/// \brief	    Helper structure for converting vector to sql string
//--------------------------------------------------------------
template <typename T>
struct queryhelper<std::vector<T>>
{
   static std::string format(CQuery* obj, const std::vector<T>& values)
   {
      std::ostringstream ss;
      ss << "( ";
      auto first = true;
      for (const auto& value : values)
      {
         if (!first)
            ss << ", ";
         first = false;
         ss << queryhelper<T>::format(obj, value);
      }
      ss << ") ";
      return ss.str();
   }
};

//--------------------------------------------------------------
/// \brief	    Helper structure for converting set to sql string
//--------------------------------------------------------------
template <typename T>
struct queryhelper<std::set<T>>
{
   static std::string format(CQuery* obj, const std::set<T>& values)
   {
      std::ostringstream ss;
      ss << "( ";
      auto first = true;
      for (const auto& value : values)
      {
         if (!first)
            ss << ", ";
         first = false;
         ss << queryhelper<T>::format(obj, value);
      }
      ss << ") ";
      return ss.str();
   }
};

//--------------------------------------------------------------
/// \brief	    Helper structure for converting CQuery to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<CQuery>
{
   static std::string format(CQuery* obj, const CQuery& anyValue)
   {
      return obj->formatSubQueryToSql(anyValue);
   }
};

//-------------------------------------------------------------- 
/// \brief	    Helper structure for converting boost::shared_ptr<T> to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<shared::CDataContainer>
{
   static std::string format(CQuery* obj, const shared::CDataContainer& anyValue)
   {
      return anyValue.serialize();
   }
};

//-------------------------------------------------------------- 
/// \brief	    Helper structure for converting boost::shared_ptr<T> to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<shared::CDataContainerSharedPtr>
{
   static std::string format(CQuery* obj, const shared::CDataContainerSharedPtr& anyValue)
   {
      return anyValue->serialize();
   }
};



//--------------------------------------------------------------
/// \brief	    Helper structure for converting IExtendedEnum to sql string
//--------------------------------------------------------------
template <typename T>
struct queryhelper<T, typename boost::enable_if<boost::is_base_of<shared::enumeration::IExtendedEnum, T>>::type>
{
   static std::string format(CQuery* obj, const T& anyValue)
   {
      return obj->formatEnumToSql(anyValue);
   }
};


//-------------------------------------------------------------- 
/// \brief	    Helper structure for converting boost::shared_ptr<T> to sql string
//--------------------------------------------------------------
template <typename T>
struct queryhelper<boost::shared_ptr<T>>
{
   static std::string format(CQuery* obj, const T& anyValue)
   {
      return queryhelper<T>::format(obj, *anyValue.get());
   }
};

//--------------------------------------------------------------
/// \brief	    Helper structure for converting with CField<T> to sql string
//--------------------------------------------------------------
template <typename T>
struct queryhelper<shared::CField<T>>
{
   static std::string format(CQuery* obj, const T& anyValue)
   {
      return queryhelper<T>::format(obj, anyValue());
   }
};


//--------------------------------------------------------------
/// \brief	    Helper structure for converting with CField<T> to sql string
//--------------------------------------------------------------
template <>
struct queryhelper<CQuery::CFunction>
{
   static std::string format(CQuery* obj, const CQuery::CFunction& anyValue)
   {
      return anyValue.toSql();
   }
};


//--------------------------------------------------------------
/// \brief	    Helper structure for converting column to its name in queries
//--------------------------------------------------------------
template <>
struct queryhelper<CDatabaseColumn>
{
   static std::string format(CQuery* obj, const CDatabaseColumn& anyValue)
   {
      return anyValue.GetName();
   }
};


//--------------------------------------------------------------
/// \brief	    Helper structure for converting column to its name in queries
//--------------------------------------------------------------
template <>
struct queryhelper<CDatabaseTable>
{
   static std::string format(CQuery* obj, const CDatabaseTable& tableInfo)
   {
      return tableInfo.GetName();
   }
};


//--------------------------------------------------------------
/// \brief	    Helper structure for get/set with an IExtendedEnum object
//--------------------------------------------------------------
template <>
struct queryhelper<CQuery::CNotUsedTemplateField>
{
   static std::string format(CQuery* obj, const CQuery::CNotUsedTemplateField& anyValue)
   {
      throw CDatabaseException("CNotUsedTemplateField must not be used");
   }
};


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
inline CQuery& CQuery::Select(const T1& field1, const T2& field2, const T3& field3, const T4& field4, const T5& field5, const T6& field6, const T7& field7, const T8& field8, const T9& field9, const T10& field10)
{
   ChangeQueryType(kSelect);
   std::ostringstream ss;
   ss << "SELECT " << queryhelper<T1>::format(this, field1);
   if (typeid(field2) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T2>::format(this, field2));
   if (typeid(field3) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T3>::format(this, field3));
   if (typeid(field4) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T4>::format(this, field4));
   if (typeid(field5) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T5>::format(this, field5));
   if (typeid(field6) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T6>::format(this, field6));
   if (typeid(field7) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T7>::format(this, field7));
   if (typeid(field8) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T8>::format(this, field8));
   if (typeid(field9) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T9>::format(this, field9));
   if (typeid(field10) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T10>::format(this, field10));
   ss << " ";
   return Append(ss);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
inline CQuery& CQuery::SelectCount(const T1& field1, const T2& field2, const T3& field3, const T4& field4, const T5& field5, const T6& field6, const T7& field7, const T8& field8, const T9& field9, const T10& field10)
{
   ChangeQueryType(kSelect);
   std::ostringstream ss;
   ss << "SELECT COUNT(" << queryhelper<T1>::format(this, field1);
   if (typeid(field2) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T2>::format(this, field2));
   if (typeid(field3) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T3>::format(this, field3));
   if (typeid(field4) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T4>::format(this, field4));
   if (typeid(field5) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T5>::format(this, field5));
   if (typeid(field6) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T6>::format(this, field6));
   if (typeid(field7) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T7>::format(this, field7));
   if (typeid(field8) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T8>::format(this, field8));
   if (typeid(field9) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T9>::format(this, field9));
   if (typeid(field10) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T10>::format(this, field10));
   ss << ") ";
   return Append(ss);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
inline CQuery& CQuery::From(const T1& table1, const T2& table2, const T3& table3, const T4& table4, const T5& table5, const T6& table6, const T7& table7, const T8& table8, const T9& table9, const T10& table10)
{
   std::ostringstream ss;
   ss << " FROM " << queryhelper<T1>::format(this, table1);

   if (typeid(table2) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T2>::format(this, table2));

   if (typeid(table3) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T3>::format(this, table3));

   if (typeid(table4) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T4>::format(this, table4));

   if (typeid(table5) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T5>::format(this, table5));

   if (typeid(table6) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T6>::format(this, table6));

   if (typeid(table7) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T7>::format(this, table7));

   if (typeid(table8) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T8>::format(this, table8));

   if (typeid(table9) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T9>::format(this, table9));

   if (typeid(table10) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T10>::format(this, table10));
   ss << " ";
   return Append(ss);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
inline CQuery& CQuery::Values(const T1& value1, const T2& value2, const T3& value3, const T4& value4, const T5& value5, const T6& value6, const T7& value7, const T8& value8, const T9& value9, const T10& value10)
{
   std::ostringstream ss;
   ss << " VALUES  (" << queryhelper<T1>::format(this, value1);
   if (typeid(value2) != typeid(CNotUsedTemplateField))
      AppendValue(ss, queryhelper<T2>::format(this, value2));
   if (typeid(value3) != typeid(CNotUsedTemplateField))
      AppendValue(ss, queryhelper<T3>::format(this, value3));
   if (typeid(value4) != typeid(CNotUsedTemplateField))
      AppendValue(ss, queryhelper<T4>::format(this, value4));
   if (typeid(value5) != typeid(CNotUsedTemplateField))
      AppendValue(ss, queryhelper<T5>::format(this, value5));
   if (typeid(value6) != typeid(CNotUsedTemplateField))
      AppendValue(ss, queryhelper<T6>::format(this, value6));
   if (typeid(value7) != typeid(CNotUsedTemplateField))
      AppendValue(ss, queryhelper<T7>::format(this, value7));
   if (typeid(value8) != typeid(CNotUsedTemplateField))
      AppendValue(ss, queryhelper<T8>::format(this, value8));
   if (typeid(value9) != typeid(CNotUsedTemplateField))
      AppendValue(ss, queryhelper<T9>::format(this, value9));
   if (typeid(value10) != typeid(CNotUsedTemplateField))
      AppendValue(ss, queryhelper<T10>::format(this, value10));
   ss << ") ";
   return Append(ss);
}

template <class T01, class T1, class T02, class T2, class T03, class T3, class T04, class T4, class T05, class T5,
          class T06, class T6, class T07, class T7, class T08, class T8, class T09, class T9, class T11, class T10>
inline CQuery& CQuery::Set(const T01& field1, const T1& value1,
                           const T02& field2, const T2& value2,
                           const T03& field3, const T3& value3,
                           const T04& field4, const T4& value4,
                           const T05& field5, const T5& value5,
                           const T06& field6, const T6& value6,
                           const T07& field7, const T7& value7,
                           const T08& field8, const T8& value8,
                           const T09& field9, const T9& value9,
                           const T11& field10, const T10& value10)
{
   std::ostringstream ss;
   ss << " SET " << queryhelper<T01>::format(this, field1) << "=" << queryhelper<T1>::format(this, value1);
   if (typeid(value2) != typeid(CNotUsedTemplateField))
      AppendSet(ss, queryhelper<T02>::format(this, field2), queryhelper<T2>::format(this, value2));
   if (typeid(value3) != typeid(CNotUsedTemplateField))
      AppendSet(ss, queryhelper<T03>::format(this, field3), queryhelper<T3>::format(this, value3));
   if (typeid(value4) != typeid(CNotUsedTemplateField))
      AppendSet(ss, queryhelper<T04>::format(this, field4), queryhelper<T4>::format(this, value4));
   if (typeid(value5) != typeid(CNotUsedTemplateField))
      AppendSet(ss, queryhelper<T05>::format(this, field5), queryhelper<T5>::format(this, value5));
   if (typeid(value6) != typeid(CNotUsedTemplateField))
      AppendSet(ss, queryhelper<T06>::format(this, field6), queryhelper<T6>::format(this, value6));
   if (typeid(value7) != typeid(CNotUsedTemplateField))
      AppendSet(ss, queryhelper<T07>::format(this, field7), queryhelper<T7>::format(this, value7));
   if (typeid(value8) != typeid(CNotUsedTemplateField))
      AppendSet(ss, queryhelper<T08>::format(this, field8), queryhelper<T8>::format(this, value8));
   if (typeid(value9) != typeid(CNotUsedTemplateField))
      AppendSet(ss, queryhelper<T09>::format(this, field9), queryhelper<T9>::format(this, value9));
   if (typeid(value10) != typeid(CNotUsedTemplateField))
      AppendSet(ss, queryhelper<T11>::format(this, field10), queryhelper<T10>::format(this, value10));
   ss << " ";
   return Append(ss);
}


template <class T1, class T2>
CQuery& CQuery::Where(const T1& field, const std::string& op, const T2& value)
{
   return PredicateInternal("WHERE ", queryhelper<T1>::format(this, field), op, queryhelper<T2>::format(this, value));
}

template <class T1, class T2>
CQuery& CQuery::WhereParenthesis(const T1& field, const std::string& op, const T2& value)
{
   return PredicateInternal("WHERE (", queryhelper<T1>::format(this, field), op, queryhelper<T2>::format(this, value));
}

template <class T1, class T2>
CQuery& CQuery::And(const T1& field, const std::string& op, const T2& value)
{
   return PredicateInternal("AND ", queryhelper<T1>::format(this, field), op, queryhelper<T2>::format(this, value));
}

template <class T1, class T2>
CQuery& CQuery::AndParenthesis(const T1& field, const std::string& op, const T2& value)
{
   return PredicateInternal("AND (", queryhelper<T1>::format(this, field), op, queryhelper<T2>::format(this, value));
}

template <class T1, class T2>
CQuery& CQuery::Or(const T1& field, const std::string& op, const T2& value)
{
   return PredicateInternal("OR ", queryhelper<T1>::format(this, field), op, queryhelper<T2>::format(this, value));
}

template <class T1, class T2>
CQuery& CQuery::OrParenthesis(const T1& field, const std::string& op, const T2& value)
{
   return PredicateInternal("OR (", queryhelper<T1>::format(this, field), op, queryhelper<T2>::format(this, value));
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
inline CQuery& CQuery::OrderBy(const T1& value1, const CQuery::E_OrderWay way1,
                               const T2& value2, const CQuery::E_OrderWay way2,
                               const T3& value3, const CQuery::E_OrderWay way3,
                               const T4& value4, const CQuery::E_OrderWay way4,
                               const T5& value5, const CQuery::E_OrderWay way5,
                               const T6& value6, const CQuery::E_OrderWay way6,
                               const T7& value7, const CQuery::E_OrderWay way7,
                               const T8& value8, const CQuery::E_OrderWay way8,
                               const T9& value9, const CQuery::E_OrderWay way9,
                               const T10& value10, const CQuery::E_OrderWay way10)
{
   std::ostringstream ss;
   ss << " ORDER BY " << queryhelper<T1>::format(this, value1);

   if (way1 == CQuery::kDesc)
      ss << " DESC";

   if (typeid(value2) != typeid(CNotUsedTemplateField))
      AppendOrderField(ss, queryhelper<T2>::format(this, value2), way2);
   if (typeid(value3) != typeid(CNotUsedTemplateField))
      AppendOrderField(ss, queryhelper<T3>::format(this, value3), way3);
   if (typeid(value4) != typeid(CNotUsedTemplateField))
      AppendOrderField(ss, queryhelper<T4>::format(this, value4), way4);
   if (typeid(value5) != typeid(CNotUsedTemplateField))
      AppendOrderField(ss, queryhelper<T5>::format(this, value5), way5);
   if (typeid(value6) != typeid(CNotUsedTemplateField))
      AppendOrderField(ss, queryhelper<T6>::format(this, value6), way6);
   if (typeid(value7) != typeid(CNotUsedTemplateField))
      AppendOrderField(ss, queryhelper<T7>::format(this, value7), way7);
   if (typeid(value8) != typeid(CNotUsedTemplateField))
      AppendOrderField(ss, queryhelper<T8>::format(this, value8), way8);
   if (typeid(value9) != typeid(CNotUsedTemplateField))
      AppendOrderField(ss, queryhelper<T9>::format(this, value9), way9);
   if (typeid(value10) != typeid(CNotUsedTemplateField))
      AppendOrderField(ss, queryhelper<T10>::format(this, value10), way10);
   return Append(ss);
}


template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
inline CQuery& CQuery::GroupBy(const T1& field1,
                               const T2& field2,
                               const T3& field3,
                               const T4& field4,
                               const T5& field5,
                               const T6& field6,
                               const T7& field7,
                               const T8& field8,
                               const T9& field9,
                               const T10& field10)
{
   std::ostringstream ss;
   ss << " GROUP BY " << queryhelper<T1>::format(this, field1);
   if (typeid(field2) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T2>::format(this, field2));
   if (typeid(field3) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T3>::format(this, field3));
   if (typeid(field4) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T4>::format(this, field4));
   if (typeid(field5) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T5>::format(this, field5));
   if (typeid(field6) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T6>::format(this, field6));
   if (typeid(field7) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T7>::format(this, field7));
   if (typeid(field8) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T8>::format(this, field8));
   if (typeid(field9) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T9>::format(this, field9));
   if (typeid(field10) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T10>::format(this, field10));
   return Append(ss);
}

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
inline CQuery& CQuery::InsertInto(const database::common::CDatabaseTable& table,
                                  const T1& field1,
                                  const T2& field2,
                                  const T3& field3,
                                  const T4& field4,
                                  const T5& field5,
                                  const T6& field6,
                                  const T7& field7,
                                  const T8& field8,
                                  const T9& field9,
                                  const T10& field10)
{
   ChangeQueryType(kInsert);
   std::ostringstream ss;
   ss << " INSERT INTO " << table.GetName() << " (" << queryhelper<T1>::format(this, field1);
   if (typeid(field2) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T2>::format(this, field2));
   if (typeid(field3) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T3>::format(this, field3));
   if (typeid(field4) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T4>::format(this, field4));
   if (typeid(field5) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T5>::format(this, field5));
   if (typeid(field6) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T6>::format(this, field6));
   if (typeid(field7) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T7>::format(this, field7));
   if (typeid(field8) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T8>::format(this, field8));
   if (typeid(field9) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T9>::format(this, field9));
   if (typeid(field10) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T10>::format(this, field10));
   ss << ") ";
   return Append(ss);
}

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
inline CQuery& CQuery::InsertOrReplaceInto(const database::common::CDatabaseTable& table,
                                           const T1& field1,
                                           const T2& field2,
                                           const T3& field3,
                                           const T4& field4,
                                           const T5& field5,
                                           const T6& field6,
                                           const T7& field7,
                                           const T8& field8,
                                           const T9& field9,
                                           const T10& field10)
{
   ChangeQueryType(kInsert);
   std::ostringstream ss;
   ss << " " << m_insertOrUpdateName << " " << table.GetName() << " (" << queryhelper<T1>::format(this, field1);
   if (typeid(field2) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T2>::format(this, field2));
   if (typeid(field3) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T3>::format(this, field3));
   if (typeid(field4) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T4>::format(this, field4));
   if (typeid(field5) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T5>::format(this, field5));
   if (typeid(field6) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T6>::format(this, field6));
   if (typeid(field7) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T7>::format(this, field7));
   if (typeid(field8) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T8>::format(this, field8));
   if (typeid(field9) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T9>::format(this, field9));
   if (typeid(field10) != typeid(CNotUsedTemplateField))
      AppendField(ss, queryhelper<T10>::format(this, field10));
   ss << ") ";
   return Append(ss);
}


template <class T>
inline const CQuery::CFunction CQuery::as(const T& fieldOrQuery, const std::string& columnName)
{
   return CFunction(functionAs(queryhelper<T>::format(this, fieldOrQuery), columnName));
}

template <class T>
inline const CQuery::CFunction CQuery::distinct(const T& fieldOrQuery)
{
   return CFunction(functionDistinct(queryhelper<T>::format(this, fieldOrQuery)));
}

template <class T>
inline const CQuery::CFunction CQuery::count(const T& fieldOrQuery)
{
   if (typeid(fieldOrQuery) != typeid(CNotUsedTemplateField))
      return CFunction(functionCount(queryhelper<T>::format(this, fieldOrQuery)));
   return CFunction(functionCount("*"));
}

template <class T>
inline const CQuery::CFunction CQuery::sum(const T& fieldOrQuery)
{
   return CFunction(functionSum(queryhelper<T>::format(this, fieldOrQuery)));
}


template <class T1, class T2>
inline const CQuery::CFunction CQuery::math(const T1& value1, const std::string& op, const T2& value2)
{
   return CFunction(queryhelper<T1>::format(this, value1) + " " + op + " " + queryhelper<T2>::format(this, value2));
}

template <class T>
inline const CQuery::CFunction CQuery::substr(const T& value, int offset, int count)
{
   return CFunction(this->functionSubstring(queryhelper<T>::format(this, value), offset, count));
}

//--------------------------------------------------------------
///\brief	generate min function ( ie: min(field0) )
///\param [in]	field    The field or query
///\return The query function
//--------------------------------------------------------------
template <class T>
inline const CQuery::CFunction CQuery::min(const T& value)
{
   return CFunction(this->functionMin(queryhelper<T>::format(this, value)));
}

//--------------------------------------------------------------
///\brief	generate min function ( ie: min(field0) ) with numeric cast
///\param [in]	field    The field or query
///\return The query function
//--------------------------------------------------------------
template <class T>
inline const CQuery::CFunction CQuery::minWithCast(const T& fieldOrQuery)
{
   return min(castNumeric(fieldOrQuery));
}


//--------------------------------------------------------------
///\brief	generate max function ( ie: max(field0) )
///\param [in]	field    The field or query
///\return The query function
//--------------------------------------------------------------
template <class T>
inline const CQuery::CFunction CQuery::max(const T& value)
{
   return CFunction(this->functionMax(queryhelper<T>::format(this, value)));
}


//--------------------------------------------------------------
///\brief	generate max function ( ie: max(field0) ) with numeric cast
///\param [in]	field    The field or query
///\return The query function
//--------------------------------------------------------------
template <class T>
inline const CQuery::CFunction CQuery::maxWithCast(const T& fieldOrQuery)
{
   return max(castNumeric(fieldOrQuery));
}


//--------------------------------------------------------------
///\brief	generate average function ( ie: average(field0) )
///\param [in]	field    The field or query
///\return The query function
//--------------------------------------------------------------
template <class T>
inline const CQuery::CFunction CQuery::average(const T& value)
{
   return CFunction(this->functionAvg(queryhelper<T>::format(this, value)));
}


//--------------------------------------------------------------
///\brief	generate average function ( ie: average(field0) ) with numeric cast
///\param [in]	field    The field or query
///\return The query function
//--------------------------------------------------------------
template <class T>
inline const CQuery::CFunction CQuery::averageWithCast(const T& fieldOrQuery)
{
   return average(castNumeric(fieldOrQuery));
}


//--------------------------------------------------------------
///\brief	generate coalesce function ( ie: coalesce(field0, default) )
///\param [in]	field       The field or query
///\param [in]	valueIfNull The fallback value
///\return The query function
//--------------------------------------------------------------
template <class T, class T2>
inline const CQuery::CFunction CQuery::coalesce(const T& value, const T2& valueIfNull)
{
   return CFunction(this->functionCoalesce(queryhelper<T>::format(this, value), queryhelper<T2>::format(this, valueIfNull)));
}


//--------------------------------------------------------------
///\brief	generate cast function ( ie: CAST (field0 AS numeric) )
///\param [in]	field       The field or query
///\param [in]	type        The typing cast
///\return The query function
//--------------------------------------------------------------
template <class T>
inline const CQuery::CFunction CQuery::cast(const T& fieldOrQuery, const std::string& type)
{
   return CFunction(this->functionCast(queryhelper<T>::format(this, fieldOrQuery)), type);
}

//--------------------------------------------------------------
///\brief	generate cast function ( ie: CAST (field0 AS numeric) )
///\param [in]	field       The field or query
///\return The query function
//--------------------------------------------------------------
template <class T>
inline const CQuery::CFunction CQuery::castNumeric(const T& fieldOrQuery)
{
   return CFunction(this->functionCastNumeric(queryhelper<T>::format(this, fieldOrQuery)));
}

template <class T>
inline const CQuery::CFunction CQuery::dateToIsoString(const T& fieldOrQuery)
{
   return CFunction(this->functionDateToIsoString(queryhelper<T>::format(this, fieldOrQuery)));
}

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
inline const CQuery::CFunction CQuery::concatenate(const T1& field1, const T2& field2, const T3& field3, const T4& field4, const T5& field5, const T6& field6, const T7& field7, const T8& field8, const T9& field9, const T10& field10)
{
   std::string current = functionConcatenate(queryhelper<T1>::format(this, field1), queryhelper<T2>::format(this, field2));
   if (typeid(field3) != typeid(CNotUsedTemplateField))
      current = functionConcatenate(current, queryhelper<T3>::format(this, field3));
   if (typeid(field4) != typeid(CNotUsedTemplateField))
      current = functionConcatenate(current, queryhelper<T4>::format(this, field4));
   if (typeid(field5) != typeid(CNotUsedTemplateField))
      current = functionConcatenate(current, queryhelper<T5>::format(this, field5));
   if (typeid(field6) != typeid(CNotUsedTemplateField))
      current = functionConcatenate(current, queryhelper<T6>::format(this, field6));
   if (typeid(field7) != typeid(CNotUsedTemplateField))
      current = functionConcatenate(current, queryhelper<T7>::format(this, field7));
   if (typeid(field8) != typeid(CNotUsedTemplateField))
      current = functionConcatenate(current, queryhelper<T8>::format(this, field8));
   if (typeid(field9) != typeid(CNotUsedTemplateField))
      current = functionConcatenate(current, queryhelper<T9>::format(this, field9));
   if (typeid(field10) != typeid(CNotUsedTemplateField))
      current = functionConcatenate(current, queryhelper<T10>::format(this, field10));
   return CFunction(current);
}
