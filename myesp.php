<?php
if ((isset($_GET['esp'])  AND strlen($_GET['esp']) == 11 ) AND isset($_GET['temp']) AND isset($_GET['humid'])) // On a les parametres
{
		// 1 : On force la conversion en nombre entier
		$_GET['temp'] = (float) $_GET['temp'];
		$_GET['humid'] = (float) $_GET['humid'];
		// 2 : Le nombre doit être compris entre 1 et 100
		if ($_GET['temp'] >= -20.00 AND $_GET['temp'] <= 60.00) 
		{   
			$ptemp = $_GET['temp'];
			echo 'th';
			if ($_GET['humid'] >= 20.00 AND $_GET['humid'] <= 100.00) 
			{   
				$phumid = $_GET['humid'];
				echo 'hu';
				if (substr(($_GET['esp']),0,3) == "ETH" ) // l'ID ESP est  ETH + 8 caracteres
				{   
					$pesp = $_GET['esp'];
					echo 'esp';
					//ok, insertion en base
					
					include 'quelbase.php';
					
					// Create connection
					$conn = new mysqli($servername, $username, $password, $dbname);
					// Check connection
					if ($conn->connect_error) {
						die("Connection failed: " . $conn->connect_error);
					}
					$date_insert = date("Y-m-d H:i:s");
					
					$sql = "INSERT INTO tab_esp_mesure (idesp, mes_th, mes_hu, date_mesure) 
							VALUES ('".$pesp."', '".$ptemp."', '".$phumid."', '".$date_insert."' )";

					if ($conn->query($sql) === TRUE) {
						echo " New record created successfully";
					} else {echo " error INSERT";}

					//purge des enregistrement > 8 jours
					$sql_DEL = "DELETE FROM `tab_esp_mesure` WHERE DATEDIFF(NOW(),`date_mesure`)>8";
					if ($conn->query($sql_DEL) === TRUE) {
						echo " Purge successfully";
					} else {echo " error DELETE";}
					
					//mise a jour de l IP publique actuelle
					$valueIP= $_SERVER["REMOTE_ADDR"];
					$ladate=date('Y-m-d H:i:s');
					$sql_LIP = 'UPDATE lip SET lip="'.$valueIP.'" , date= "'.$ladate.'" , comment= "'.$pesp.'" WHERE idx=1';		
					if ($conn->query($sql_LIP) === TRUE) {
						echo " Update IP successfully";
					} else {echo " error Update IP";}
					
					
					$conn->close();
					
				} else {echo 'err esp';}
			} else {echo 'err hu.'; }
		}else{ echo 'err th.';}
	}

else // Il manque des paramètres, on avertit le visiteur
{
	echo 'error';
}
?>