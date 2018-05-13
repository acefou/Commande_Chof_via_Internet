<!DOCTYPE html>
<html>
  <head>
    <title>Monitor ACEFOU</title>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.5.0/Chart.min.js"></script>
	<meta charset="utf-8">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>    
	<link rel="stylesheet" type="text/css" href="style.css">
  </head>
  <body>
	<h1>Monitor ACEFOU</h1>
	<h2>Evolution de la T° moyenne par heure sur les 8 derniers jours</h2>
	<canvas id="myChart" width="1600" height="900"></canvas>
	<a href=MonitorACEFOU.php><button type="button" class="btn btn-primary btn-lg btn-block">Retour aux valeurs actuelles</button></a>
	<p>propulsé par ACEFOU</p>
    <script>
		<?php include('acqhisto.php'); ?>
		var ctx = document.getElementById("myChart");
		var myChart = new Chart(ctx, {
		  type: 'line',
		  data: {
			labels: DateH,
			datasets: [
			  { 
				data: Mes,
				borderColor: "#3e95cd",
				fill: false
			  }
			]
		  },
		  options: {
            legend: {
                display: false,
                labels: {
                    fontColor: "#000080",
                }
            }
		  }
		});
	</script>
  </body>
</html>
