<!DOCTYPE HTML>
<html>
	<head>
		<title>Home automation</title>
		<link rel="stylesheet" type="text/css" href="style.css"/>
	</head>
<body>
		<ul>
			<li><a href="index.php">Timothy</a></li>
			<li><a class="active" href="Bewaker.html">Gaurd</a></li>
			<li><a href="Verpleegster.html">Nurse</a></li>
		</ul>

			
			<?php
					define("LAMP_PATH", "lamp.json");
					define("BED_PATH", "bed.json");
					
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
					$bed = updatePHP(BED_PATH);
			?>
			
			<h1> Home Automation System for Security Gaurd </h1>
			</br>
			Uit bed: <?php echo $bed->{'OutOfBed'} ?><br>
			Epilepsie-aanval: <?php echo $bed->{'EpilepsieAanval'} ?><br>
			<form method="post">
				<input type="submit" name="Ack_Epilepsy" value="Epilepsie aanval" />
				<!--<input type="submit" name="ToggleLed" value="Toggle Bed Led" />
				<input type="submit" name="ToggleLed" value="Toggle Raam Venster Led" />
				<input type="submit" name="ToggleLed" value="Toggle Raam Led-strip" />
				<input type="submit" name="ToggleLed" value="Toggle Deur Led" /><br>-->
			</form>
			

			<!--<section>
				<input type="text" name="Lamp Led" placeholder="Bezig met Uitlezen..."><br>
				<input type="text" name="Raam Led" placeholder="Bezig met Uitlezen..."><br>
				<input type="text" name="Deur Led" placeholder="Bezig met Uitlezen..."><br>
				<input type="text" name="Deur status" placeholder="Bezig met Uitlezen..."><br>
				<input type="text" name="Raam status" placeholder="Bezig met Uitlezen..."><br>

			</section>
			<section>


				<label>Led Lamp:<input type="checkbox" class="ios-switch green  bigswitch" checked /><div><div></div></div></label>
				<label>Led strip van Raam:<input type="checkbox" class="ios-switch green  bigswitch" checked /><div><div></div></div></label>
				<label>Led van Bed:<input type="checkbox" class="ios-switch green  bigswitch" checked /><div><div></div></div></label>
				<label>Deur:<input type="checkbox" class="ios-switch green  bigswitch" checked /><div><div></div></div></label>
				<label>Deur led:<input type="checkbox" class="ios-switch green  bigswitch" checked /><div><div></div></div></label>
				<label>Raam Venster:<input type="checkbox" class="ios-switch green  bigswitch" checked /><div><div></div></div></label>
			</section>-->


			</br>
			<?php
					if(array_key_exists('Ack_Epilepsy', $_POST)){
						$bed->{'EpilepsieAanval'} = "0";
						updateJson($bed, BED_PATH);
					}
			?>
</body>

</html>
