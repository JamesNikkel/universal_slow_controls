<?php
// get_last_pinwheel_id.php
// James Nikkel, Yale University, 2016
// james.nikkel@yale.edu
//


$query = "SELECT `Pinwheel_ID` FROM `Spacer_Pinwheel`  ORDER BY `Pinwheel_ID` DESC LIMIT 1";
$result = mysql_query($query);
if (!$result)
  die ("Could not query the database <br />" . mysql_error());


while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
  $last_pinwheel_id = $row['Pinwheel_ID'];

?>
