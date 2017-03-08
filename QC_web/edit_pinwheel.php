<?php
  // edit_pinwheel.php
  // James Nikkel, Yale University, 2016
  // james.nikkel@yale.edu
  //

session_start();
$req_priv = "full";
include("db_login.php");
include("page_setup.php");

if (!empty($_POST['choosen_pinwheel']))
  $_SESSION['choosen_pinwheel'] = $_POST['choosen_pinwheel'];

if (empty($_SESSION['choosen_pinwheel']))
  $_SESSION['choosen_pinwheel'] = 1;

//////////////////////////////////// New entry:

if (isset($_POST['new_pinwheel']))
{
  
  $query = "INSERT into `Spacer_Pinwheel` (`Pinwheel_ID`, `Last_update`) VALUES (NULL, '".time()."')";
  $result = mysql_query($query);
  if (!$result)
    die ("Could not query the database <br />" . mysql_error());
  
  include("aux/get_last_pinwheel_id.php");
  $_SESSION['choosen_pinwheel'] = $last_pinwheel_id;
}

////////////////////////////   Goto specific entry:



if (isset($_POST['first_pinwheel']))
  $_SESSION['choosen_pinwheel'] = 1;

if (isset($_POST['last_pinwheel']))
{
  include("aux/get_last_pinwheel_id.php");
  $_SESSION['choosen_pinwheel'] = $last_pinwheel_id;
}
if (isset($_POST['prev_pinwheel']))
  $_SESSION['choosen_pinwheel'] -= 1;
  
if (isset($_POST['next_pinwheel']))
  $_SESSION['choosen_pinwheel'] += 1;

include("aux/get_last_pinwheel_id.php");
if ($_SESSION['choosen_pinwheel'] < 1)
  $_SESSION['choosen_pinwheel'] = 1;

if ($_SESSION['choosen_pinwheel'] > $last_pinwheel_id)
  $_SESSION['choosen_pinwheel'] =  $last_pinwheel_id;


$pinwheel_id = (int)$_SESSION['choosen_pinwheel'];

////////////////////////////////////    Do table updates :

if (isset($_POST['pinwheel_id']))
{
  
  $_POST['pinwheel_id'] = (int)$_POST['pinwheel_id'];

  // clean up strings:

  $_POST['pinwheel_status'] = trim($_POST['pinwheel_status']);
  if (!get_magic_quotes_gpc())
    $_POST['pinwheel_status'] = addslashes($_POST['pinwheel_status']);
  
  $_POST['pinwheel_location'] = trim($_POST['pinwheel_location']);
  if (!get_magic_quotes_gpc())
    $_POST['pinwheel_location'] = addslashes($_POST['pinwheel_location']);
  
  $_POST['pinwheel_note'] = trim($_POST['pinwheel_note']);
  if (!get_magic_quotes_gpc())
    $_POST['pinwheel_note'] = addslashes($_POST['pinwheel_note']);
  
  
  $query = "UPDATE `Spacer_Pinwheel` SET  `Status`=\"".$_POST['pinwheel_status']."\", `Location`=\"".$_POST['pinwheel_location']."\", `Note`=\"".$_POST['pinwheel_note']."\", `Last_update`=".time()." WHERE `Pinwheel_ID` = ".$_POST['pinwheel_id'];
  
  $result = mysql_query($query);
  if (!$result)
    die ("Could not query the database <br />" . mysql_error());

    foreach ($pinwheel_parameter_names as $parm_name)
      {
	$update_field = 'pinwheel_'.$parm_name;
	$update_val = (float)$_POST[$update_field];
	
	$query = "UPDATE `Spacer_Pinwheel` SET `".$parm_name."`=\"".$update_val."\" WHERE `Pinwheel_ID` = ".$_POST['pinwheel_id'];	

	$result = mysql_query($query);
	if (!$result)
	  die ("Could not query the database <br />" . mysql_error());
      }

}



/////////////  Get selected pinwheel values:

include("aux/get_pinwheel_vals.php");

mysql_close($connection);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



echo ('<TABLE border="1" cellpadding="2" width=100%>');
echo ('<TR>');

echo ('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
echo ('<TH>');
echo ('<input type="submit" name="new_pinwheel" value="New Pinwheel" title="Generate a new pinwheel entry" style="font-size: 10pt">');
echo ('</TH>');
echo ('</FORM>');

echo ('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
echo ('<TH>');
echo ('&#160 &#160 &#160 &#160 Choose pinwheel ID: <input type="text" name="choosen_pinwheel" size = 6>');
echo ('</TH>');
echo ('</FORM>');

echo ('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
echo ('<TH>');
echo ('<input type="submit" name="first_pinwheel" value="Goto First Pinwheel" title="Go to the first added pinwheel" style="font-size: 10pt">');
echo ('<input type="submit" name="prev_pinwheel"  value="Goto Previous Pinwheel" title="Go to the pinwheel before the currently selected one" style="font-size: 10pt">');
echo ('<input type="submit" name="next_pinwheel"  value="Goto Next Pinwheel" title="Go to the pinwheel after the currently selected one" style="font-size: 10pt">');
echo ('<input type="submit" name="last_pinwheel"  value="Goto Last Pinwheel" title="Go to the last added pinwheel" style="font-size: 10pt">');
echo ('</TH>');
echo ('</FORM>');

echo ('</TR>');
echo ('</TABLE>');
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
echo ('<BR>');
echo ('<BR>');

echo ('<TABLE border="1" cellpadding="2" width=100%>');
 
echo ('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');

echo ('<TR>');                
echo ('<TD align="left" colspan = 2>');
echo ('Pinwheel ID:  '.$pinwheel_id);
echo ('</TD>');

echo ('<TD align="left" colspan=2>');
echo ('Entry Last updated:');
echo ('<BR>');
echo (date("G:i:s  M d, y", $pinwheel_last_update));
echo ('</TD>');
echo ('</TR>');   

/////////////////////////////////////////////////////////////

foreach ($pinwheel_parameter_names as $parm_name)
{
  echo ('<TR>');               
  echo ('<TD align="left" colspan = 4>');
  echo ($parm_name.' = <input type="text" name="pinwheel_'.$parm_name.'" value="'.$pinwheel_parm_values[$parm_name].'" size = 8> ('.$pinwheel_parameter_units[$parm_name].')');
  echo ('</TD>');
  echo ('</TR>');   	
}

/////////////////////////////////////////////////////////////


echo ('<TR>');                
echo ('<TD align="left"  colspan = 2>');
echo ('Status: ');
echo ('<SELECT name="pinwheel_status">');
foreach ($qc_status_array as $index)
{
  echo('<option ');
  if (strcmp($index, $pinwheel_status)==0)
    echo ('selected');
  echo(' value="'.$index.'">  '.$index.'  </option>');
}
echo ('</SELECT>');
echo ('</TD>');

echo ('<TD align="left"  colspan = 2>');
echo ('Current location: ');
echo ('<SELECT name="pinwheel_location">');
foreach ($item_location_array as $index)
{
  echo('<option ');
  if (strcmp($index, $pinwheel_location)==0)
    echo ('selected');
  echo(' value="'.$index.'">  '.$index.'  </option>');
}
echo ('</SELECT>');
echo ('</TD>');
echo ('</TR>');    

echo ('<TR>');                  
echo ('<TD align="left" colspan=4>');
echo ('Notes:');
echo ('<BR>');
echo ('<TEXTAREA name="pinwheel_note" rows="4" cols="60">');
echo ($pinwheel_note);
echo ('</TEXTAREA>');
echo ('<BR>');
echo ('</TD>');
echo ('</TR>');


echo ('<TR>');                   
echo ('<TD align="center" colspan=4>');
echo ('<input type="hidden" name="pinwheel_id" value="'.$pinwheel_id.'">');
echo ('<input type="submit" name="void" value="submit">');
echo ('</TD>');
echo ('</TR>');
echo ('</FORM>');

echo ('</TABLE>');

echo ('</body>');
echo ('</HTML>');
?>
