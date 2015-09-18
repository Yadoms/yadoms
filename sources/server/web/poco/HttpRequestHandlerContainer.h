#pragma once
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>


namespace web { namespace poco {
      
   /*
    * Cette classse permet d'eviter de recreer les gestionnaires de requete à chaque demande Web
    * Elle hérite de HTTPRequestHandler et contient un SharedPtr de HTTPRequestHandler.
    * Ainsi on va recreer à chaque appel un container mais pas l'objet complet
    */
   class CHttpRequestHandlerContainer : public Poco::Net::HTTPRequestHandler
   {
   public:
      /**
       * Ctor
       * @param m_requestHandler : gestionnaire de requete HTTP à encapsuler
       */
      CHttpRequestHandlerContainer(boost::shared_ptr<Poco::Net::HTTPRequestHandler> & m_requestHandler);

      /**
       * Dtor
       */
      virtual ~CHttpRequestHandlerContainer();

      /**
       * Permet de demander le traitement de la requete
       * @param request : requete à traiter
       * @param response : réponse à renvoyer
       */
      virtual void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

   private:
      boost::shared_ptr<Poco::Net::HTTPRequestHandler> m_requestHandler;
   };
   
} //namespace poco
} //namespace web


