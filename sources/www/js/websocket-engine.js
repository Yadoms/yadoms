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
              
        WebSocketEngine.webSocket_ = new WebSocket(websocketEndpoint);

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
