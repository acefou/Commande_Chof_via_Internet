<?php

function fquelip() {
include 'quelbase.php';

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$lip = "";

$sqlLIP = "SELECT `lip` FROM `lip` WHERE `idx`=1 ";
$result = $conn->query($sqlLIP);

if ($result->num_rows > 0) {
    // output data of each row
    while($row = $result->fetch_assoc()) {
		$lip = $row["lip"];
    }
} 
$conn->close();
//$lip='192.168.1.8';
return $lip;


}
?> 
