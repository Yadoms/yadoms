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
?>
<?
//getting parameters
$os = $_GET["os"];
$arch = $_GET["arch"];
$name = $_GET["name"];
$lang = $_GET["lang"];

$answer = array( 'os' => $os, 'arch' => $arch, 'name' => $name );

if (($os == null) || ($arch == null))
	sendAnswerAndExit(false, "os and arch GET parameters must be defined", $answer);

//if the name is defined we get information only on this item
if ($name != null)
{
	//we look for all directory located in /plugins/$os/$arch/$name/

	$dirName = "plugins/" . $os;

	if (!is_dir($dirName))
		sendAnswerAndExit(false, "os not found : " . $dirName, $answer);

	$dirName .= "/" . $arch;

	if (!is_dir($dirName))
		sendAnswerAndExit(false, "arch not found :" . $dirName, $answer);

	$dirName .= "/" . $name;

	if (!is_dir($dirName))
		sendAnswerAndExit(false, "plugin not found :" . $dirName, $answer);
	
	$dirName .= '/';
	
	//everything is ok we can look for all folders
	$folder = opendir($dirName);
	if (!$folder)
		sendAnswerAndExit(false, "Unable to open directory " . $dirName, $answer);
	
	$versionList = array();
	
	$iconName = "icon.png";
	
	while (($file = readdir($folder)) !== false)
	{
		if ((is_dir($dirName . $file)) && ($file != ".") && ($file != ".."))
		{
			//for each folder we look if there is a package.json file inside
			$fullPath = $dirName . $file;
			$packageJsonPath = $fullPath . "/" . "package.json";
			$zipFile = $fullPath . "/" . "package.zip";
			$jsonFilePresent = is_file($packageJsonPath);
			$iconPath = $fullPath . "/" . $iconName;
			if (!$jsonFilePresent)
			{
				//the package.json is not present we extract it from package.zip
				if (is_file($zipFile))
				{
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
			}
		}
	}
	$answer["versions"] = $versionList;
}
else
{
	//we looking for all plugins
}
sendAnswerAndExit(true, "", $answer);

?>