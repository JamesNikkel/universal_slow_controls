<?php
  // plot_entries.php
 // James Nikkel, Yale University, 2017.
  // james.nikkel@yale.edu
  //
session_start();
$req_priv = "basic";
include("db_login.php");
include("page_setup.php");
include("aux/make_data_plot.php");



$plot_type_array = array(
			"Housings",
			"PMTs",
			"Pinwheels",
			"Panels",
			);


if (empty($_SESSION['choose_type']))
  $_SESSION['choose_type'] = $plot_type_array[0];

if (!(empty($_POST['choose_type'])))
  $_SESSION['choose_type'] = $_POST['choose_type'];


echo ('<TABLE border="1" cellpadding="2" width=100%>');
foreach ($plot_type_array as $index)
{
  echo ('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
  echo ('<TH>');
  echo ('<input type="submit" name="choose_type" value="'.$index.'" 
               title="'.$index.'" style="font-size: 10pt">');
  echo ('</TH>');
}

echo ('</TABLE>');


echo ('<TABLE border="1" cellpadding="2" width=100%>');

if ($_SESSION['choose_type'] == "Housings")
  {
    foreach ($housing_parameter_names as $parm_name)
      {
	echo ('<TR>');               
	echo ('<TD align="left">');
    
	$query = "SELECT ID, ".$parm_name." FROM Housing ORDER BY ID";

	$result = mysql_query($query);
	if (!$result)
	  die ("Could not query the database <br />" . mysql_error());
		
	$h_id  = array();
	$value = array();

	while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
	  {
	    if ($row[$parm_name] != 0)
	      {
		$h_id[]  = (int)$row['ID'];
		$value[] = (double)$row[$parm_name];
	      }
	  }
	
	$plot_name = "jpgraph_cache/housingplot_".$parm_name.".png";
	$plot_title = $parm_name;
 
	make_data_plot($plot_name, $h_id, $value, $plot_title, "Housing ID",
		       $parm_name." (".$housing_parameter_units[$parm_name].")", 
		       $housing_parameter_targets[$parm_name],
		       $housing_parameter_targets_plus[$parm_name],
		       $housing_parameter_targets_minus[$parm_name]);
 
	echo ('<img src='.$plot_name.'>'); 

	echo ('</TD>');
	echo ('</TR>');   	
      }
  }

 else if ($_SESSION['choose_type'] == "PMTs")
   {
     foreach ($pmt_parameter_names as $parm_name)
      {
	echo ('<TR>');               
	echo ('<TD align="left">');
    
	$query = "SELECT PMT_ID, ".$parm_name." FROM PMT ORDER BY PMT_ID";

	$result = mysql_query($query);
	if (!$result)
	  die ("Could not query the database <br />" . mysql_error());
		
	$h_id  = array();
	$value = array();

	while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
	  {
	    if ($row[$parm_name] != 0)
	      {
		$h_id[]  = (int)$row['PMT_ID'];
		$value[] = (double)$row[$parm_name];
	      }
	  }
  
	$plot_name = "jpgraph_cache/housingplot_".$parm_name.".png";
	$plot_title = $parm_name;
 
	make_data_plot($plot_name, $h_id, $value, $plot_title, "PMT ID",
		       $parm_name." (".$pmt_parameter_units[$parm_name].")", 
		       $pmt_parameter_targets[$parm_name],
		       $pmt_parameter_targets_plus[$parm_name],
		       $pmt_parameter_targets_minus[$parm_name]);
 
	echo ('<img src='.$plot_name.'>'); 

	$plot_name = "jpgraph_cache/housingplot_".$parm_name."_hist.png";
	make_data_hist($plot_name, $h_id, $value, $plot_title, "PMT ID",
		       $parm_name." (".$pmt_parameter_units[$parm_name].")", 
		       $pmt_parameter_targets[$parm_name],
		       $pmt_parameter_targets_plus[$parm_name],
		       $pmt_parameter_targets_minus[$parm_name]);

	echo ('<img src='.$plot_name.'>'); 

	echo ('</TD>');
	echo ('</TR>');   	
      }
   }


echo ('</TABLE>');



mysql_close($connection);
echo(' </body>');
echo ('</HTML>');
?>
