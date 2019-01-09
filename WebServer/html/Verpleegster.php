<!DOCTYPE HTML>
<html>
	<head>
		<title>Home automation</title>
		<link rel="stylesheet" type="text/css" href="style.css"/>
	</head>
<body>
		<ul>
			<li><a href="index.php">Timothy</a></li>
			<li><a  href="Bewaker.php">Gaurd</a></li>
			<li><a class="active" href="Verpleegster.php">Nurse</a></li>
		</ul>
		<?php
				ini_set('display_errors', 'On');
				error_reporting(E_ALL);
				
				define("DOOR_PATH", "door.json");
				
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
	
				if ($door->{'ToggleDoor'}) {
					$doorStatus = "open";
				} else {
					$doorStatus = "gesloten";
				}
			?>

			<main>
				<h1> Home Automation System for Nurse </h1>
				
				<form method="post">
					<input type="submit" name="ToggleDoor" value="Deur: <?php echo $doorStatus; ?>" />
				</form><br>
				
				<table style="width:100%">
					<tr>
						<th>SleepCycle</th>
						<th>SitCycle</th>
						<th>Cycle</th>
					</tr>
					<tr>
						<td>"    "</td>
						<td>"    "</td>
						<td>"    "</td>
					</tr>
					<tr>
						<td>"    "</td>
						<td>"    "</td>
						<td>"    "</td>
					</tr>
					<tr>
						<td>"    "</td>
						<td>"    "</td>
						<td>"    "</td>
					</tr>
					<tr>
						<td>"    "</td>
						<td>"    "</td>
						<td>"    "</td>
					</tr>
					<tr>
						<td>"    "</td>
						<td>"    "</td>
						<td>"    "</td>
					</tr>
				</table>
			</main>

			</br>
			
			<?php 
			
				if(array_key_exists('ToggleDoor', $_POST)){
						
					if ($door->{'ToggleDoor'}) {
						$door->{'ToggleDoor'} = "0";
					} else {
						$door->{'ToggleDoor'} = "1";
					}
					updateJson($door, DOOR_PATH);
				}
			?>

</body>

</html>