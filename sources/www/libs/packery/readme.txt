Modified by lgm42 following https://github.com/metafizzy/packery/issues/348

To make this works for our needs we have changed packed method at line 178 of packery.js

- proto._getPackMethod = function() {
-   return this._getOption('horizontal') ? 'rowPack' : 'columnPack';
- };

+ proto._getPackMethod = function() {
+   return 'rowPack';
+ };