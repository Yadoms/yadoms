<?
header('Content-Type: application/json');

/**
* this function send the json answer
*/
function sendAnswerAndExit($result, $message, $answer)
{
	$data = array ('result' => $result, 'message' => $message, 'data' => $answer);
	echo json_encode($data);
	//we can exit script
	exit;
}

function startsWith($haystack, $needle) {
    // search backwards starting from haystack length characters from the end
    return $needle === "" || strrpos($haystack, $needle, -strlen($haystack)) !== FALSE;
}

function endsWith($haystack, $needle) {
    // search forward starting from end minus needle length characters
    return $needle === "" || (($temp = strlen($haystack) - strlen($needle)) >= 0 && strpos($haystack, $needle, $temp) !== FALSE);
}

function remove_utf8_bom($text)
{
    $bom = pack('H*','EFBBBF');
    $text = preg_replace("/^$bom/", '', $text);
    return $text;
}

//Make a insensitive and begin with item search
function matchInArray($element, $array, $elementIfMatch)
{
	foreach ($array as $key => $value)
	{
		if (strchr(strtolower($element), $value))
			return $elementIfMatch;
	}
	return $element;
}
?>
<?
//variables initialization
$osWin = array( "windows", "win");
$osLinux = array("linux", "unix");
$osMac = array("darwin", "mac", "osx");

$archX86 = array("x86", "ia32", "i686");
$archX64 = array("x64", "ia64");

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

//if the name is defined we get information only on this item
//we look for all directory located in /plugins/$os/$arch/
	
$dirName = "plugins/" . $os;

if (!is_dir($dirName))
	sendAnswerAndExit(false, "os not found : " . $dirName, $answer);

$dirName .= "/" . $arch;

if (!is_dir($dirName))
	sendAnswerAndExit(false, "arch not found :" . $dirName, $answer);

$dirName .= '/';
	
//everything is ok we can look for all folders, 1 folder -> 1 plugin
$pluginDirIterator = opendir($dirName);

if (!$pluginDirIterator)
	sendAnswerAndExit(false, "Unable to open directory " . $dirName, $answer);

$pluginList = array();
	
while (($plugin = readdir($pluginDirIterator)) !== false)
{
	$pluginDirName = $dirName . $plugin;
		
	if ((is_dir($pluginDirName)) && ($plugin != ".") && ($plugin != ".."))
	{
		$pluginDirName .= "/";
		$pluginValid = false;
		
		$folder = opendir($pluginDirName);
		if (!$folder)
			sendAnswerAndExit(false, "Unable to open directory " . $pluginDirName, $answer);
		
		$versionList = array();
		
		$iconName = "icon.png";
		
		while (($file = readdir($folder)) !== false)
		{
			if ((is_dir($pluginDirName . $file)) && ($file != ".") && ($file != ".."))
			{
				//for each folder we look if there is a package.json file inside
				$fullPath = $pluginDirName . $file;
				$packageJsonPath = $fullPath . "/" . "package.json";
				$jsonFilePresent = is_file($packageJsonPath);
				$files = glob($fullPath . "/*.zip");
				if (count($files) >= 1)
				{
					$zipFile = $files[0];
					$iconPath = $fullPath . "/" . $iconName;
					if (!$jsonFilePresent)
					{
						//the package.json is not present we extract it from package.zip
						//we get zip content
						$zip = new ZipArchive;
						$res = $zip->open($zipFile);
						if ($res === TRUE) 
						{
							//we get package.json 
							$zip->extractTo($fullPath, "package.json");
							
							//we extract all locales files
							for ($i = 0; $i < $zip->numFiles; $i++) 
							{ 
								$entry = $zip->getNameIndex($i); 
								
								if ( substr( $entry, -1 ) == '/' ) 
									continue; // skip directories 
								
								if (startsWith($entry, "locales/"))
								{
									$localesDir = $fullPath . '/locales';
									//if the locales folder doesn't exists we create it
									if (!is_dir($localesDir))
										mkdir($localesDir);

									//we copy the file inside
									$ofp = fopen( $localesDir . '/' . basename($entry), 'w' ); 
									$fp = $zip->getStream( $entry ); 
									if (!$fp) 
										sendAnswerAndExit(false, 'Unable to extract the file', $answer); 
								
									while (!feof($fp)) 
										fwrite( $ofp, fread($fp, 8192)); 
									
									fclose($fp); 
									fclose($ofp); 
								}
								
								if (startsWith($entry, $iconName))
								{
									//we copy the file inside
									$ofp = fopen($iconPath, 'w'); 
									$fp = $zip->getStream( $entry ); 
									if (!$fp) 
										sendAnswerAndExit(false, 'Unable to extract the file', $answer); 
								
									while (!feof($fp)) 
										fwrite( $ofp, fread($fp, 8192)); 
									
									fclose($fp); 
									fclose($ofp); 
								}
							} 

							$zip->close(); 
							
							//we check if the json file is present again
							$jsonFilePresent = is_file($packageJsonPath);
						}
					}
				}
				
				//the file may be extracted or already present
				if ($jsonFilePresent)
				{
					//we can get package.json informations
					$content = file_get_contents($packageJsonPath);
					$encoding = mb_detect_encoding($content);
					$content = iconv($encoding, 'UTF-8', $content);
					$content = remove_utf8_bom($content);
					//the language file exist we take useful information
					$json = json_decode($content, true);

					//TODO : check json var
					$dir = array();
					$dir["name"] = $json["name"];
					$dir["description"] = $json["description"];
					$dir["version"] = $json["version"];
					$dir["releaseType"] = $json["releaseType"];
					$dir["author"] = $json["author"];
					$dir["url"] = $json["url"];
					$dir["credits"] = $json["credits"];
					$dir["downloadUrl"] = dirname($_SERVER["SCRIPT_URI"]) . "/" . $zipFile;
					
					//if the icon is present 
					if (is_file($iconPath))
						$dir["iconUrl"] = dirname($_SERVER["SCRIPT_URI"]) . "/" . $iconPath;

					//we manage the i18n
					$langFile = $fullPath . "/locales/" . $lang . ".json";
					if (is_file($langFile))
					{
						$content = file_get_contents($langFile);
						$encoding = mb_detect_encoding($content);
						$content = mb_convert_encoding($content, $encoding, "UTF-8");
						//the language file exist we take useful information
						$content = remove_utf8_bom($content);
						$lngJsonFile = json_decode($content, true);
						if ($lngJsonFile["description"] != null)
							$dir["description"] = $lngJsonFile["description"];
					}
									
					array_push($versionList, $dir);
					$pluginValid = true;
				}
			}
		}
		//if we have a valid plugin dir we add it to the answer list
		if ($pluginValid)
			$pluginList[$plugin] = $versionList;
	}
}
$answer["plugins"] = $pluginList;

sendAnswerAndExit(true, "", $answer);

?>