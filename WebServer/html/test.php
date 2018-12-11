<!DOCTYPE html>

<html>
<body>

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
	Switch value: <?php echo $jsonObject->{'Switch'} ?><br>
	ToggleLed value: <?php echo $jsonObject->{'ToggleLed'} ?><br>
	LedValue value: <?php echo $jsonObject->{'LedValue'} ?><br>
	<form method="post">
		<input type="submit" name="ToggleLed" value="Toggle Led" /><br>
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

</body>
</html>
