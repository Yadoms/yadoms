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
 * Event used when aa acquisition update message is received from server
 * @type {function({data: (String|Blob|ArrayBuffer|Object)})}
 */
WebSocketEngine.onAcquisitionUpdated = null;

/**
 * Event used when a task update is received from the server
 * @type {function({data: (String|Blob|ArrayBuffer|Object)})}
 */
WebSocketEngine.onTaskUpdated = null;

/**
 * Update the acquisition filter
 * @param keywordCollectionToFilter The collection of keyword id to filter
 */
WebSocketEngine.updateAcquisitionFilter = function(keywordCollectionToFilter) {
    WebSocketEngine.webSocket_.send(JSON.stringify({"type" : "acquisitionFilter", "data" : keywordCollectionToFilter}));
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
        WebSocketEngine.webSocket_ = new WebSocket('ws://' + window.location.host + '/ws');

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
                            if($.isFunction(WebSocketEngine.onAcquisitionUpdated)) {
                                WebSocketEngine.onAcquisitionUpdated(websocketData);
                            }
                            break;
                        case "devicenew":
                            break;
                        case "taskupdatenotification":
                            if($.isFunction(WebSocketEngine.onTaskUpdated)) {
                                WebSocketEngine.onTaskUpdated(websocketData);
                            }
                            console.log("TaskUpdateNotification : " + JSON.stringify(websocketData));
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
