 <?php

include_once 'quelip.php';
$lip=fquelip();
//active commande
$Ladresse = "http://".$lip."/handleremcmdchof";
$lines = file($Ladresse);
$Lestatus = (int)$lines[0];


//determination du bouton
switch ($Lestatus) {
    case 200:
        echo '<script>console.log("200")</script>';
        break;
    case 201:
		echo '<script>console.log("201")</script>';
        break;
	default:
		echo '<script>console.log("indefini")</script>';

}

?> 