<?php
// get_separator_vals.php
// James Nikkel, Yale University, 2016
// james.nikkel@yale.edu
//

if (empty($_SESSION['choosen_separator']))
  $_SESSION['choosen_separator'] = 1;
$separator_id = (int)$_SESSION['choosen_separator'];

$query = "SELECT * FROM `Optical_Separator` WHERE `Separator_ID` = ".$separator_id;
$result = mysql_query($query);
if (!$result)
  die ("Could not query the database <br />" . mysql_error());

$separator_parm_values = array();

while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
  { 
    
    foreach ($separator_parameter_names as $parm_name)
      {
	if (isset($row[$parm_name]))
	  $separator_parm_values[] = (double)$row[$parm_name];
	else
	  $separator_parm_values[] = 0.0;
      }

    
  if (isset($row['Loc_Column']))
    $separator_column = (int)$row['Loc_Column'];
  else
    $separator_column = -1;
   
  if (isset($row['Loc_Column']))
    $separator_row = (int)$row['Loc_Column'];
  else
    $separator_row = -1;

  if (isset($row['Orientation']))
    $separator_orientation = $row['Orientation'];
  else
    $separator_orientation = "";
   
  if (isset($row['Status']))
    $separator_status = $row['Status'];
  else
    $separator_status = "";
    
  if (isset($row['Location']))
    $separator_location = $row['Location'];
  else
    $separator_location = "";

  if (isset($row['Note']))
    $separator_note = $row['Note'];
  else
    $separator_note = "";
  if (isset($row['Last_update']))
    $separator_last_update = (int)$row['Last_update'];
  else
    $separator_last_update = 0;

 }

$separator_parm_values = array_combine($separator_parameter_names, $separator_parm_values);

?>
