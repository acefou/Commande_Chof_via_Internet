 <?php

include_once 'quelip.php';
include 'myfonctions/fenvoimail.php';

$lip=fquelip();
//active commande
$Ladresse = "http://".$lip."/handleremcmdchof";
$lines = file($Ladresse);
$Lestatus = (int)$lines[0];

$mail_Data .= "Demande de changement de l'état du relais de chauffage via internet \r\n";
$mail_Data .= "Vérifier l'état.\r\n";

$CR_Mail = envoimail("Activation a distance", $mail_Data, 1);

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