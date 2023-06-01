<?php

	$servername = "localhost";
	$username = "username";
	$password = "password";
	$dbname = "dbname";
	$table = "igate_data";
	$secret = "SECRETCODE";
	
	// takes raw data from the request 
	$json = file_get_contents('php://input');
	// Converts it into a PHP object 
	$data = json_decode($json, true);
	if ($data['secret'] == $secret)
	{
		// extract JSON data
		$callsign    = $data['call'];
		$date        = gmdate("Y-m-d");
		$time        = $data['time'];
		$destination = $data['destination'];
		$path        = $data['path'];
		$rssi        = $data['rssi'];
		$signal      = $data['signal'];
		$snr         = $data['snr'];
		$payload     = $data['payload'];
		$lat         = $data['latitude'];
		$long        = $data['longitude'];
		$dx          = $data['distance'];
		
		// Create connection
		$conn = mysqli_connect($servername, $username, $password, $dbname);
		// Check connection
		if (!$conn) 
		{
			die("Connection failed: " . mysqli_connect_error());
		}	
	
	
		// Attempt insert query execution
		$sql = sprintf("INSERT INTO `$table` VALUES ('$callsign','$date','$time','$destination','$path','$rssi','$signal','$snr','%s','$lat','$long','$dx')",mysqli_real_escape_string($conn, $payload));
		
		if(mysqli_query($conn, $sql))
		{
			echo "[SQL] : Record inserted successfully.";
		} 
		else
		{
			echo "[ERROR] : Could not able to execute : $sql. : " . mysqli_error($conn);
		}
	
		// Close connection
		mysqli_close($conn);
	}
?>
