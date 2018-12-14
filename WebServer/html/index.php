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
					define("LAMP_PATH", "lamp.json");
					
					function updatePHP($path) {
						$jsonFile = fopen($path, r) or die("Unable to open file");
						$jsonString = fread($jsonFile, filesize($path));
						fclose($jsonFile);
						return json_decode($jsonString);
					}

					function updateJson($jsonObject, $path) {
						$jsonOutputString = json_encode($jsonObject);
						$jsonOutput = fopen($path, "w") or die("Unable to write file");
						fwrite($jsonOutput, $jsonOutputString);
						fclose($jsonOutput);
						header("Refresh:0");
					}

					function toggleLed($jsonObject, $path) {
						if ($jsonObject->{'ToggleLed'}) {
							$jsonObject->{'ToggleLed'} = "0";
						} else {
							$jsonObject->{'ToggleLed'} = "1";
						}
						updateJson($jsonObject, $path);
					}

					$lamp = updatePHP(LAMP_PATH);
				?>

				<p>
					<!--Switch value: <?php echo $jsonObject->{'Switch'}; ?><br>
					//LedValue value: <?php echo $jsonObject->{'LedValue'} ?><br>-->
					Lamp ToggleLed value: <?php echo $lamp->{'ToggleLed'} ?><br>
					Lamp Color value: <?php echo $lamp->{'Color'} ?><br>
					<form method="post">
						<input type="submit" name="ToggleLed_lamp" value="Toggle Lamp Led" />
						<input type="submit" name="ToggleLed" value="Toggle Bed Led" />
						<input type="submit" name="ToggleLed" value="Toggle Raam Venster Led" />
						<input type="submit" name="ToggleLed" value="Toggle Raam Led-strip" />
						<input type="submit" name="ToggleLed" value="Toggle Deur Led" /><br>
					</form>
					<form method="post">
						<input type="radio" name="Color_lamp" value="White" checked/> White <br>
						<input type="radio" name="Color_lamp" value="Green" /> Green <br>
						<input type="radio" name="Color_lamp" value="Red" /> Red <br>
						<input type="radio" name="Color_lamp" value="Blue" /> Blue <br>
						<input type="radio" name="Color_lamp" value="Yellow" /> Yellow <br> 
						<input type="submit"/> <br>
					</form>
					<!--<form method="post">
						<input type="number" name="LedValue" id="LedValue" min="0" max="15"><br>
						<input type="submit" value="submit"><br>
					</form>-->
				</p>

				<?php
					if(array_key_exists('ToggleLed_lamp', $_POST)){
						toggleLed($lamp, LAMP_PATH);
					}
					if(array_key_exists('Color_lamp', $_POST)){
						$lamp->{'Color'} = $_POST["Color_lamp"];
						updateJson($lamp, LAMP_PATH);
					}
					/*if(array_key_exists('LedValue', $_POST)){
						$jsonObject->{'LedValue'} = (int)$_POST["LedValue"];
						updateJson($jsonObject);
					}*/
				?>
			</main>

			</br>

</body>

</html>
