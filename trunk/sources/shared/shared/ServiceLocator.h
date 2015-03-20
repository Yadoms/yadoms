#pragma once

#include <shared/Export.h>

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
   class YADOMS_SHARED_EXPORT CServiceLocator
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
      /// \brief		   Get the first object which implements TInterface
      /// \template     TInterface   The interface/class to search for implementation
      /// \return       The found object, or null shared_ptr if not found
      //--------------------------------------------------------------
      template<class TInterface>
      inline boost::shared_ptr<TInterface> get();
      
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
   inline boost::shared_ptr<TInterface> CServiceLocator::get()
   {
      ServiceContainer::iterator i = m_content.find(typeid(TInterface).hash_code());
      if (i != m_content.end())
      {
         if (i->second.size()>0)
            return boost::any_cast< boost::shared_ptr<TInterface> >(i->second[0]);
      }
      return boost::shared_ptr<TInterface>();
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

