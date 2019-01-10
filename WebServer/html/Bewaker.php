<!DOCTYPE HTML>

<html>
	<head>
		<title>Home automation</title>
		<link rel="stylesheet" type="text/css" href="style.css"/>
	</head>
	<meta http-equiv="refresh" content="2">
	
	<body>
		<ul>
			<li><a href="index.php">Timothy</a></li>
			<li><a class="active" href="Bewaker.php">Guard</a></li>
			<li><a href="Verpleegster.php">Nurse</a></li>
		</ul>
			
		<?php
			ini_set('display_errors', 'On');
			error_reporting(E_ALL);

			define("BED_PATH", "bed.json");
			define("CHAIR_PATH", "chair.json");
			define("PILLAR_PATH", "pillar.json");
			define("FRIDGE_PATH", "fridge.json");
			define("DOOR_PATH", "door.json");
			define("MAIN_PATH", "main.json");
			
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

			$bed = updatePHP(BED_PATH);
			$chair = updatePHP(CHAIR_PATH);
			$pillar = updatePHP(PILLAR_PATH);
			$fridge = updatePHP(FRIDGE_PATH);
			$door = updatePHP(DOOR_PATH);
			$main = updatePHP(MAIN_PATH);
			
			if ($door->{'ToggleDoor'}) {
				$doorStatus = "open";
			} else {
				$doorStatus = "gesloten";
			}
		?>
		
		<h1> Home Automation System for Security Guard </h1></br>
		
		<table style="width:100%">
			<td>
				<h2> Alarms:</h2><br>
				
				<?php if ($bed->{'OutOfBed'}) { ?>
				Client is te lang uit bed! <form method="post">
					<input type="submit" name="Ack_OutOfBed" value="Stop Alarm" />
				</form><br>
				<?php } ?>
				
				<?php if ($bed->{'EpilepsieAanvalBed'}) { ?>
				Epileptische aanval in bed! <form method="post">
					<input type="submit" name="Ack_EpilepsyBed" value="Stop Alarm" />
				</form><br>
				<?php } ?>
				
				<?php if ($chair->{'EpilepsieAanvalStoel'}) { ?>
				Epileptische aanval op de stoel! <form method="post">
					<input type="submit" name="Ack_EpilepsyChair" value="Stop Alarm" />
				</form><br>
				<?php } ?>
				
				<?php if ($pillar->{'SmokeAlarm'}) { ?>
				Rook gedetecteerd! <form method="post">
					<input type="submit" name="Ack_Smoke" value="Stop Alarm" />
				</form><br>
				<?php } ?>
				
				<?php if ($fridge->{'OpenTooLong'}) { ?>
				Koelkast te lang open! <form method="post">
					<input type="submit" name="Ack_Fridge" value="Stop Alarm" />
				</form><br>
				<?php } ?>
				
				<?php if ($main->{'TooLongAbsent'}) { ?>
				Client te lang niet gedetecteerd! <form method="post">
					<input type="submit" name="Ack_Missing" value="Stop Alarm" />
				</form><br>
				<?php } ?>
			</td>
			<td>
				<form method="post">
					<input type="submit" name="ToggleDoor" value="Deur: <?php echo $doorStatus; ?>" />
				</form><br>
			</td>

		
		<?php
				if(array_key_exists('Ack_OutOfBed', $_POST)){
					$bed->{'OutOfBed'} = "0";
					updateJson($bed, BED_PATH);
				}
				
				if(array_key_exists('Ack_Missing', $_POST)){
					$main->{'TooLongAbsent'} = "0";
					updateJson($main, MAIN_PATH);
				}
				
				if(array_key_exists('Ack_EpilepsyBed', $_POST)){
					$bed->{'EpilepsieAanval'} = "0";
					updateJson($bed, BED_PATH);
				}
				
				if(array_key_exists('Ack_EpilepsyChair', $_POST)){
					$chair->{'EpilepsieAanval'} = "0";
					updateJson($chair, CHAIR_PATH);
				}
				
				if(array_key_exists('Ack_Smoke', $_POST)){
					$pillar->{'SmokeAlarm'} = "0";
					updateJson($pillar, PILLAR_PATH);
				}
				
				if(array_key_exists('Ack_Fridge', $_POST)){
					$fridge->{'OpenTooLong'} = "0";
					updateJson($fridge, FRIDGE_PATH);
				}
				
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
