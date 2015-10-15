/**
 * Class PromiseCounter : This promise wait for nb_index release before call the callback
 */

/**
 * Ctor
 * @param nb_index : number of promise to wait
 * @param theCallback :
 * @param param
 * @constructor
 */
PromiseCounter = function(nb_index, theCallback, param) {
   this.counter = nb_index;
   this.param = param;
   this.callback = theCallback;
};

/**
 * Increment by value (or one if ignored) the promise counter
 */
PromiseCounter.prototype.incrementCounter = function(value) {
   if (value)
      this.counter+= value;
   else
      this.counter++;
};

PromiseCounter.prototype.resolve = function( index ) {

   this.counter--;

   if (this.counter <= 0)
      this.callback( this.param );
};