<?
header('Content-Type: application/json');
require('helper.inc');
?>

<?
//getting parameters
$os = $_GET["os"];
$arch = $_GET["arch"];
$lang = $_GET["lang"];

$answer = array( 'inputParameters' => array('os' => $os, 'arch' => $arch));

if (($os == null) || ($arch == null))
	sendAnswerAndExit(false, "os and arch GET parameters must be defined", $answer);

//we manage param tolerance
$os = matchInArray($os, $osWin, "windows");
$os = matchInArray($os, $osLinux, "linux");
$os = matchInArray($os, $osMac, "darwin");

$arch = matchInArray($arch, $archX86, "x86");
$arch = matchInArray($arch, $archX64, "x64");

//we look for all directories located in /plugins/$os/$arch/
	
$dirName = "plugins/" . $os;

if (!is_dir($dirName))
	sendAnswerAndExit(false, "os folder not found : " . $dirName, $answer);

$dirName .= "/" . $arch;

if (!is_dir($dirName))
	sendAnswerAndExit(false, "arch folder not found :" . $dirName, $answer);

$dirName .= '/';

try
{
	$answer["plugins"] = buildItemList($dirName, $lang);
}
catch (Exception $e)
{
	sendAnswerAndExit(false, $e->getMessage(), $answer);
}

sendAnswerAndExit(true, "", $answer);
?>