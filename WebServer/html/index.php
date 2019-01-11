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

					$windowStatus = $window->{'windowStatus'};
					if ($lamp->{'ToggleLed'}) {
						$lampStatus = "aan";
					} else {
						$lampStatus = "uit";
					}

				?>

				<p>
					Lamp is <?php echo $lampStatus ?><br>
					Lamp kleur: <?php echo $lamp->{'Color'} ?><br>
					<form method="post">
						<input type="submit" name="ToggleLed_lamp" value="Toggle Lamp Led" />
					</form>
					<form method="post">
						<input type="radio" name="Color_lamp" value="White" checked/> White <br>
						<input type="radio" name="Color_lamp" value="Green" /> Green <br>
						<input type="radio" name="Color_lamp" value="Red" /> Red <br>
						<input type="radio" name="Color_lamp" value="Blue" /> Blue <br>
						<input type="radio" name="Color_lamp" value="Yellow" /> Yellow <br>
						<input type="submit"/> <br>
					</form>
					<form method="post">
						<input type="submit" name="windowStatus" value="windowStatus: <?php echo $windowStatus;?>"/>
					</form>

					<div class="""slidecontainer">
	  				<input type="range" min="1" max="100" value="50" class="slider" id="slider">
	  				<p>Value: <span id="demo"></span></p>
					</div>
					<script>
						var slider = document.getElementById("slider");
						var output = document.getElementById("demo");
						output.innerHTML = slider.value;
						slider.oninput = function() {
	  				output.innerHTML = this.value;
						}
					</script>
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
