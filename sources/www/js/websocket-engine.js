/**
 * Created by jmdecoret on 01/04/2015.
 */

/**
 * Ctor which does nothing because it is used as a static class
 * @constructor
 */
function WebSocketEngine(){}

/**
 * The current socket
 * @type {Object}
 * @private
 */
WebSocketEngine.webSocket_ = null;

/**
 * Check if websocket is active
 * @return {boolean}
 */
WebSocketEngine.isActive = function() {
	return !isNullOrUndefined(this.webSocket_);
};

/**
 * Check if websocket is active
 * @return {boolean}

 Constant	Value	Description
CONNECTING	0	The connection is not yet open.
OPEN	1	The connection is open and ready to communicate.
CLOSING	2	The connection is in the process of closing.
CLOSED	3	The connection is closed or couldn't be opened.

*/
WebSocketEngine.isConnected = function() {
	if (!isNullOrUndefined(this.webSocket_)){
	   if (this.webSocket_.readyState != 3)
         return true;
	   else
		   return false;
	} else 
      return false;
};

/**
 * Update the acquisition filter
 * @param keywordCollectionToFilter The collection of keyword id to filter
 */
WebSocketEngine.updateAcquisitionFilter = function(keywordCollectionToFilter) {
   
   WebSocketEngine.webSocket_.send(JSON.stringify({"type" : "acquisitionFilter", "data" : keywordCollectionToFilter}));
};


/**
 * Make a ping of the server
 * @param keywordCollectionToFilter The collection of keyword id to filter
 */
WebSocketEngine.sendIsServerAlive = function () {
    WebSocketEngine.webSocket_.send(JSON.stringify({ "type": "isAlive"}));
};

/**
 * Initialize the websocket engine
 * @param {function} callback The callback for result
 */
WebSocketEngine.initializeWebSocketEngine = function(callback) {

     /*
      * Definitions of functions called in the document by websocket returns
      */

     //we listen acquisitionupdate event
     $(document).on("acquisitionupdate", function(e, websocketData) {
         var acq = AcquisitionManager.factory(websocketData.data);
         dispatchNewAcquisitionsToWidgets(acq);
     });
     //we listen time event
     $(document).on("timenotification", function(e, websocketData) {
         dispatchTimeToWidgets(websocketData.time);
     });
     //we listen keyword Deletion event
     $(document).on("keyworddeleted", function(e, websocketData) {
         dispatchkeywordDeletedToWidgets(websocketData.data);
         dispatchkeywordDeletedToDevicesView(websocketData.data);
     });
     /*
      */     
     
    if (!isNullOrUndefined(WebSocketEngine.webSocket_))
        WebSocketEngine.webSocket_.close();

    //we check if websocket are handled
    if(window.MozWebSocket) {
        window.WebSocket = window.MozWebSocket;
    }
    if(window.WebSocket) {
        //the protocol of the web socket depends on the http or https protocol
         var websocketEndpoint = '';
         var protocol = "";
         if(!isNullOrUndefinedOrEmpty(Yadoms.baseUrl)) {
            websocketEndpoint = concatenateUrl(Yadoms.baseUrl.replace("http", "ws"), '/ws');
         } else {
            if (window.location.protocol.indexOf("https") !== -1)
               protocol = "wss://";
            else
               protocol = "ws://";
            websocketEndpoint = protocol + window.location.host + '/ws';
         }
        
        try{        
           WebSocketEngine.webSocket_ = new WebSocket(websocketEndpoint);
        }catch(error){
           console.warn (error);
        }
        
        WebSocketEngine.webSocket_.onopen = function() {
            console.debug('Web socket opened');
            if ($.isFunction(callback))
                callback();
        };

        WebSocketEngine.webSocket_.onmessage = function(e) {
            if (!isNullOrUndefined(e)) {
                var websocketData = JSON.parse(e.data);
                if (!isNullOrUndefined(websocketData)) {
                    switch (websocketData.type.toLowerCase()) {
                        case "acquisitionupdate":
                           $(document).trigger("acquisitionupdate", websocketData);
                           break;
                        case "keyworddeleted":
                           $(document).trigger("keyworddeleted", websocketData);
                           break;
                        case "devicenew":
                           break;
                        case "taskupdatenotification":
                           $(document).trigger("taskupdatenotification." + websocketData.uuid, websocketData);
                           break;
                        case "isalive":
                           $(document).trigger("isalive");
                           break;
                        case "timenotification":
                           $(document).trigger("timenotification", websocketData);
                           break;
                    }
                }
            }
        };

        WebSocketEngine.webSocket_.onclose = function() {
            console.debug('Web socket closed');
        };
    }
    else {
        console.debug("Web socket unhandled");
    }
};