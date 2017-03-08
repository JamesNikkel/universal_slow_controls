<?php
  // edit_housing.php
  // James Nikkel, Yale University, 2016
  // james.nikkel@yale.edu
  //

session_start();
$req_priv = "full";
include("db_login.php");
include("page_setup.php");

$table = "Housing";

if (!empty($_POST['choosen']))
  $_SESSION['choosen_housing'] = $_POST['choosen'];

if (empty($_SESSION['choosen_housing']))
  $_SESSION['choosen_housing'] = 1;

//////////////////////////////////// New entry:

if (isset($_POST['new']))
{
  
  $query = "INSERT into `".$table."` (`ID`, `Last_update`) VALUES (NULL, '".time()."')";
  $result = mysql_query($query);
  if (!$result)
    die ("Could not query the database <br />" . mysql_error());
  
  include("aux/get_last_table_id.php");
  $_SESSION['choosen_housing'] = $last_id;
}

////////////////////////////   Goto specific entry:



if (isset($_POST['first']))
  $_SESSION['choosen_housing'] = 1;

if (isset($_POST['last']))
{
  include("aux/get_last_housing_id.php");
  $_SESSION['choosen_housing'] = $last_id;
}
if (isset($_POST['prev']))
  $_SESSION['choosen_housing'] -= 1;
  
if (isset($_POST['next']))
  $_SESSION['choosen_housing'] += 1;

include("aux/get_last_housing_id.php");
if ($_SESSION['choosen_housing'] < 1)
  $_SESSION['choosen_housing'] = 1;

if ($_SESSION['choosen_housing'] > $last_id)
  $_SESSION['choosen_housing'] =  $last_id;


$id = (int)$_SESSION['choosen_housing'];

////////////////////////////////////    Do table updates :

if (isset($_POST['id']))
  {
  
    $_POST['id'] = (int)$_POST['id'];

    // clean up strings:
    $_POST['pmt_type'] = trim($_POST['pmt_type']);
    if (!get_magic_quotes_gpc())
      $_POST['pmt_type'] = addslashes($_POST['pmt_type']);
 
    $_POST['status'] = trim($_POST['status']);
    if (!get_magic_quotes_gpc())
      $_POST['status'] = addslashes($_POST['status']);
  
     $_POST['location'] = trim($_POST['location']);
    if (!get_magic_quotes_gpc())
      $_POST['location'] = addslashes($_POST['location']);
    
    $_POST['note'] = trim($_POST['note']);
    if (!get_magic_quotes_gpc())
      $_POST['note'] = addslashes($_POST['note']);
  
    $_POST['pmt_id'] = (int)$_POST['pmt_id'];

    $query = "UPDATE `Housing` SET `PMT_Type`=\"".$_POST['pmt_type']."\",  `Status`=\"".$_POST['status']."\",  `Location`=\"".$_POST['location']."\", `PMT_ID`=".$_POST['pmt_id'].", 
            `Note`=\"".$_POST['note']."\", `Last_update`=".time()."  
            WHERE `ID` = ".$_POST['id'];

    $result = mysql_query($query);
    if (!$result)
      die ("Could not query the database <br />" . mysql_error());

  
    foreach ($housing_parameter_names as $parm_name)
      {
	$update_val = (float)$_POST[$parm_name];
	
	$query = "UPDATE `Housing` SET `".$parm_name."`=\"".$update_val."\" WHERE `ID` = ".$_POST['id'];	

	$result = mysql_query($query);
	if (!$result)
	  die ("Could not query the database <br />" . mysql_error());
      }

  }



/////////////  Get selected housing values:

include("aux/get_housing_vals.php");

mysql_close($connection);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



echo ('<TABLE border="1" cellpadding="2" width=100%>');
echo ('<TR>');

echo ('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
echo ('<TH>');
echo ('<input type="submit" name="new" value="New Housing" title="Generate a new housing entry" style="font-size: 10pt">');
echo ('</TH>');
echo ('</FORM>');

echo ('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
echo ('<TH>');
echo ('&#160 &#160 &#160 &#160 Choose housing ID: <input type="text" name="choosen" size = 6>');
echo ('</TH>');
echo ('</FORM>');

echo ('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
echo ('<TH>');
echo ('<input type="submit" name="first" value="Goto First Housing" title="Go to the first added housing" style="font-size: 10pt">');
echo ('<input type="submit" name="prev"  value="Goto Previous Housing" title="Go to the housing before the currently selected one" style="font-size: 10pt">');
echo ('<input type="submit" name="next"  value="Goto Next Housing" title="Go to the housing after the currently selected one" style="font-size: 10pt">');
echo ('<input type="submit" name="last"  value="Goto Last Housing" title="Go to the last added housing" style="font-size: 10pt">');
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
echo ('<TD align="left" colspan = 1>');
echo ('Housing ID:  '.$id);
echo ('</TD>');

echo ('<TD align="left"  colspan = 1>');
echo ('Associated PMT ID: <input type="text" name="pmt_id" value="'.$pmt_id.'" size = 4> (-1 if none)');
echo ('</TD>');

echo ('<TD align="left" colspan = 1>');
echo ('PMT Type: ');
echo ('<SELECT name="pmt_type">');
foreach ($pmt_type_array as $index)
{
  echo('<option ');
  if (strcmp($index, $pmt_type)==0)
    echo ('selected');
  echo(' value="'.$index.'">  '.$index.'  </option>');
}
echo ('</SELECT>');
echo ('</TD>');

echo ('<TD align="left" colspan=1>');
echo ('Entry Last updated:');
echo ('<BR>');
echo (date("G:i:s  M d, y", $last_update));
echo ('</TD>');
echo ('</TR>');   

/////////////////////////////////////////////////////////////

foreach ($housing_parameter_names as $parm_name)
{
  echo ('<TR>');               
  echo ('<TD align="left" colspan = 4>');
  echo ($parm_name.' = <input type="text" name="'.$parm_name.'" value="'.$parm_values[$parm_name].'" size = 8> ('.$housing_parameter_units[$parm_name].')');
  echo ('</TD>');
  echo ('</TR>');   	
}

/////////////////////////////////////////////////////////////
 	    	    
echo ('<TR>');                
echo ('<TD align="left"  colspan = 2>');
echo ('Status: ');
echo ('<SELECT name="status">');
foreach ($qc_status_array as $index)
{
  echo('<option ');
  if (strcmp($index, $status)==0)
    echo ('selected');
  echo(' value="'.$index.'">  '.$index.'  </option>');
}
echo ('</SELECT>');
echo ('</TD>');

echo ('<TD align="left"  colspan = 2>');
echo ('Current location: ');
echo ('<SELECT name="location">');
foreach ($item_location_array as $index)
{
  echo('<option ');
  if (strcmp($index, $location)==0)
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
echo ('<TEXTAREA name="note" rows="4" cols="60">');
echo ($note);
echo ('</TEXTAREA>');
echo ('<BR>');
echo ('</TD>');
echo ('</TR>');


echo ('<TR>');                   
echo ('<TD align="center" colspan=4>');
echo ('<input type="hidden" name="id" value="'.$id.'">');
echo ('<input type="submit" name="void" value="submit">');
echo ('</TD>');
echo ('</TR>');
echo ('</FORM>');


echo ('</TABLE>');

echo ('</body>');
echo ('</HTML>');
?>
