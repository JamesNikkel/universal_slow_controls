<?php
  // slow_control_list_entries.php
 // James Nikkel, Yale University, 2016.
  // james.nikkel@yale.edu
  //
session_start();
$req_priv = "basic";
include("db_login.php");
include("page_setup.php");
include("aux/make_data_plot.php");

echo ('<TABLE border="1" cellpadding="2" width=100%>');




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
      $h_id[]  = (int)$row['ID'];
      $value[] = (double)$row[$parm_name];
    }  
  
  $plot_name = "jpgraph_cache/housingplot_".$parm_name.".png";
  $plot_title = $parm_name;
 
  make_data_plot($plot_name, $h_id, $value, $plot_title, 
		 $parm_name." (".$housing_parameter_units[$parm_name].")", 
		 $housing_parameter_targets[$parm_name],
		 $housing_parameter_targets_plus[$parm_name],
		 $housing_parameter_targets_minus[$parm_name]);
 
  echo ('<img src='.$plot_name.'>'); 

  echo ('</TD>');
  echo ('</TR>');   	
}




echo ('</TABLE>');



mysql_close($connection);
echo(' </body>');
echo ('</HTML>');
?>
