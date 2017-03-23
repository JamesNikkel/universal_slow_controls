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
echo ('<TH align="left">');  echo ('Number of Passed Hamamatsu PMTs'); echo ('</TH>');
echo ('<TH align="left">');  echo ('Number of Passed ET PMTs');     echo ('</TH>');
echo ('<TH align="left">');  echo ('Number of Passed Housings');       echo ('</TH>');
echo ('<TH align="left">');  echo ('Number of Passed Panels');     echo ('</TH>');
echo ('<TH align="left">');  echo ('Number of Passed Pinwheels');   echo ('</TH>');
echo ('</TR>');

echo ('<TR>');	

$query = "SELECT COUNT(*) FROM PMT WHERE PMT_Type='Hamamatsu' AND Status!='Failed QC' ";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TD align="left">');  echo ($row[0]);	      echo ('</TD>');

$query = "SELECT COUNT(*) FROM PMT WHERE PMT_Type='ETL' AND Status!='Failed QC'";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TD align="left">');  echo ($row[0]);	      echo ('</TD>');

$query = "SELECT COUNT(*) FROM Housing WHERE Status!='Failed QC'";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TD align="left">');  echo ($row[0]);	      echo ('</TD>');

$query = "SELECT COUNT(*) FROM Optical_Separator WHERE Status!='Failed QC'";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TD align="left">');  echo ($row[0]);	      echo ('</TD>');

$query = "SELECT COUNT(*) FROM Spacer_Pinwheel WHERE Status!='Failed QC'";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TD align="left">');  echo ($row[0]);	      echo ('</TD>');
 
?>
