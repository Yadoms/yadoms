#pragma once

#include "exception/NullReference.hpp"

namespace shared { 

   //--------------------------------------------------------------
   /// \brief		This class implement the Locator pattern
   ///            The aim is to provide a container which coulmd contain and share any object
   ///
   ///
   ///   Usage
   ///   ==========
   ///      
   ///      class A : public B
   ///      {
   ///      };
   ///      
   ///      boost::shared<A> obj(new A);
   ///      
   ///      
   ///      //register object as implementation ob interface B
   ///      shared::CServiceLocator::instance().push<B>(obj);
   ///      
   ///      //get one object
   ///      boost::shared<B> b = shared::CServiceLocator::instance().get<B>();
   ///      
   ///      //get all
   ///      std::vector< boost::shared<B> > all = shared::CServiceLocator::instance().getAll<B>();
   //--------------------------------------------------------------
   class CServiceLocator
   {
   public:
      //--------------------------------------------------------------
      /// \brief		Constructor
      //--------------------------------------------------------------
      CServiceLocator();
      
      //--------------------------------------------------------------
      /// \brief		Destructor
      //--------------------------------------------------------------
      virtual ~CServiceLocator();
      
      //--------------------------------------------------------------
      /// \brief		Get a static instance
      /// \return    A reference to the static instance of ServiceLocator
      //--------------------------------------------------------------
      static CServiceLocator& instance();

      //--------------------------------------------------------------
      /// \brief		   Push a new object in locator
      /// \param [in] 	anyObj	Any boost::shared_ptr<T> where T implements TInterface
      /// \template     TInterface   The interface/class which object implements
      /// \template     TInstance    This template should not be specified, it should be infered by compiler
      //--------------------------------------------------------------
      template<class TInterface, class TInstance>
      inline void push(boost::shared_ptr<TInstance> anyObj);
      
      //--------------------------------------------------------------
      /// \brief		   Remove all objects matching the interface
      /// \template     TInterface   The interface/class which object implements
      //--------------------------------------------------------------
      template<class TInterface>
      inline void removeInterface();

      //--------------------------------------------------------------
      /// \brief		   Remove an object from an implementating an interface
      /// \template     TInterface   The interface/class which object implements
      /// \template     TInstance    This template should not be specified, it should be infered by compiler
      //--------------------------------------------------------------
      template<class TInterface, class TInstance>
      inline void remove(boost::shared_ptr<TInstance> anyObj);

      //--------------------------------------------------------------
      /// \brief		   Get the first object which implements TInterface
      /// \template     TInterface   The interface/class to search for implementation
      /// \param[in] noThrow Don't throw if object not found (return empty object)
      /// \return       The found object, or null shared_ptr if not found
      /// \throw exception::CNullReference if object not found and noThrow == false
      //--------------------------------------------------------------
      template<class TInterface>
      inline boost::shared_ptr<TInterface> get(bool noThrow = false);
      
      //--------------------------------------------------------------
      /// \brief		   Get all objects which implements TInterface
      /// \template     TInterface   The interface/class to search for implementation
      /// \return       A copy of a vector (copy is needed)
      //--------------------------------------------------------------
      template<class TInterface>
      inline std::vector< boost::shared_ptr<TInterface> > getAll();

   private:
      //--------------------------------------------------------------
      /// \brief		Define the type of locator content
      //--------------------------------------------------------------
      typedef std::map< size_t, std::vector<boost::any> > ServiceContainer;

      //--------------------------------------------------------------
      /// \brief		The locator content
      //--------------------------------------------------------------
      ServiceContainer m_content;

      //--------------------------------------------------------------
      /// \brief		The static instance
      //--------------------------------------------------------------
      static CServiceLocator m_instance;
   };
      
   template<class TInterface, class TInstance>
   inline void CServiceLocator::push(boost::shared_ptr<TInstance> anyObj)
   {
      //ensure TInstance implements TInstance
      boost::shared_ptr<TInterface> workingPointer = boost::dynamic_pointer_cast<TInterface>(anyObj);
      m_content[typeid(TInterface).hash_code()].push_back(workingPointer);
   }   

   template<class TInterface>
   inline void CServiceLocator::removeInterface()
   {
      ServiceContainer::iterator i = m_content.find(typeid(TInterface).hash_code());
      if (i != m_content.end())
         m_content.erase(i);
   }

   template<class TInterface, class TInstance>
   inline void CServiceLocator::remove(boost::shared_ptr<TInstance> anyObj)
   {
      std::vector<boost::any> & ref = m_content[typeid(TInterface).hash_code()];
      for (std::vector< boost::any >::iterator i = ref.begin(); i != ref.end(); ++i)
      {
         try
         {
            boost::shared_ptr<TInstance> bObj = boost::any_cast< boost::shared_ptr<TInstance> >(*i);
            if (bObj && bObj == anyObj)
            {
               ref.erase(i);
               break;
            }
         }
         catch (...)
         {

         }
      }
   }


   template<class TInterface>
   inline boost::shared_ptr<TInterface> CServiceLocator::get(bool noThrow)
   {
      ServiceContainer::iterator i = m_content.find(typeid(TInterface).hash_code());
      if (i == m_content.end() || i->second.size() == 0)
      {
         if (noThrow)
            return boost::shared_ptr<TInterface>();

         throw exception::CNullReference(typeid(TInterface).name());
      }

      return boost::any_cast< boost::shared_ptr<TInterface> >(i->second[0]);
   }

   template<class TInterface>
   inline std::vector< boost::shared_ptr<TInterface> > CServiceLocator::getAll()
   {
      std::vector< boost::shared_ptr<TInterface> > result;
      ServiceContainer::iterator i = m_content.find(typeid(TInterface).hash_code());
      if (i != m_content.end())
      {
         std::vector<boost::any>::iterator j;
         for(j= i->second.begin(); j!= i->second.end(); ++j)
         {
            result.push_back(boost::any_cast< boost::shared_ptr<TInterface> >(*j));
         }
      }
      return result;
   }
   
} //namespace shared

