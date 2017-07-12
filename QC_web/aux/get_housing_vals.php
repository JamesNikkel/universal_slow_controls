<?php
// get_housing_vals.php
// James Nikkel, Yale University, 2016
// james.nikkel@yale.edu
//

if (empty($_SESSION['choosen_housing']))
  $_SESSION['choosen_housing'] = 1;
$id = (int)$_SESSION['choosen_housing'];

$query = "SELECT * FROM `Housing` WHERE `ID` = ".$id;
$result = mysql_query($query);
if (!$result)
  die ("Could not query the database <br />" . mysql_error());

$parm_values = array();

while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
  {
   
    foreach ($housing_parameter_names as $parm_name)
      {
	if (isset($row[$parm_name]))
	  $parm_values[] = (double)$row[$parm_name];
	else
	  $parm_values[] = 0.0;
      }

    if (isset($row['PMT_Type']))
      $pmt_type = $row['PMT_Type'];
    else
      $pmt_type = "Unknown";

    if (isset($row['PMT_ID']))
      $pmt_id = (int)$row['PMT_ID'];
    else
      $pmt_id = -1;

    if (isset($row['Loc_Column']))
      $loc_column = (int)$row['Loc_Column'];
    else
      $loc_column = -1;
   
    if (isset($row['Loc_Row']))
      $loc_row = (int)$row['Loc_Row'];
    else
      $loc_row = -1;

    if (isset($row['End']))
      $loc_end = $row['End'];
    else
      $loc_end = "";
   
    if (isset($row['Status']))
      $status = $row['Status'];
    else
      $status = "";

    if (isset($row['Location']))
      $location = $row['Location'];
    else
      $location = "";

    if (isset($row['Note']))
      $note = $row['Note'];
    else
      $note = "";

    if (isset($row['Last_update']))
      $last_update = (int)$row['Last_update'];
    else
      $last_update = 0;    
  }

$parm_values = array_combine($housing_parameter_names, $parm_values);

$length = ($parm_values["J"] + $parm_values["K"] + $parm_values["L"] + $parm_values["M"])/4.0;

$volume = $parm_values["A"]*$parm_values["B"]*0.5 + 0.75*5.67*5.67 +  $parm_values["G"]*$parm_values["H"]*0.625 +
  ($length - 0.5 - 0.75 - 0.625)*($parm_values["C"] + $parm_values["D"])*($parm_values["E"] + $parm_values["F"])/4.0;

$volume = $volume*2.54*2.54*2.54/1000.0;

$parm_values["Volume"] = $volume;

?>
