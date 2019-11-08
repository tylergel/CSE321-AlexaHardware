<?php
$mysqli = new mysqli("localhost", "tylergel", "tylergel", "water-filter-app");

/* check connection */
if ($mysqli->connect_errno) {
    printf("Connect failed: %s\n", $mysqli->connect_error);
    exit();
}
$sql_post = "INSERT INTO temperature (temperature) VALUES ('".$_GET['temperature']."')";
$mysqli->query($sql_post);

$sql_post = "INSERT INTO quality (quality) VALUES ('".$_GET['quality']."')";
$mysqli->query($sql_post);

$sql_post = "INSERT INTO level (level) VALUES ('".$_GET['level']."')";
$mysqli->query($sql_post);




$ansql = "SELECT * from pour";
$anresult = $mysqli->query($ansql);
$pour = [];

while($row = mysqli_fetch_assoc($anresult)){
     $pour = $row;
}



$response = json_encode($pour);
print_r($pour['pour']);
return $response;

?>
