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

			$bed = updatePHP(BED_PATH);
			$chair = updatePHP(CHAIR_PATH);
			$pillar = updatePHP(PILLAR_PATH);
			$fridge = updatePHP(FRIDGE_PATH);
			$door = updatePHP(DOOR_PATH);
			$main = updatePHP(MAIN_PATH);
			$window = updatePHP(WINDOW_PATH);

			if ($door->{'doorStatus'}) {
				$doorStatus = "open";
			} else {
				$doorStatus = "gesloten";
			}

			$updateTimeDay = $bed->{'updateTimeDay'};
			$updateTimeNight = $bed->{'updateTimeNight'};
			$windowStatus = $window->{'windowStatus'};
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
					<input type="submit" name="doorStatus" value="Deur: <?php echo $doorStatus; ?>" />
				</form><br><br><br><br>
				<form method="post">
					<input type="submit" value="Tijd voor melding overdag: <?php echo $updateTimeDay; ?>" />
					<input type="number" style="width: 7em" name="updateTimeDay" id="updateTimeDay" min="0" max="3600" />
				</form>
				<form method="post">
					<input type="submit" value="Tijd voor melding 's nachts: <?php echo $updateTimeNight; ?>" />
					<input type="number" style="width: 7em" name="updateTimeNight" id="updateTimeNight" min="0" max="3600" /><br>
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
					$bed->{'EpilepsieAanvalBed'} = "0";
					updateJson($bed, BED_PATH);
				}

				if(array_key_exists('Ack_EpilepsyChair', $_POST)){
					$chair->{'EpilepsieAanvalStoel'} = "0";
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
</body>

</html>
