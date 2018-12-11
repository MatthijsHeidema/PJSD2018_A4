<!DOCTYPE HTML>
<html>
	<head>
		<title>Home automation</title>
		<link rel="stylesheet" type="text/css" href="style.css"/>
	</head>
<body>
		<ul>
			<li><a class="active" href="index.html">Timothy</a></li>
			<li><a href="Bewaker.html">Gaurd</a></li>
			<li><a href="Verpleegster.html">Nurse</a></li>
		</ul>

			<main>
				<h1> Home Automation System for Timothy </h1>

<?php
	define("PATH_TO_JSON", "/home/pi/Documents/PI_client/client.json");
	$jsonFile = fopen(PATH_TO_JSON, r) or die("Unable to open file");
	$jsonString = fread($jsonFile, filesize(PATH_TO_JSON));
	fclose($jsonFile);

	$jsonObject = json_decode($jsonString);
	//var_dump($jsonObject);
?>

<p>
	Switch value: <?php echo $jsonObject->{'Switch'}; ?><br>
	LedValue value: <?php echo $jsonObject->{'LedValue'} ?><br>
	ToggleLed value: <?php echo $jsonObject->{'ToggleLed'} ?><br>
	<form method="post">
		<input type="submit" name="ToggleLed" id="ToggleLed" value="Toggle Lamp Led" />
		<input type="submit" name="ToggleLed" id="ToggleLed" value="Toggle Bed Led" />
		<input type="submit" name="ToggleLed" id="ToggleLed" value="Toggle Raam Venster Led" />
		<input type="submit" name="ToggleLed" id="ToggleLed" value="Toggle Raam Led-strip" />
		<input type="submit" name="ToggleLed" id="ToggleLed" value="Toggle Deur Led" /><br>
	</form>
	<form method="post">
		<input type="number" name="LedValue" id="LedValue" min="0" max="15"><br>
		<input type="submit" value="submit"><br>
	</form>
</p>

<?php
	if(array_key_exists('ToggleLed', $_POST)){
		test($jsonObject);
	}
	if(array_key_exists('LedValue', $_POST)){
		$jsonObject->{'LedValue'} = (int)$_POST["LedValue"];
		updateJson($jsonObject);
	}

	function test($jsonObject) {
		if ($jsonObject->{'ToggleLed'}) {
			$jsonObject->{'ToggleLed'} = 0;
		} else {
			$jsonObject->{'ToggleLed'} = 1;
		}
		updateJson($jsonObject);
	}

	function updateJson($jsonObject) {
		$jsonOutputString = json_encode($jsonObject);
		$jsonOutput = fopen(PATH_TO_JSON, "w") or die("Unable to open file");
		fwrite($jsonOutput, $jsonOutputString);
		fclose($jsonOutput);
		header("Refresh:0");
	}
?>
			</main>

			</br>

</body>

</html>
