<?php
  // edit_pmt.php
  // James Nikkel, Yale University, 2016
  // james.nikkel@yale.edu
  //

session_start();
$req_priv = "full";
include("db_login.php");
include("page_setup.php");

if (!empty($_POST['choosen_pmt']))
  $_SESSION['choosen_pmt'] = $_POST['choosen_pmt'];

if (empty($_SESSION['choosen_pmt']))
  $_SESSION['choosen_pmt'] = 1;

//////////////////////////////////// New entry:

if (isset($_POST['new_pmt']))
{
  
  $query = "INSERT into `PMT` (`PMT_ID`, `Last_update`) VALUES (NULL, '".time()."')";
  $result = mysql_query($query);
  if (!$result)
    die ("Could not query the database <br />" . mysql_error());
  
  include("aux/get_last_pmt_id.php");
  $_SESSION['choosen_pmt'] = $last_pmt_id;
}

////////////////////////////   Goto specific entry:



if (isset($_POST['first_pmt']))
  $_SESSION['choosen_pmt'] = 1;

if (isset($_POST['last_pmt']))
{
  include("aux/get_last_pmt_id.php");
  $_SESSION['choosen_pmt'] = $last_pmt_id;
}
if (isset($_POST['prev_pmt']))
  $_SESSION['choosen_pmt'] -= 1;
  
if (isset($_POST['next_pmt']))
  $_SESSION['choosen_pmt'] += 1;

include("aux/get_last_pmt_id.php");
if ($_SESSION['choosen_pmt'] < 1)
  $_SESSION['choosen_pmt'] = 1;

if ($_SESSION['choosen_pmt'] > $last_pmt_id)
  $_SESSION['choosen_pmt'] =  $last_pmt_id;


$pmt_id = (int)$_SESSION['choosen_pmt'];

////////////////////////////////////    Do table updates :

if (isset($_POST['pmt_id']))
{
  
  $_POST['pmt_id'] = (int)$_POST['pmt_id'];

  // clean up strings:
  $_POST['pmt_type'] = trim($_POST['pmt_type']);
  if (!get_magic_quotes_gpc())
    $_POST['pmt_type'] = addslashes($_POST['pmt_type']);
 
  $_POST['pmt_status'] = trim($_POST['pmt_status']);
  if (!get_magic_quotes_gpc())
    $_POST['pmt_status'] = addslashes($_POST['pmt_status']);
  
   $_POST['pmt_location'] = trim($_POST['pmt_location']);
  if (!get_magic_quotes_gpc())
    $_POST['pmt_location'] = addslashes($_POST['pmt_location']);

  $_POST['pmt_serial'] = trim($_POST['pmt_serial']);
  if (!get_magic_quotes_gpc())
    $_POST['pmt_serial'] = addslashes($_POST['pmt_serial']);


  $_POST['pmt_note'] = trim($_POST['pmt_note']);
  if (!get_magic_quotes_gpc())
    $_POST['pmt_note'] = addslashes($_POST['pmt_note']);
  
  $_POST['pmt_housing_id'] = (int)$_POST['pmt_housing_id'];

  $query = "UPDATE `PMT` SET `PMT_Type`=\"".$_POST['pmt_type']."\", `Serial_number`=\"".$_POST['pmt_serial']."\", `Status`=\"".$_POST['pmt_status']."\", `Location`=\"".$_POST['pmt_location']."\",`Housing_ID`=".$_POST['pmt_housing_id'].", 
            `Note`=\"".$_POST['pmt_note']."\", `Last_update`=".time()."  
            WHERE `PMT_ID` = ".$_POST['pmt_id'];

  $result = mysql_query($query);
  if (!$result)
    die ("Could not query the database <br />" . mysql_error());
  
  
  foreach ($pmt_parameter_names as $parm_name)
    {
      $update_field = 'pmt_'.$parm_name;
      $update_val = (float)$_POST[$update_field];
	
      $query = "UPDATE `PMT` SET `".$parm_name."`=\"".$update_val."\" WHERE `PMT_ID` = ".$_POST['pmt_id'];	

      $result = mysql_query($query);
      if (!$result)
	die ("Could not query the database <br />" . mysql_error());
    }

}



/////////////  Get selected pmt values:

include("aux/get_pmt_vals.php");

mysql_close($connection);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



echo ('<TABLE border="1" cellpadding="2" width=100%>');
echo ('<TR>');

echo ('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
echo ('<TH>');
echo ('<input type="submit" name="new_pmt" value="New PMT" title="Generate a new PMT entry" style="font-size: 10pt">');
echo ('</TH>');
echo ('</FORM>');

echo ('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
echo ('<TH>');
echo ('&#160 &#160 &#160 &#160 Choose pmt ID: <input type="text" name="choosen_pmt" size = 6>');
echo ('</TH>');
echo ('</FORM>');

echo ('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
echo ('<TH>');
echo ('<input type="submit" name="first_pmt" value="Goto First PMT" title="Go to the first added PMT" style="font-size: 10pt">');
echo ('<input type="submit" name="prev_pmt"  value="Goto Previous PMT" title="Go to the PMT before the currently selected one" style="font-size: 10pt">');
echo ('<input type="submit" name="next_pmt"  value="Goto Next PMT" title="Go to the PMT after the currently selected one" style="font-size: 10pt">');
echo ('<input type="submit" name="last_pmt"  value="Goto Last PMT" title="Go to the last added PMT" style="font-size: 10pt">');
echo ('</TH>');
echo ('</FORM>');

echo ('</TR>');
echo ('</TABLE>');


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

echo ('<BR>');
echo ('<BR>');

echo ('<TABLE border="1" cellpadding="2" width=100%>');
 
echo ('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');

echo ('<TR>');                
echo ('<TD align="left" colspan = 1>');
echo ('PMT ID:  '.$pmt_id);
echo ('</TD>');


echo ('<TD align="left"  colspan = 1>');
echo ('Associated Housing ID: <input type="text" name="pmt_housing_id" value="'.$pmt_housing_id.'" size = 4> (-1 if none)');
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
//echo ('</TD>');

//echo ('<TD align="left" colspan = 1>');
echo ('      Serial number: <input type="text" name="pmt_serial" value="'.$pmt_serial.'" size = 8> ');
echo ('</TD>');   


echo ('<TD align="left" colspan=1>');
echo ('Entry Last updated:');
echo ('<BR>');
echo (date("G:i:s  M d, y", $pmt_last_update));
echo ('</TD>');
echo ('</TR>');   
	    


/////////////////////////////////////////////////////////////

foreach ($pmt_parameter_names as $parm_name)
{
  echo ('<TR>');               
  echo ('<TD align="left" colspan = 4>');
  echo ($parm_name.' = <input type="text" name="pmt_'.$parm_name.'" value="'.$pmt_parm_values[$parm_name].'" size = 8> ('.$pmt_parameter_units[$parm_name].')');
  echo ('</TD>');
  echo ('</TR>');   	
}

/////////////////////////////////////////////////////////////
 	    	    
 	    
echo ('<TR>');                
echo ('<TD align="left"  colspan = 2>');
echo ('Status: ');
echo ('<SELECT name="pmt_status">');
foreach ($pmt_qc_status_array as $index)
{
  echo('<option ');
  if (strcmp($index, $pmt_status)==0)
    echo ('selected');
  echo(' value="'.$index.'">  '.$index.'  </option>');
}
echo ('</SELECT>');
echo ('</TD>');

echo ('<TD align="left"  colspan = 2>');
echo ('Current location: ');
echo ('<SELECT name="pmt_location">');
foreach ($item_location_array as $index)
{
  echo('<option ');
  if (strcmp($index, $pmt_location)==0)
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
echo ('<TEXTAREA name="pmt_note" rows="4" cols="60">');
echo ($pmt_note);
echo ('</TEXTAREA>');
echo ('<BR>');
echo ('</TD>');
echo ('</TR>');


echo ('<TR>');                   
echo ('<TD align="center" colspan=4>');
echo ('<input type="hidden" name="pmt_id" value="'.$pmt_id.'">');
echo ('<input type="submit" name="void" value="submit">');
echo ('</TD>');
echo ('</TR>');
echo ('</FORM>');



echo ('</TABLE>');

echo ('</body>');
echo ('</HTML>');
?>
