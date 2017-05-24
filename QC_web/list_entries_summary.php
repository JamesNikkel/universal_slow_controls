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

echo ('<TR>');	
echo ('<TD colspan=5 align="center">'); echo ('---'); echo ('</TD>'); 
echo ('</TR>');

echo ('<TR>');	
$query = "SELECT COUNT(*) FROM Housing WHERE Status='Complete'";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TH colspan=5 align="left">'); echo ('Number of Housings Complete = '); echo ($row[0]); echo ('</TH>'); 
echo ('</TR>');


echo ('<TR>');	
$query = "SELECT COUNT(*) FROM Housing WHERE Status='Leak checked' OR Status='Burn in' OR Status='Oil filled' OR Status='Complete'";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TH colspan=5 align="left">'); echo ('Number of Housings Leak checked = '); echo ($row[0]); echo ('</TH>'); 
echo ('</TR>');

echo ('<TR>');	
$query = "SELECT COUNT(*) FROM Housing WHERE Status='Stuffed' OR Status='Leak checked' OR Status='Burn in' OR Status='Oil filled' OR Status='Complete'";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TH colspan=5 align="left">'); echo ('Number of Housings Stuffed = '); echo ($row[0]); echo ('</TH>'); 
echo ('</TR>');

echo ('<TR>');
echo ('<TD colspan=5 align="center">'); echo ('---'); echo ('</TD>');
echo ('</TR>');

echo ('<TR>');
$query = "SELECT COUNT(*) FROM Housing WHERE Status='Unchecked' ";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TH colspan=5 align="left">'); echo ('Number of housings waiting to be cleaned = '); echo ($row[0]); echo ('</TH>');
echo ('</TR>');

echo ('<TR>');	
$query = "SELECT COUNT(*) FROM Housing WHERE Status='First clean' ";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TH colspan=5 align="left">'); echo ('Number of housings waiting to be measured = '); echo ($row[0]); echo ('</TH>'); 
echo ('</TR>');

echo ('<TR>');
$query = "SELECT COUNT(*) FROM Housing WHERE Status='Measured' ";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TH colspan=5 align="left">'); echo ('Number of housings waiting to be soldered = '); echo ($row[0]); echo ('</TH>');
echo ('</TR>');

echo ('<TR>');
$query = "SELECT COUNT(*) FROM Housing WHERE Status='Base soldered' ";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TH colspan=5 align="left">'); echo ('Number of housings waiting to be stuffed = '); echo ($row[0]); echo ('</TH>');
echo ('</TR>');

echo ('<TR>');
$query = "SELECT COUNT(*) FROM Housing WHERE Status='Stuffed' ";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TH colspan=5 align="left">'); echo ('Number of housings waiting to be leak checked = '); echo ($row[0]); echo ('</TH>');
echo ('</TR>');

echo ('<TR>');
$query = "SELECT COUNT(*) FROM Housing WHERE Status='Leak checked' ";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TH colspan=5 align="left">'); echo ('Number of housings waiting to be burned in = '); echo ($row[0]); echo ('</TH>');
echo ('</TR>');

echo ('<TR>');
$query = "SELECT COUNT(*) FROM Housing WHERE Status='Burned in' ";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TH colspan=5 align="left">'); echo ('Number of housings waiting to be filled with oil = '); echo ($row[0]); echo ('</TH>');
echo ('</TR>');

echo ('<TR>');
$query = "SELECT COUNT(*) FROM Housing WHERE Status='Oil filled' ";
$result = mysql_query($query);
$row = mysql_fetch_row($result);
echo ('<TH colspan=5 align="left">'); echo ('Number of housings waiting for final check = '); echo ($row[0]); echo ('</TH>');
echo ('</TR>');


?>
