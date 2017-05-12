<?php
 
echo ('<TR>');
echo ('<TH align="left">');  echo ('Number of Hamamatsu PMTs'); echo ('</TH>');
echo ('<TH align="left">');  echo ('Number of ET PMTs');     echo ('</TH>');
echo ('<TH align="left">');  echo ('Number of Housings');       echo ('</TH>');
echo ('<TH align="left">');  echo ('Number of Panels');     echo ('</TH>');
echo ('<TH align="left">');  echo ('Number of Pinwheels');   echo ('</TH>');
echo ('</TR>');

echo ('<TR>');	

$query = "SELECT COUNT(*) FROM PMT WHERE PMT_Type='Hamamatsu' ";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TD align="left">');  echo ($row[0]);	      echo ('</TD>');

$query = "SELECT COUNT(*) FROM PMT WHERE PMT_Type='ETL' ";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TD align="left">');  echo ($row[0]);	      echo ('</TD>');

$query = "SELECT COUNT(*) FROM Housing";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TD align="left">');  echo ($row[0]);	      echo ('</TD>');

$query = "SELECT COUNT(*) FROM Optical_Separator";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TD align="left">');  echo ($row[0]);	      echo ('</TD>');

$query = "SELECT COUNT(*) FROM Spacer_Pinwheel";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TD align="left">');  echo ($row[0]);	      echo ('</TD>');

echo ('</TR>');

echo ('<TR>');
echo ('<TH align="left">');  echo ('Number of Non-Failed Hamamatsu PMTs'); echo ('</TH>');
echo ('<TH align="left">');  echo ('Number of Non-Failed ET PMTs');     echo ('</TH>');
echo ('<TH align="left">');  echo ('Number of Non-Failed Housings');       echo ('</TH>');
echo ('<TH align="left">');  echo ('Number of Non-Failed Panels');     echo ('</TH>');
echo ('<TH align="left">');  echo ('Number of Non-Failed Pinwheels');   echo ('</TH>');
echo ('</TR>');

echo ('<TR>');	

$query = "SELECT COUNT(*) FROM PMT WHERE PMT_Type='Hamamatsu' AND Status!='Failed'";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TD align="left">');  echo ($row[0]);	      echo ('</TD>');

$query = "SELECT COUNT(*) FROM PMT WHERE PMT_Type='ETL' AND Status!='Failed'";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TD align="left">');  echo ($row[0]);	      echo ('</TD>');

$query = "SELECT COUNT(*) FROM Housing WHERE Status!='Failed'";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TD align="left">');  echo ($row[0]);	      echo ('</TD>');

$query = "SELECT COUNT(*) FROM Optical_Separator WHERE Status!='Failed'";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TD align="left">');  echo ($row[0]);	      echo ('</TD>');

$query = "SELECT COUNT(*) FROM Spacer_Pinwheel WHERE Status!='Failed'";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TD align="left">');  echo ($row[0]);	      echo ('</TD>');

echo ('</TR>');



$query = "SELECT COUNT(*) FROM Housing WHERE Status=='Complete'";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('Number of Housings Complete'); 
echo ($row[0]);	      echo ('<BR>');



?>
