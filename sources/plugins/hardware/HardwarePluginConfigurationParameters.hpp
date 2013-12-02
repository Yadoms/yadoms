#pragma once
#include <string>
#include <map>
#include <boost/assert.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


//--------------------------------------------------------------
/// \class Hardware plugin configuration parameter base class
//--------------------------------------------------------------
class CHardwarePluginConfigurationParameter
{
public:
   CHardwarePluginConfigurationParameter(const std::string& name)
      :m_name(name) {}

   virtual boost::shared_ptr<CHardwarePluginConfigurationParameter> clone() const = 0;

   const std::string& getName() const { return m_name; }

   virtual const std::string valueToString() const = 0;
   virtual void valueFromString(const std::string& valueAsString) = 0;

   virtual void load(boost::property_tree::ptree& pt) = 0;
   virtual void save(boost::property_tree::ptree& pt) const = 0;

private:
   std::string m_name;
};

//--------------------------------------------------------------
/// \class  Hardware plugin configuration templated parameter for simple types
/// \note   To use this class, types have to support << operator
//--------------------------------------------------------------
template<typename T>
class CHardwarePluginConfigurationParameterT : public CHardwarePluginConfigurationParameter
{
public:
   CHardwarePluginConfigurationParameterT(const std::string& name, const T value)
      :CHardwarePluginConfigurationParameter(name), m_value(value) {}

   virtual boost::shared_ptr<CHardwarePluginConfigurationParameter> clone() const
   {
      boost::shared_ptr<CHardwarePluginConfigurationParameter> p(new CHardwarePluginConfigurationParameterT<T>(getName(), m_value));
      return p;
   }

   T get() const { return m_value; }

   virtual const std::string valueToString() const
   {
      std::ostringstream os;
      os << m_value;
      return os.str();
   }

   virtual void valueFromString(const std::string& valueAsString)
   {
      std::istringstream is(valueAsString);
      is >> m_value;
   }

   virtual void load(boost::property_tree::ptree& pt)
   {
      m_value = pt.get<T>(getName());
   }
   virtual void save(boost::property_tree::ptree& pt) const
   {
      pt.put<T>(getName(), m_value);
   }

protected:
   T m_value;
};

//--------------------------------------------------------------
/// \class Hardware plugin configuration string parameter
//--------------------------------------------------------------
class CHardwarePluginConfigurationStringParameter : public CHardwarePluginConfigurationParameter
{
public:
   CHardwarePluginConfigurationStringParameter(const std::string& name, const std::string& value)
      :CHardwarePluginConfigurationParameter(name), m_value(value) {}

   virtual boost::shared_ptr<CHardwarePluginConfigurationParameter> clone() const
   {
      boost::shared_ptr<CHardwarePluginConfigurationParameter> p(new CHardwarePluginConfigurationStringParameter(getName(), m_value));
      return p;
   }

   const std::string& get() const { return m_value; }

   virtual const std::string valueToString() const
   { return m_value; }

   virtual void valueFromString(const std::string& valueAsString)
   { m_value = valueAsString; }

   virtual void load(boost::property_tree::ptree& pt)
   {
      m_value = pt.get<std::string>(getName());
   }
   virtual void save(boost::property_tree::ptree& pt) const
   {
      pt.put<std::string>(getName(), m_value);
   }

private:
   std::string m_value;
};

//--------------------------------------------------------------
/// \class Hardware plugin configuration bool parameter
//--------------------------------------------------------------
class CHardwarePluginConfigurationBoolParameter : public CHardwarePluginConfigurationParameterT<bool>
{
public:
   CHardwarePluginConfigurationBoolParameter(const std::string& name, bool value)
      :CHardwarePluginConfigurationParameterT<bool>(name, value) {}

   virtual boost::shared_ptr<CHardwarePluginConfigurationParameter> clone() const
   {
      boost::shared_ptr<CHardwarePluginConfigurationParameter> p(new CHardwarePluginConfigurationBoolParameter(getName(), m_value));
      return p;
   }

   virtual const std::string valueToString() const
   { return m_value ? "true" : "false"; }

   virtual void valueFromString(const std::string& valueAsString)
   { m_value = valueAsString == "true"; }
};

//--------------------------------------------------------------
/// \type Hardware plugin configuration int parameter
//--------------------------------------------------------------
typedef CHardwarePluginConfigurationParameterT<int> CHardwarePluginConfigurationIntParameter;

//--------------------------------------------------------------
/// \type Hardware plugin configuration double parameter
//--------------------------------------------------------------
typedef CHardwarePluginConfigurationParameterT<double> CHardwarePluginConfigurationDoubleParameter;

//--------------------------------------------------------------
/// \class Enum parameter, generic class
//--------------------------------------------------------------
class CHardwarePluginConfigurationEnumGeneric : public CHardwarePluginConfigurationParameter
{
public:
   typedef std::map<unsigned int, std::string> ValuesNames;

public:
   CHardwarePluginConfigurationEnumGeneric(const std::string& name)
      :CHardwarePluginConfigurationParameter(name){}
   virtual ~CHardwarePluginConfigurationEnumGeneric(){}
   virtual const ValuesNames& getAvailableValues() const = 0;
};

//--------------------------------------------------------------
/// \class Hardware plugin configuration enum parameter
//--------------------------------------------------------------
template<typename Enum>
class CHardwarePluginConfigurationEnumParameter : public CHardwarePluginConfigurationEnumGeneric
{
public:
   CHardwarePluginConfigurationEnumParameter(const std::string& name, Enum value, const ValuesNames& valueNames)
      :CHardwarePluginConfigurationEnumGeneric(name), m_value(value), m_valueNames(valueNames) {}

   virtual boost::shared_ptr<CHardwarePluginConfigurationParameter> clone() const
   {
      boost::shared_ptr<CHardwarePluginConfigurationParameter> p(new CHardwarePluginConfigurationEnumParameter<Enum>(getName(), m_value, m_valueNames));
      return p;
   }

   Enum get() const { return m_value; }

   virtual const std::string valueToString() const
   {
      try
      {
         return m_valueNames.at(m_value);
      }
      catch (const std::out_of_range& oor)
      {
         BOOST_ASSERT(false);  // Name of the value doesn't exist
         std::cerr << "Out of Range error: " << oor.what() << '\n';//TODO : log
         return std::string(getName() + " : Invalid value ");
      }
   }

   virtual void valueFromString(const std::string& valueAsString)
   {
      ValuesNames::const_iterator it;
      for (it = m_valueNames.begin() ; it != m_valueNames.end() ; ++it)
      {
         if ((*it).second == valueAsString)
         {
            m_value = (Enum)((*it).first);
            break;
         }
      }
      if (it == m_valueNames.end())
      {
         BOOST_ASSERT(0); // Value name not found
         throw std::out_of_range(std::string ("Enum value not found for ") + valueAsString);
      }
   }

   const ValuesNames& getAvailableValues() const
   {
      return m_valueNames;
   }

   virtual void load(boost::property_tree::ptree& pt)
   {
      m_value = (Enum)pt.get<unsigned int>(getName());
   }
   virtual void save(boost::property_tree::ptree& pt) const
   {
      pt.put<unsigned int>(getName(), m_value);
   }

private:
   Enum m_value;
   const ValuesNames& m_valueNames;
};

