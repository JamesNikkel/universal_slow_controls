<?php
  // edit_separator.php
  // James Nikkel, Yale University, 2016
  // james.nikkel@yale.edu
  //

session_start();
$req_priv = "full";
include("db_login.php");
include("page_setup.php");

if (!empty($_POST['choosen_separator']))
  $_SESSION['choosen_separator'] = $_POST['choosen_separator'];

if (empty($_SESSION['choosen_separator']))
  $_SESSION['choosen_separator'] = 1;

//////////////////////////////////// New entry:

if (isset($_POST['new_separator']))
  {
  
    $query = "INSERT into `Optical_Separator` (`Separator_ID`, `Last_update`) VALUES (NULL, '".time()."')";
    $result = mysql_query($query);
    if (!$result)
      die ("Could not query the database <br />" . mysql_error());
  
    include("aux/get_last_separator_id.php");
    $_SESSION['choosen_separator'] = $last_separator_id;
  }

////////////////////////////   Goto specific entry:


if (isset($_POST['first_separator']))
  $_SESSION['choosen_separator'] = 1;

if (isset($_POST['last_separator']))
  {
    include("aux/get_last_separator_id.php");
    $_SESSION['choosen_separator'] = $last_separator_id;
  }
if (isset($_POST['prev_separator']))
  $_SESSION['choosen_separator'] -= 1;
  
if (isset($_POST['next_separator']))
  $_SESSION['choosen_separator'] += 1;

include("aux/get_last_separator_id.php");
if ($_SESSION['choosen_separator'] < 1)
  $_SESSION['choosen_separator'] = 1;

if ($_SESSION['choosen_separator'] > $last_separator_id)
  $_SESSION['choosen_separator'] =  $last_separator_id;


$separator_id = (int)$_SESSION['choosen_separator'];

////////////////////////////////////    Do table updates :

if (isset($_POST['separator_id']))
  {
  
    $_POST['separator_id'] = (int)$_POST['separator_id'];

    // clean up strings:
 
    $_POST['separator_status'] = trim($_POST['separator_status']);
    if (!get_magic_quotes_gpc())
      $_POST['separator_status'] = addslashes($_POST['separator_status']);

     $_POST['separator_location'] = trim($_POST['separator_location']);
    if (!get_magic_quotes_gpc())
      $_POST['separator_location'] = addslashes($_POST['separator_location']);

    $_POST['separator_note'] = trim($_POST['separator_note']);
    if (!get_magic_quotes_gpc())
      $_POST['separator_note'] = addslashes($_POST['separator_note']);
  
 
    $query = "UPDATE `Optical_Separator` SET `Status`=\"".$_POST['separator_status']."\", `Location`=\"".$_POST['separator_location']."\",
            `Note`=\"".$_POST['separator_note']."\", `Last_update`=".time()."  
            WHERE `Separator_ID` = ".$_POST['separator_id'];

    $result = mysql_query($query);
    if (!$result)
      die ("Could not query the database <br />" . mysql_error());

    foreach ($separator_parameter_names as $parm_name)
      {
	$update_field = 'separator_'.$parm_name;
	$update_val = (float)$_POST[$update_field];
	
	$query = "UPDATE `Optical_Separator` SET `".$parm_name."`=\"".$update_val."\" WHERE `Separator_ID` = ".$_POST['separator_id'];	

	$result = mysql_query($query);
	if (!$result)
	  die ("Could not query the database <br />" . mysql_error());
      }
  }



/////////////  Get selected separator values:

include("aux/get_separator_vals.php");

mysql_close($connection);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



echo ('<TABLE border="1" cellpadding="2" width=100%>');
echo ('<TR>');

echo ('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
echo ('<TH>');
echo ('<input type="submit" name="new_separator" value="New Separator" title="Generate a new separator entry" style="font-size: 10pt">');
echo ('</TH>');
echo ('</FORM>');

echo ('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
echo ('<TH>');
echo ('&#160 &#160 &#160 &#160 Choose separator ID: <input type="text" name="choosen_separator" size = 6>');
echo ('</TH>');
echo ('</FORM>');

echo ('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
echo ('<TH>');
echo ('<input type="submit" name="first_separator" value="Goto First Separator" title="Go to the first added separator" style="font-size: 10pt">');
echo ('<input type="submit" name="prev_separator"  value="Goto Previous Separator" title="Go to the separator before the currently selected one" style="font-size: 10pt">');
echo ('<input type="submit" name="next_separator"  value="Goto Next Separator" title="Go to the separator after the currently selected one" style="font-size: 10pt">');
echo ('<input type="submit" name="last_separator"  value="Goto Last Separator" title="Go to the last added separator" style="font-size: 10pt">');
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
echo ('Separator ID:  '.$separator_id);
echo ('</TD>');

echo ('<TD align="left" colspan=2>');
echo ('Entry Last updated:');
echo ('<BR>');
echo (date("G:i:s  M d, y", $separator_last_update));
echo ('</TD>');
echo ('</TR>');   

/////////////////////////////////////////////////////////////

foreach ($separator_parameter_names as $parm_name)
{
  echo ('<TR>');               
  echo ('<TD align="left" colspan = 4>');
  echo ($parm_name.' = <input type="text" name="separator_'.$parm_name.'" value="'.$separator_parm_values[$parm_name].'" size = 8> ('.$separator_parameter_units[$parm_name].')');
  echo ('</TD>');
  echo ('</TR>');   	
}

/////////////////////////////////////////////////////////////


echo ('<TR>');                
echo ('<TD align="left"  colspan = 2>');
echo ('Status: ');
echo ('<SELECT name="separator_status">');
foreach ($qc_status_array as $index)
{
  echo('<option ');
  if (strcmp($index, $separator_status)==0)
    echo ('selected');
  echo(' value="'.$index.'">  '.$index.'  </option>');
}
echo ('</SELECT>');
echo ('</TD>');

echo ('<TD align="left"  colspan = 2>');
echo ('Current location: ');
echo ('<SELECT name="separator_location">');
foreach ($item_location_array as $index)
{
  echo('<option ');
  if (strcmp($index, $separator_location)==0)
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
echo ('<TEXTAREA name="separator_note" rows="4" cols="60">');
echo ($separator_note);
echo ('</TEXTAREA>');
echo ('<BR>');
echo ('</TD>');
echo ('</TR>');


echo ('<TR>');                   
echo ('<TD align="center" colspan=4>');
echo ('<input type="hidden" name="separator_id" value="'.$separator_id.'">');
echo ('<input type="submit" name="void" value="submit">');
echo ('</TD>');
echo ('</TR>');
echo ('</FORM>');

echo ('</TABLE>');

echo ('</body>');
echo ('</HTML>');
?>
