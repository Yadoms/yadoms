#pragma once

namespace authentication
{
   //-------------------------------------
   ///\brief Interface for authentication
   //-------------------------------------
   class IAuthentication
   {
   public:
      //-------------------------------------
      ///\brief Destructor
      //-------------------------------------
      virtual ~IAuthentication()
      {
      }

      //-------------------------------------
      ///\brief Indicates if authentication is active
      ///\return    true if the user is authentication is active
      //-------------------------------------   
      virtual bool isAuthenticationActive() const = 0;

      //-------------------------------------
      ///\brief Try to authenticate a user/password combination
      ///\param [in]    username       The user name
      ///\param [in]    password       The password
      ///\return    true if the user is successfully authenticated
      //-------------------------------------   
      virtual bool authenticate(const std::string& username,
                                const std::string& password) const = 0;
   };
} //namespace authentication


