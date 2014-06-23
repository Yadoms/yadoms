#pragma once

#include <shared/Export.h>
#include <shared/exception/OutOfRange.hpp>
#include <shared/exception/InvalidParameter.hpp>
#include "serialization/IDataSerializable.h"
#include "serialization/IDataFileSerializable.h"

namespace shared
{
   //--------------------------------------------------------------
   /// \brief	This class should be used as a data container
   ///			The aim is to handle any data; like configuration
	///			
	///			The container should contains data by a get/set method
	///			Each data is associated to a key name
	///			The data could be a simple type, a list of data, or a DataContainer child
	///
	///			The container has the serialize feature
	///			This is done be serializing/deserializing a string,
	///			or a file
	///			
	///			USAGE
	///			
	///			1. simple data container
	///				shared::CDataContainer test;
	///				test.set("intValue", 5);
	///				test.set("doubleValue", 4.0);
	///				test.set("stringValue", "plouf");
	///			
	///				int i = test.get<int>("intValue");
	///				double d = test.get<double>("doubleValue");
	///				std::string s = test.get<std::string>("stringValue");
	///			
	///			
	///			2. vector of data container
	///				std::vector<int> vi;
	///				...
	///				test.setValues("vectorint", vi);
	///			
	///				std::vector<double> vd;
	///				...
	///				test.setValues("vectordouble", vd);
	///			
	///			
	///				std::vector<int> vi2 = test.getValues<int>("vectorint");
	///				std::vector<double> vd2 = test.getValues<double>("vectordouble");
   ///			
   ///			
   ///			
   ///			3. sub container (act as a tree)
   ///            
   ///            //create another container
   ///            shared::CDataContainer subContainer;
   ///            subContainer.set("int4", 6);
   ///            subContainer.set("double1", 8.0);
   ///            subContainer.set("string2", "plouf2");
   ///            
   ///            //add it to main container
   ///            test.setChild("config1", subContainer);
   ///            //retreive container
   ///            shared::CDataContainer getTree = test.getChild("config1");
   ///            //direct access to a sub container data
   ///            double d = test.get<double>("config1.double1");
   ///
   ///
   ///
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CDataContainer : public serialization::IDataSerializable, public serialization::IDataFileSerializable
   {
   public:
      //--------------------------------------------------------------
      /// \brief		Constructor. Empty data
      //--------------------------------------------------------------
      CDataContainer();

      //--------------------------------------------------------------
      /// \brief		Constructor. 
      /// \param [in] initialData    Initial data for this container (will be deserialized)
      //--------------------------------------------------------------
      CDataContainer(const std::string & initialData);

      //--------------------------------------------------------------
      /// \brief		Constructor. 
      /// \param [in] initialData    Initial data for this container (will be deserialized)
      //--------------------------------------------------------------
      CDataContainer(const CDataContainer & initialData);

      //--------------------------------------------------------------
      /// \brief			Destructor
      /// \return    	void
      //--------------------------------------------------------------
      virtual ~CDataContainer(void);

      //--------------------------------------------------------------
      /// \brief	    Get parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      inline T get(const std::string& parameterName) const;

	  //--------------------------------------------------------------
	  /// \brief	    Get parameter value
	  /// \param [in] parameterName    Name of the parameter
	  /// \return     The parameter value
	  /// \throw      shared::exception::COutOfRange if parameter can not be converted
	  /// \throw      shared::exception::CInvalidParameter if parameter is not found
	  //--------------------------------------------------------------
	  CDataContainer getChild(const std::string& parameterName) const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter value. If the name is not found the default value is returned
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] defaultValue     Default value
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      inline T get(const std::string& parameterName, const T & defaultValue) const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter values
      /// \param [in] parameterName    Name of the parameter
      /// \return     The parameter values
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      inline std::vector<T> getValues(const std::string& parameterName) const;

      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      //--------------------------------------------------------------
      template<class T>
      inline void set(const std::string& parameterName, const T & value);
	  
	  //--------------------------------------------------------------
      /// \brief	    Append a child
      /// \param [in] parameterName    Child name
      /// \param [in] value            Child container
      //--------------------------------------------------------------
      void setChild(const std::string& parameterName, const CDataContainer & value);

      //--------------------------------------------------------------
      /// \brief	    Set parameter values
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] values           Valuse of the parameter
      //--------------------------------------------------------------
      template<class T>
      inline void setValues(const std::string& parameterName, const std::vector<T> & values);

      //--------------------------------------------------------------
      /// \brief	    Check if parameter value is present
      /// \param [in] parameterName    Name of the parameter
      /// \return     true if parameter found
      //--------------------------------------------------------------
      bool hasValue(const std::string& parameterName) const;
      
      //--------------------------------------------------------------
      /// \brief	    Type representing authorized enum values list
      //--------------------------------------------------------------
      typedef std::map<std::string, unsigned int> EnumValuesNames;

      //--------------------------------------------------------------
      /// \brief	    Get enum parameter value
      /// \param [in] parameterName    Name of the enum parameter
      /// \param[in]  valueNames  List of authorized values for the enum
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter value is not in the provided list
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<typename EnumType>
	  EnumType getEnumValue(const std::string& parameterName, const EnumValuesNames& valuesNames) const;

      //--------------------------------------------------------------
      /// \brief		Equality operator
      /// \param [in] rhs  The container to compare with
      /// \return   true if the container equals to this instance
      //--------------------------------------------------------------
      bool operator ==(const CDataContainer &rhs) const;

      //--------------------------------------------------------------
      /// \brief		Difference operator
      /// \param [in] rhs  The container to compare with
      /// \return   true if the container is different to this instance
      //--------------------------------------------------------------
      bool operator !=(const CDataContainer &rhs) const;

      //--------------------------------------------------------------
      /// \brief		Affectation operator
      /// \param [in] rhs  The container to copy
      /// \return   Reference to this instance
      //--------------------------------------------------------------
      CDataContainer & operator=(const CDataContainer &rhs);

      //--------------------------------------------------------------
      /// \brief		Copy another container to this instance
      /// \param [in] rhs  The container to copy
      //--------------------------------------------------------------
      void set(const CDataContainer &rhs);

      //--------------------------------------------------------------
      /// \brief		Check if the container is empty
      /// \return    true if contanier is empty
      //--------------------------------------------------------------
      bool empty() const;



	  // IDataSerializable implementation
	  virtual std::string serialize() const;
	  virtual void deserialize(const std::string & data);
	  // [END] IDataSerializable implementation

	  // IDataFileSerializable implementation
	  virtual void serializeToFile(const std::string & filename) const;
	  virtual void deserializeFromFile(const std::string & filename);
	  // [END] IDataFileSerializable implementation


	  //--------------------------------------------------------------
	  /// \brief		An empty container which could be used as default method parameter
	  //--------------------------------------------------------------
	  const static CDataContainer EmptyContainer;

   private:
      //--------------------------------------------------------------
      /// \brief	   The configuration content
      //--------------------------------------------------------------
      boost::property_tree::ptree m_tree;

      //--------------------------------------------------------------
      /// \brief	   Mutex protecting the configuration content
      //--------------------------------------------------------------
      mutable boost::mutex m_treeMutex;
   };





   //--------------------------------------------------------------
   /// \brief	    The below part of header contains all the function
   ///				definitions (out of class definition for better 
   ///				cross compiler compatibilities).
   ///				-> for template methods
   ///				-> for template specialization
   //--------------------------------------------------------------



   template<class T>
   inline T CDataContainer::get(const std::string& parameterName) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         return m_tree.get<T>(parameterName);
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }


   template<class T>
   inline T CDataContainer::get(const std::string& parameterName, const T & defaultValue) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         if (hasValue(parameterName))
            return m_tree.get<T>(parameterName);
         return defaultValue;
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }




   template<class T>
   inline std::vector<T> CDataContainer::getValues(const std::string& parameterName) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      std::vector<T> result;
      try
      {
         boost::property_tree::ptree child = m_tree.get_child(parameterName);

         boost::property_tree::ptree::const_iterator end = child.end();
         for (boost::property_tree::ptree::const_iterator it = child.begin(); it != end; ++it) {
            result.push_back(it->second.get_value<T>());
         }
         return result;
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }


   template<class T>
   inline void CDataContainer::set(const std::string& parameterName, const T & value)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         m_tree.put(parameterName, value);
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }

   template<class T>
   inline void CDataContainer::setValues(const std::string& parameterName, const std::vector<T> & values)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         boost::property_tree::ptree innerData;

         typename std::vector<T>::const_iterator i;
         for (i = values.begin(); i != values.end(); ++i)
         {
            boost::property_tree::ptree t;
            t.put("", *i);
            innerData.push_back( std::make_pair("", t ) );
         }

         m_tree.add_child(parameterName, innerData);
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }


   template<>
   inline void CDataContainer::setValues(const std::string& parameterName, const std::vector<CDataContainer> & values)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         boost::property_tree::ptree innerData;

         std::vector<CDataContainer>::const_iterator i;
         for (i = values.begin(); i != values.end(); ++i)
         {
            innerData.push_back( std::make_pair("", i->m_tree ) );
         }

         m_tree.add_child(parameterName, innerData);
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }

   template<typename EnumType>
   EnumType CDataContainer::getEnumValue(const std::string& parameterName, const EnumValuesNames& valuesNames) const
   {
	   std::string stringValue = get<std::string>(parameterName);
	   EnumValuesNames::const_iterator it = valuesNames.find(stringValue);
	   if (it != valuesNames.end())
		   return (EnumType)(it->second);

	   throw exception::COutOfRange(std::string("Value ") + stringValue + " was not found for " + parameterName + " parameter");
   }

} // namespace shared
