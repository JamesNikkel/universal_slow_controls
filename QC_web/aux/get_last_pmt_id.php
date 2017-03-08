<?php
// get_last_pmt_id.php
// James Nikkel, Yale University, 2016
// james.nikkel@yale.edu
//


$query = "SELECT `PMT_ID` FROM `PMT`  ORDER BY `PMT_ID` DESC LIMIT 1";
$result = mysql_query($query);
if (!$result)
  die ("Could not query the database <br />" . mysql_error());


while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
  $last_pmt_id = $row['PMT_ID'];

?>
