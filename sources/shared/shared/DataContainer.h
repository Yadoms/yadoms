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
   ///				test.set<int>("intValue", 5);
   ///				test.set<double>("doubleValue", 4.0);
   ///				test.set<std::string>("stringValue", "plouf");
   ///			
   ///				int i = test.get<int>("intValue");
   ///				double d = test.get<double>("doubleValue");
   ///				std::string s = test.get<std::string>("stringValue");
   ///			
   ///			
   ///			2. vector of data container
   ///				std::vector<int> vi;
   ///				...
   ///				test.set< std::vector<int> >("vectorint", vi);
   ///			
   ///				std::vector<double> vd;
   ///				...
   ///				test.set< std::vector<double> >("vectordouble", vd);
   ///			
   ///			
   ///				std::vector<int> vi2 = test.getValues< std::vector<int> >("vectorint");
   ///				std::vector<double> vd2 = test.getValues< std::vector<double> >("vectordouble");
   ///			
   ///			
   ///			
   ///			3. sub container (act as a tree)
   ///            
   ///            //create another container
   ///            shared::CDataContainer subContainer;
   ///            subContainer.set<int>("int4", 6);
   ///            subContainer.set<double>("double1", 8.0);
   ///            subContainer.set<std::string>("string2", "plouf2");
   ///            
   ///            //add it to main container
   ///            test.set("config1", subContainer);
   ///            //retreive container
   ///            shared::CDataContainer subt = test.get<shared::CDataContainer>("config1");
   ///            //direct access to a sub container data
   ///            double d = test.get<double>("config1.double1");
   ///
   ///
   ///
   //--------------------------------------------------------------
   /*
   class CDataContainer;

   class YADOMS_SHARED_EXPORT IDataContainable
   {
   public:
      virtual ~IDataContainable() { }

      //--------------------------------------------------------------
      /// \brief		Extract the data into a data container 
      /// \return    The data container
      //--------------------------------------------------------------
      virtual const CDataContainer & extractContent() const = 0;

      //--------------------------------------------------------------
      /// \brief		Fill this object instance with a container
      /// \param [in] initialData    Initial data for this container (will be deserialized)
      //--------------------------------------------------------------
      virtual void fillFromContent(const CDataContainer & initialData) = 0;
   };
   */

   class YADOMS_SHARED_EXPORT CDataContainer : public serialization::IDataSerializable, public serialization::IDataFileSerializable /*, public IDataContainable*/
   {
   public:
      //--------------------------------------------------------------
      //
      //
      //
      // Constructors & destructor
      //
      //
      //
      //--------------------------------------------------------------

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
      //
      //
      //
      // Public get/set methods
      //
      //
      //
      //--------------------------------------------------------------

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
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      //--------------------------------------------------------------
      template<class T>
      inline void set(const std::string& parameterName, const T & value);


      //--------------------------------------------------------------
      //
      //
      //
      // Public get/set methods for ENUMS
      //
      //
      //
      //--------------------------------------------------------------

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
      //
      //
      //
      // Public initialization methods
      //
      //
      //
      //--------------------------------------------------------------


      //--------------------------------------------------------------
      /// \brief		Copy another container to this instance
      /// \param [in] rhs  The container to copy
      //--------------------------------------------------------------
      void initializeWith(const CDataContainer &rhs);

      //--------------------------------------------------------------
      //
      //
      //
      // Public utility methods
      //
      //
      //
      //--------------------------------------------------------------

      //--------------------------------------------------------------
      /// \brief	    Check if parameter value is present
      /// \param [in] parameterName    Name of the parameter
      /// \return     true if parameter found
      //--------------------------------------------------------------
      bool hasValue(const std::string& parameterName) const;


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
      /// \brief		Affectation operator
      /// \param [in] rhs  A serialized string
      /// \return   Reference to this instance
      //--------------------------------------------------------------
      CDataContainer & operator=(const std::string &rhs);

      //--------------------------------------------------------------
      /// \brief		Check if the container is empty
      /// \return    true if contanier is empty
      //--------------------------------------------------------------
      bool empty() const;


      //--------------------------------------------------------------
      //
      //
      //
      // Interface implementations
      //
      //
      //
      //--------------------------------------------------------------

      // IDataSerializable implementation
      virtual std::string serialize() const;
      virtual void deserialize(const std::string & data);
      // [END] IDataSerializable implementation

      // IDataFileSerializable implementation
      virtual void serializeToFile(const std::string & filename) const;
      virtual void deserializeFromFile(const std::string & filename);
      // [END] IDataFileSerializable implementation

      // IDataContainable implementation
      //virtual const CDataContainer & extractContent() const;
      //virtual void fillFromContent(const CDataContainer & initialData);
      // [END] IDataContainable implementation


      //--------------------------------------------------------------
      //
      //
      //
      // Static helpers
      //
      //
      //
      //--------------------------------------------------------------


      //--------------------------------------------------------------
      /// \brief		An empty container which could be used as default method parameter
      //--------------------------------------------------------------
      const static CDataContainer EmptyContainer;


   protected:

      //--------------------------------------------------------------
      //
      //
      //
      // Internal get/set methods
      //
      //
      //
      //--------------------------------------------------------------

      //--------------------------------------------------------------
      /// \brief	    Get parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      inline T getInternal(const std::string& parameterName) const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter value. If the name is not found the default value is returned
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] defaultValue     Default value
      /// \return     The parameter value
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      inline T getInternal(const std::string& parameterName, const T & defaultValue) const;

      //--------------------------------------------------------------
      /// \brief	    Get parameter values
      /// \param [in] parameterName    Name of the parameter
      /// \return     The parameter values
      /// \throw      shared::exception::COutOfRange if parameter can not be converted
      /// \throw      shared::exception::CInvalidParameter if parameter is not found
      //--------------------------------------------------------------
      template<class T>
      inline std::vector<T> getValuesInternal(const std::string& parameterName) const;


      //--------------------------------------------------------------
      /// \brief	    Set parameter value
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] value            Value of the parameter
      //--------------------------------------------------------------
      template<class T>
      inline void setInternal(const std::string& parameterName, const T & value);

      //--------------------------------------------------------------
      /// \brief	    Set parameter values
      /// \param [in] parameterName    Name of the parameter
      /// \param [in] values           Valuse of the parameter
      //--------------------------------------------------------------
      template<class T>
      inline void setValuesInternal(const std::string& parameterName, const std::vector<T> & values);


      //--------------------------------------------------------------
      //
      //
      //
      // Internal helpers (allow redirecting to the good get/set internal method)
      //
      //
      //
      //--------------------------------------------------------------

      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with single value type (int, double, class,...)
      //--------------------------------------------------------------
      template <typename T> struct helper {
         static T getInternal(const CDataContainer * tree, const std::string& parameterName) { return tree->getInternal<T>(parameterName); }
         static T getInternal(const CDataContainer * tree, const std::string& parameterName, const T & defaultValue)
         {
            if (tree->hasValue(parameterName))
               return tree->getInternal<T>(defaultValue);
            return defaultValue;
         }
         
         static void setInternal(CDataContainer * tree, const std::string& parameterName, const T & value) 
         { 
            tree->setInternal<T>(parameterName, value); 
         }
         
      };

      //--------------------------------------------------------------
      /// \brief	    Helper structure for get/set with vector of value type (vector<int>, vector<double>, vector<class>,...)
      //--------------------------------------------------------------
      template <typename T> struct helper < std::vector<T> > {
         static std::vector<T> getInternal(const CDataContainer * tree, const std::string& parameterName) { return tree->getValuesInternal<T>(parameterName); }
         static std::vector<T> getInternal(const CDataContainer * tree, const std::string& parameterName, const T & defaultValue)
         {
            if (tree->hasValue(parameterName))
               return tree->getValuesInternal<T>(parameterName);
            return defaultValue;
         }

         static void setInternal(CDataContainer * tree, const std::string& parameterName, const std::vector<T> & value)
         { 
            tree->setValuesInternal(parameterName, value);
         }
      };

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
   template<typename T>
   inline T CDataContainer::get(const std::string& parameterName) const
   {
      return helper<T>::getInternal(this, parameterName);
   }

   template<typename T>
   inline T CDataContainer::get(const std::string& parameterName, const T & defaultValue) const
   {
      return helper<T>::getInternal(this, parameterName, defaultValue);
   }

   template<typename T>
   inline void CDataContainer::set(const std::string& parameterName, const T & value)
   {
      helper<T>::setInternal(this, parameterName, value);
   }



   template<class T>
   inline T CDataContainer::getInternal(const std::string& parameterName) const
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

   template<>
   inline CDataContainer CDataContainer::getInternal(const std::string& parameterName) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         CDataContainer t;
         t.m_tree = m_tree.get_child(parameterName);;
         return t;
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
   /*
   template<>
   inline IDataContainable CDataContainer::getInternal(const std::string& parameterName) const
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         CDataContainer dc;
         dc.m_tree = m_tree.get_child(parameterName);
         return IDataContainable::createFromContainer(dc);
      }
      catch (boost::property_tree::ptree_bad_path& e)
      {
         throw exception::CInvalidParameter(parameterName + " : " + e.what());
      }
      catch (boost::property_tree::ptree_bad_data& e)
      {
         throw exception::COutOfRange(parameterName + " can not be converted to expected type, " + e.what());
      }
   }*/




   template<class T>
   inline std::vector<T> CDataContainer::getValuesInternal(const std::string& parameterName) const
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
   inline void CDataContainer::setInternal(const std::string& parameterName, const T & value)
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

   template<>
   inline void CDataContainer::setInternal(const std::string& parameterName, const CDataContainer & value)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         m_tree.add_child(parameterName, value.m_tree);
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
   inline void CDataContainer::setValuesInternal(const std::string& parameterName, const std::vector<T> & values)
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
            innerData.push_back(std::make_pair("", t));
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
   inline void CDataContainer::setValuesInternal(const std::string& parameterName, const std::vector<CDataContainer> & values)
   {
      boost::lock_guard<boost::mutex> lock(m_treeMutex);

      try
      {
         boost::property_tree::ptree innerData;

         std::vector<CDataContainer>::const_iterator i;
         for (i = values.begin(); i != values.end(); ++i)
         {
            innerData.push_back(std::make_pair("", i->m_tree));
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
