#pragma once
#include "boost/function.hpp"

//
/// \brief Static class that provide converters from const char * to data type 
//
class CConverters
{
public:
   template<class TValue>
   static inline boost::function1<TValue, const char *> getConverter()
   {
      return &lexicalCast;
   }

   template<class TValue> 
   static inline TValue lexicalCast(const char * v)
   {
      return boost::lexical_cast<TValue>(v);
   }

  
   template<>
   static inline boost::function1<double, const char*> getConverter<double>()
   {
      return &convertDouble;
   }

   template<>
   static inline boost::function1<float, const char*> getConverter<float>()
   {
      return &convertFloat;
   }

   template<>
   static inline boost::function1<int, const char*> getConverter<int>()
   {
      return &convertInt;
   }

   template<>
   static inline boost::function1<long, const char*> getConverter<long>()
   {
      return &convertLong;
   }
   
   template<>
   static inline boost::function1<std::string, const char*> getConverter<std::string>()
   {
      return &doNotConvert;
   }

private:

   static inline double convertDouble(const char * v)
   {
      return atof(v);
   }

   static inline float convertFloat(const char * v)
   {
      return (float)atof(v);
   }

   static inline int convertInt(const char * v)
   {
      return atoi(v);
   }

   static inline long convertLong(const char * v)
   {
      return atol(v);
   }

   static inline std::string doNotConvert(const char * v)
   {
      return v;
   }

private:
   CConverters() {}
   virtual ~CConverters() {}
};
