/**
 * Created by jmberhault on 05/01/2018.
 */


/**
 * Constructor which does nothing because it is used as a static class
 * @constructor
 */
function SessionDataManager(){}

/**
 * Add a variable
 * @param {string} name of the variable
 * @param {string} value of the variable
 */
SessionDataManager.addVariable = function(name, value) {
    assert(!isNullOrUndefined(name), "name must be defined");
    assert(!isNullOrUndefined(value), "value must be defined");

	//if !(typeof value === 'string' || value instanceof String) {
	//}
	
    // Save data to sessionStorage
    sessionStorage.setItem(name, value);
};

/**
 * Create all tasks
 * @param {string} name of the variable
 */
SessionDataManager.getVariable = function(name) {
	assert(!isNullOrUndefined(name), "name must be defined");

	// Get saved data from sessionStorage
    return sessionStorage.getItem(name);

}

/**
 * Remove a variable stored into the session
 * @param {string} name of the value already stored
 */
SessionDataManager.removeVariable = function(name) {
   assert(!isNullOrUndefined(name), "name must be defined");

   // Remove saved data from sessionStorage
   sessionStorage.removeItem(name);
}

/**
 * Clean up all the cache
 */
SessionDataManager.cleanup = function() {
   // Remove all saved data from sessionStorage
   sessionStorage.clear();
}