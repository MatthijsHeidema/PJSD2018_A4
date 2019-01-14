<!DOCTYPE HTML>
<html>
	<head>
		<title>Home automation</title>
		<link rel="stylesheet" type="text/css" href="style.css"/>
	</head>
<body>
		<ul>
			<li><a href="index.php">Timothy</a></li>
			<li><a  href="Bewaker.php">Guard</a></li>
			<li><a class="active" href="Verpleegster.php">Nurse</a></li>
		</ul>
		<?php
				ini_set('display_errors', 'On');
				error_reporting(E_ALL);
				
				define("DOOR_PATH", "door.json");
				define("CHAIR_PATH", "chair.json");
				define("BED_PATH", "bed.json");
				
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

				$door = updatePHP(DOOR_PATH);
				$bed = updatePHP(BED_PATH);
				$chair = updatePHP(CHAIR_PATH);
				$OutOfBedCount = $bed->{'OutOfBedCount'};
				$updateTimeDay = $bed->{'updateTimeDay'};
				$updateTimeNight = $bed->{'updateTimeNight'};
	
				if ($door->{'doorStatus'}) {
					$doorStatus = "open";
				} else {
					$doorStatus = "gesloten";
				}
			?>

			<main>
				<h1> Home Automation System for Nurse </h1>
				
				<form method="post">
					<input type="submit" name="doorStatus" value="Deur: <?php echo $doorStatus; ?>" />
				</form><br><br>
				<form method="post">
					<input type="submit" value="Tijd voor melding overdag: <?php echo $updateTimeDay; ?>" />
					<input type="number" style="width: 7em" name="updateTimeDay" id="updateTimeDay" min="0" max="3600" />
				</form>
				<form method="post">
					<input type="submit" value="Tijd voor melding 's nachts: <?php echo $updateTimeNight; ?>" />
					<input type="number" style="width: 7em" name="updateTimeNight" id="updateTimeNight" min="0" max="3600" /><br>
				</form>
				
				<p>Uw patient is afgelopen nacht <?php echo $OutOfBedCount ?> keer uit bed geweest.</p>
				
			</main>

			</br>
			
			<?php 
			
				if(array_key_exists('doorStatus', $_POST)){
						
					if ($door->{'doorStatus'}) {
						$door->{'doorStatus'} = "0";
					} else {
						$door->{'doorStatus'} = "1";
					}
					updateJson($door, DOOR_PATH);
				}
				
				if(array_key_exists('updateTimeDay', $_POST))
				{
					$bed->{'updateTimeDay'} = (int)$_POST["updateTimeDay"];
					$chair->{'updateTimeDay'} = (int)$_POST["updateTimeDay"];
					updateJson($bed, BED_PATH);
					updateJson($chair, CHAIR_PATH);
				}

				if(array_key_exists('updateTimeNight', $_POST))
				{
					$bed->{'updateTimeNight'} = (int)$_POST["updateTimeNight"];
					$chair->{'updateTimeNight'} = (int)$_POST["updateTimeNight"];
					updateJson($bed, BED_PATH);
					updateJson($chair, CHAIR_PATH);
				}
			?>

</body>

</html>
