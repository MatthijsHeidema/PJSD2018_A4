<!DOCTYPE HTML>
<html>
	<head>
		<title>Home automation</title>
		<link rel="stylesheet" type="text/css" href="style.css"/>
	</head>
<body>
		<ul>
			<li><a class="active" href="index.php">Timothy</a></li>
			<li><a href="Bewaker.php">Guard</a></li>
			<li><a href="Verpleegster.php">Nurse</a></li>
		</ul>

			<main>
				<h1> Home Automation System for Timothy </h1>

				<?php
					ini_set('display_errors', 'On');
					error_reporting(E_ALL);

					define("LAMP_PATH", "lamp.json");
					define("WINDOW_PATH", "window.json");

					function updatePHP($path) {
						$jsonFile = fopen($path, "r") or die("Unable to open file");
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

					$window = updatePHP(WINDOW_PATH);
					$lamp = updatePHP(LAMP_PATH);

					if ($lamp->{'ToggleLed'}) {
						$lampStatus = "aan";
					} else {
						$lampStatus = "uit";
					}
					
					if ($window->{'windowStatus'} == "1") {
						$windowStatus = "dicht";
					} else {
						$windowStatus = "open";
					}

				?>

				<p>
					<form method="post">
						<input id="notAButton"		type="submit" name="notAButton"		value="<?php echo $lamp->{'Color'} ?>"				/><br>
						<input id="controlButton"	type="submit" name="ToggleLed_lamp"	value="Lamp <?php echo $lampStatus; ?>" 			/>
						<input id="controlButton"	type="submit" name="windowStatus" 	value="Raam: <?php echo $windowStatus;?>"			/><br>
						<input id="whiteButton" 	type="submit" name="Color_lamp" 	value="White"	 									/>
						<input id="greenButton" 	type="submit" name="Color_lamp" 	value="Green"	 									/><br>
						<input id="redButton" 		type="submit" name="Color_lamp" 	value="Red"		 									/>
						<input id="blueButton" 		type="submit" name="Color_lamp" 	value="Blue"	 									/><br>
						<input id="yellowButton"	type="submit" name="Color_lamp" 	value="Yellow"	 									/><br>
					</form>
				</p>

				<?php
					if(array_key_exists('ToggleLed_lamp', $_POST)){
						toggleLed($lamp, LAMP_PATH);
					}
					if(array_key_exists('Color_lamp', $_POST)){
						$lamp->{'Color'} = $_POST["Color_lamp"];
						updateJson($lamp, LAMP_PATH);
					}

					if(array_key_exists('windowStatus', $_POST))
					{
						if ($window->{'windowStatus'}) {
							$window->{'windowStatus'} = "0";
							$window->{'aangepast'} = "1";
						} else {
							$window->{'windowStatus'} = "1";
							$window->{'aangepast'} = "1";
						}
						updateJson($window, WINDOW_PATH);
					}
				?>
			</main>

			</br>

</body>

</html>
