<?php
  // list_entries.php
 // James Nikkel, Yale University, 2017.
  // james.nikkel@yale.edu
  //
session_start();
$req_priv = "basic";
include("db_login.php");
include("page_setup.php");
include("aux/make_data_plot.php");

echo ('<TABLE border="1" cellpadding="2" width=100%>');



$plot_type_array = array(
			 "Summary",
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


if ($_SESSION['choose_type'] == "Summary")
  {
    include("list_entries_summary.php");
  }

 else if ($_SESSION['choose_type'] == "Housings")
   {
     $temp = $_SESSION['choosen_housing'];
   
     echo ('<TR>');
     echo ('<TH align="left">');  echo ('Housing ID'); echo ('</TH>');
     echo ('<TH align="left">');  echo ('PMT ID');     echo ('</TH>');
     echo ('<TH align="left">');  echo ('Type');       echo ('</TH>');
     echo ('<TH align="left">');  echo ('Status');     echo ('</TH>');
     echo ('<TH align="left">');  echo ('Location');   echo ('</TH>');
     echo ('<TH align="left">');  echo ('Volume');   echo ('</TH>');
     echo ('</TR>');
    

     $table = "Housing";
     include("aux/get_last_table_id.php");
    
     for ($i=1; $i <= $last_id; $i++)
       {
	 $_SESSION['choosen_housing'] = $i;
	 /////////////  Get selected housing values:
	 include("aux/get_housing_vals.php");
	
	 echo ('<TR>');
	 echo ('<TD align="left">'); echo ($id);	      echo ('</TD>');
	 echo ('<TD align="left">'); echo ($pmt_id);   echo ('</TD>');
	 echo ('<TD align="left">'); echo ($pmt_type); echo ('</TD>');
	 echo ('<TD align="left">'); echo ($status);   echo ('</TD>');
	 echo ('<TD align="left">'); echo ($location); echo ('</TD>');
	 echo ('<TD align="left">'); echo ($volume); echo ('</TD>');
	 echo ('</TR>');
       }
    
     $_SESSION['choosen_housing'] = $temp;
   }

 else if ($_SESSION['choose_type'] == "PMTs")
   {
     {
       $temp = $_SESSION['choosen_pmt'];
   
       echo ('<TR>');
       echo ('<TH align="left">');  echo ('PMT ID');         echo ('</TH>');
       echo ('<TH align="left">');  echo ('Type');           echo ('</TH>');
       echo ('<TH align="left">');  echo ('Serial Number');  echo ('</TH>');
       echo ('<TH align="left">');  echo ('Housing ID');     echo ('</TH>');
       echo ('<TH align="left">');  echo ('Status');         echo ('</TH>');
       echo ('<TH align="left">');  echo ('Location');       echo ('</TH>');
       echo ('</TR>');
    

       $table = "PMT";
       include("aux/get_last_pmt_id.php");
    
       for ($i=1; $i <= $last_pmt_id; $i++)
	 {
	   $_SESSION['choosen_pmt'] = $i;
	   /////////////  Get selected housing values:
	   include("aux/get_pmt_vals.php");
	
	   echo ('<TR>');
	   echo ('<TD align="left">'); echo ($pmt_id);	       echo ('</TD>');
	   echo ('<TD align="left">'); echo ($pmt_type);       echo ('</TD>');
	   echo ('<TD align="left">'); echo ($pmt_serial);     echo ('</TD>');
	   echo ('<TD align="left">'); echo ($pmt_housing_id); echo ('</TD>');
	   echo ('<TD align="left">'); echo ($pmt_status);     echo ('</TD>');
	   echo ('<TD align="left">'); echo ($pmt_location);   echo ('</TD>');
	   echo ('</TR>');
	 }

       $_SESSION['choosen_pmt'] = $temp;
     }


   }

echo ('</TABLE>');

mysql_close($connection);
echo(' </body>');
echo ('</HTML>');
?>
