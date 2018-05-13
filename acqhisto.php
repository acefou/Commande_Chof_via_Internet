<?php
 //acefou 26/02/18
 //génère les tableaux de variables Y Y utilisés dans le script js qui lui construit le graphique 
include 'quelbase.php';

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sqlX = "SELECT concat(DATE_FORMAT(`date_mesure`, '%Y-%m-%d'),' ', HOUR(`date_mesure`),'h') as DateH FROM `tab_esp_mesure` GROUP BY YEAR(`date_mesure`), DAY(`date_mesure`), HOUR(`date_mesure`) ORDER BY `date_mesure` ASC; ";
$sqlY = "SELECT FORMAT(AVG(`mes_th`),2) AS moyenne FROM `tab_esp_mesure` GROUP BY YEAR(`date_mesure`), DAY(`date_mesure`), HOUR(`date_mesure`) ORDER BY `date_mesure` ASC; ";

$result = $conn->query($sqlX);
if ($result->num_rows > 0) {
	$tabD = "var DateH = [";
    while ($row = $result->fetch_row()) {
        $tabD = $tabD."'".$row[0]."',";
    }
	$tabD= substr($tabD, 0,strlen($tabD)-1);
	$tabD= $tabD.'];';	
	echo $tabD;
}
$result = $conn->query($sqlY);
if ($result->num_rows > 0) {
	$tabM = "var Mes = [";
	while ($row = $result->fetch_row()) {
        $tabM = $tabM.$row[0].",";
    }
	$tabM= substr($tabM, 0,strlen($tabM)-1);
	$tabM= $tabM.'];';	
	echo $tabM;
}

$conn->close();
?> 

