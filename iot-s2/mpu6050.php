<?php

$dbname = 'iot_s2';
$dbuser = 'root';  
$dbpass = ''; 
$dbhost = 'localhost'; 

$connect = @mysqli_connect($dbhost,$dbuser,$dbpass,$dbname);

if(!$connect){
	echo "Error: " . mysqli_connect_error();
	exit();
}

echo "Connection Success! ";

$ax = $_GET["ax"];
$ay = $_GET["ay"]; 
$az = $_GET["az"]; 

$gx = $_GET["gx"]; 
$gy = $_GET["gy"]; 
$gz = $_GET["gz"]; 


$query = "INSERT INTO mpu6050 (ax, ay, az, gx, gy, gz) VALUES ('$ax', '$ay', '$az', '$gx', '$gy', '$gz')";
$result = mysqli_query($connect,$query);

echo "Insertion Success!";

?>