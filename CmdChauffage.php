 <?php
 
include 'quelip.php';

 $lip=fquelip();
 
//Envoi commande
 $Ladresse = "http://".$lip."/commandechauffage";
//$Ladresse = "http://192.168.1.8/commandechauffage";
$lines = file($Ladresse);

//echo $lines[0];
$Lestatus = (int)$lines[0];

?> 