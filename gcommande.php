 <?php
	ini_set("display_errors",0);error_reporting(0);
	ini_set('max_execution_time', 0);
	include_once 'quelip.php';
	$lip=fquelip();
	//recherche l'état
	$Ladresse = "http://".$lip."/status";
	$lines = file($Ladresse);
	$Lestatus = (int)$lines[0];


	//determination du bouton
	switch ($Lestatus) {
		case 200:
			//vert
			echo '<a><button type="button" onclick="RemoteCommande()" class="btn btn-success btn-lg btn-block">Mise en Marche Chauffage</button></a>';
			 echo '<script>console.log("200")</script>';
			break;
		case 201:
			//rouge
			echo '<a><button type="button" onclick="RemoteCommande()" class="btn btn-danger btn-lg btn-block">Arr&#234;t Chauffage</button></a>';
			echo '<script>console.log("201")</script>';
			break;
		default:
			//gris et disable
			echo '<a><button type="button" class="btn btn-secondary btn-lg btn-block disabled">Pilotage chauffage non disponible</button></a>';
			echo '<script>console.log("indefini")</script>';

	}

?> 