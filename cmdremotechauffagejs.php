 <?php
 
include_once 'quelip.php';

 $lip=fquelip();
// $Ladresse = "http://".$lip."/commandechauffage";
// $Ladresse = "http://192.168.1.8/commandechauffage";
 echo 'console.log("'.$lip.'");';
 
 //echo '$.get("http://'.$lip.'/remotecommandechauffage")';
 /*
 echo '$.post("http://'.$lip.'/handleremcmdchof")  
	.done(function() {  alert( "Data Loaded ");})
	.fail(function() {  alert( "error");})
 ';
 */
 
 echo '$.post("envoicmd.php")  
	.done(function() {  alert( "Commande envoyée");})
	.fail(function() {  alert( "error");})
 ';
 
// echo '$.get("gcommande.php")';
//echo 'console.log(" post cmdremote");';
//echo $Ladresse;
?> 
