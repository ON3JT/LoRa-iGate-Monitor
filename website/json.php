<?php
	header("Access-Control-Allow-Origin: *");
	
	$servername = "localhost";
	$username = "username";
	$password = "password";
	$dbname = "dbname";
	$table = "igate_data";
	
    $date = date("Y-m-d");
	
	if( isset($_GET['date']) ){
		$date = $_GET['date'];
	}
	
	// Create connection
	$conn = mysqli_connect($servername, $username, $password, $dbname);
	
	// Check connection
	if (!$conn) 
	{
		die("Connection failed: " . mysqli_connect_error());
	}	
	
	// Attempt insert query execution
	$query = "SELECT * FROM igate_data WHERE DATE = '$date' ORDER BY TIME DESC";
	$result = mysqli_query($conn, $query) or die("Error in Selecting " . mysqli_error($connection));

    //create an array
    $emparray = array();
    while($row =mysqli_fetch_assoc($result))
    {
        $emparray[] = $row;
    }
   // echo json_encode($emparray);
	echo json_encode($emparray, JSON_PRETTY_PRINT);
	
	// Close connection
	mysqli_close($conn);

?>