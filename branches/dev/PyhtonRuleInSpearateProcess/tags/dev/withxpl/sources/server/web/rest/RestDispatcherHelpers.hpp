#pragma once


#define ADD_URL_PATTERN(r, data, idx, elem) \
   temp.push_back( elem );


#define DECLARE_URL_PATTERN(_seq)\
CRestDispatcher::CUrlPattern temp; \
   BOOST_PP_SEQ_FOR_EACH_I(ADD_URL_PATTERN, _var, _seq)

#define REGISTER_HANDLER(dispatcher, reqType, pattern, method) \
   dispatcher.registerRestMethodHandler(reqType,  temp, boost::bind(&method, this, _1, _2));

#define REGISTER_HANDLER_WITH_INDIRECTOR(dispatcher, reqType, pattern, method, indirectorMethod) \
   dispatcher.registerRestMethodHandler(reqType,  temp, boost::bind(&method, this, _1, _2), boost::bind(&indirectorMethod, this, _1, _2, _3));

#define REGISTER_DISPATCHER_HANDLER(dispatcher, reqType, _seq, method) \
   { \
      DECLARE_URL_PATTERN(_seq); \
      REGISTER_HANDLER(dispatcher, reqType, _ , method)\
   }


#define REGISTER_DISPATCHER_HANDLER_WITH_INDIRECTOR(dispatcher, reqType, _seq, method, indirectorMethod) \
   { \
      DECLARE_URL_PATTERN(_seq); \
      REGISTER_HANDLER_WITH_INDIRECTOR(dispatcher, reqType, _ , method, indirectorMethod)\
   }


