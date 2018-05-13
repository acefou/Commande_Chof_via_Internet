<!DOCTYPE html>
<html lang="fr">
  <head>
    <title>Monitor ACEFOU</title>
	<meta charset="utf-8">
	<meta http-equiv="refresh" content="5">
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
	<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>    
	<link rel="stylesheet" type="text/css" href="style.css">
	<script type="text/javascript"> function RemoteCommande() { <?php include('cmdremotechauffagejs.php'); ?>  } </script>
  </head>
  <body>
	<h1>Monitor ACEFOU</h1>
	<h2><?php include('acqact.php'); ?></h2>
	<div class="ex1"><?php include('acqthact.php'); ?></div>
	<div class="ex1"><?php include('acqhuact.php'); ?></div>
	<a href="listt.php"><button type="button" class="btn btn-primary btn-lg btn-block">Historique des 8 derniers jours</button></a>
	<!-- le php rempli ce type de button -->
	<?php include('gcommande.php'); ?>
    <p>v2.0 propulsé par ACEFOU</p>
  </body>
</html>
	