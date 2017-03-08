<?php
// get_pinwheel_vals.php
// James Nikkel, Yale University, 2016
// james.nikkel@yale.edu
//

if (empty($_SESSION['choosen_pinwheel']))
  $_SESSION['choosen_pinwheel'] = 1;
$pinwheel_id = (int)$_SESSION['choosen_pinwheel'];

$query = "SELECT * FROM `Spacer_Pinwheel` WHERE `Pinwheel_ID` = ".$pinwheel_id;
$result = mysql_query($query);
if (!$result)
  die ("Could not query the database <br />" . mysql_error());

$pinwheel_parm_values = array();

while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
  { 
    
    foreach ($pinwheel_parameter_names as $parm_name)
      {
	if (isset($row[$parm_name]))
	  $pinwheel_parm_values[] = (double)$row[$parm_name];
	else
	  $pinwheel_parm_values[] = 0;
      }

    
  if (isset($row['Loc_Column']))
    $pinwheel_column = (int)$row['Loc_Column'];
  else
    $pinwheel_column = -1;
   
  if (isset($row['Loc_Column']))
    $pinwheel_row = (int)$row['Loc_Column'];
  else
    $pinwheel_row = -1;

  if (isset($row['End']))
    $pinwheel_end = $row['End'];
  else
    $pinwheel_end = "";
   
  if (isset($row['Status']))
    $pinwheel_status = $row['Status'];
  else
    $pinwheel_status = "";

  if (isset($row['Location']))
    $pinwheel_location = $row['Location'];
  else
    $pinwheel_location = "";

  if (isset($row['Note']))
    $pinwheel_note = $row['Note'];
  else
    $pinwheel_note = "";
  if (isset($row['Last_update']))
    $pinwheel_last_update = (int)$row['Last_update'];
  else
    $pinwheel_last_update = 0;

 }

$pinwheel_parm_values = array_combine($pinwheel_parameter_names, $pinwheel_parm_values);

?>
