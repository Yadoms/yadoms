<?
header('Content-Type: application/json');
require('helper.inc');
?>

<?
//getting parameters
$lang = $_GET["lang"];

$answer = array( 'inputParameters' => array('lang' => $lang));

//we look for all directories located in /widgets/
try
{
	$answer["widgets"] = buildItemList("widgets/", $lang);
}
catch (Exception $e)
{
	sendAnswerAndExit(false, $e->getMessage(), $answer);
}

sendAnswerAndExit(true, "", $answer);
?>