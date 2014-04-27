/**
 * Created by Nicolas on 09/04/14.
 */

module( "utility.js" );
test( "assert", function() {
   ok( assert(true, "test must be ok"), "assert() with an ok test" );
   throws( function() {
         assert(false, "test must be ko")}, "assert() with a ko test"
   );
});

test( "parseBool", function() {
   ok( parseBool("true", false) == true, "parseBool with \"true\" value");
   ok( parseBool("false", true) == false, "parseBool with \"false\" value");
   ok( parseBool("VRAI", false) == false, "parseBool with \"VRAI\" value");
   ok( parseBool(false, true) == false, "parseBool with false value");
   ok( parseBool(true, false) == true, "parseBool with true value");
   ok( parseBool(undefined, false) == false, "parseBool with undefined value");
   ok( parseBool(null, false) == false, "parseBool with null value");
   ok( parseBool("TRUE", false) == true, "parseBool with \"TRUE\" value");
   ok( parseBool("1", false) == true, "parseBool with \"1\" value");
   ok( parseBool("0", true) == false, "parseBool with \"0\" value");
   ok( parseBool("no", true) == false, "parseBool with \"no\" value");
   ok( parseBool("yes", false) == true, "parseBool with \"yes\" value");
});

test( "createUUID", function() {
   var uuid = createUUID();
   ok (uuid.length == 36, "uuid length" );
   ok (((uuid[8] == "-") && (uuid[13] == "-") && (uuid[18] == "-") && (uuid[23] == "-")), "uuid pattern" );
   for (var i = 1; i <= 1000; i++)
      ok (createUUID() != createUUID(), "uuid unique test n° " + i);
});

test( "isNullOrUndefined", function() {
   ok( isNullOrUndefined(undefined) == true, "isNullOrUndefined with undefined parameter");
   ok( isNullOrUndefined(null) == true, "isNullOrUndefined with null parameter");
   ok( isNullOrUndefined("test") == false, "isNullOrUndefined with string parameter");
   ok( isNullOrUndefined(4) == false, "isNullOrUndefined with int parameter");
});