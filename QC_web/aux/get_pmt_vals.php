<?php
// get_pmt_vals.php
// James Nikkel, Yale University, 2016
// james.nikkel@yale.edu
//

if (empty($_SESSION['choosen_pmt']))
  $_SESSION['choosen_pmt'] = 1;
$pmt_id = (int)$_SESSION['choosen_pmt'];

$query = "SELECT * FROM `PMT` WHERE `PMT_ID` = ".$pmt_id;
$result = mysql_query($query);
if (!$result)
  die ("Could not query the database <br />" . mysql_error());

while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
  {
  
    foreach ($pmt_parameter_names as $parm_name)
      {
	if (isset($row[$parm_name]))
	  $pmt_parm_values[] = (double)$row[$parm_name];
	else
	  $pmt_parm_values[] = 0.0;
      }

    if (isset($row['PMT_Type']))
      $pmt_type = $row['PMT_Type'];
    else
      $pmt_type = "Unknown";
  
    if (isset($row['Serial_number']))
      $pmt_serial = $row['Serial_number'];
    else
      $pmt_serial = "";
  
    if (isset($row['Status']))
      $pmt_status = $row['Status'];
    else
      $pmt_status = "";
  
    if (isset($row['Location']))
      $pmt_location = $row['Location'];
    else
      $pmt_location = "";
  

    if (isset($row['Housing_ID']))
      $pmt_housing_id = (int)$row['Housing_ID'];
    else
      $pmt_housing_id = -1;
  
    if (isset($row['Note']))
      $pmt_note = $row['Note'];
    else
      $pmt_note = "";
  
    if (isset($row['Last_update']))
      $pmt_last_update = (int)$row['Last_update'];
    else
      $pmt_last_update = 0;
  }

$pmt_parm_values = array_combine($pmt_parameter_names, $pmt_parm_values);


?>
