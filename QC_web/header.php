<?php
// header.php
// James Nikkel, Yale University, 2016
// james.nikkel@yale.edu
//
echo('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');

////////////////   Check login info

if (!(empty($_POST['login'])))
  include("aux/login.php");

if (!(empty($_POST['logout'])))
  include("aux/logout.php"); 

////////////////   If there is nothing found, autologin as guest
include("aux/guest_login.php");

/////////////////////////////////////////////////////////   Table starts here!
echo('<font size="-1">');
echo('<TABLE border="0" cellpadding="2" width=100%>');
echo('<TR valign="center">');

///  Make a small form where we can choose the refresh time of the page.
///  Use refresh_time = -1 to never refresh
echo('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
echo('<TH align="left" width="25">');

echo('<input type="image" src="pixmaps/reload.png" alt="Refresh" title="Refresh page">');
echo('</TH>');
echo('</FORM>');

if (strpos($_SERVER['PHP_SELF'], "List_entries.php") === false)
{
    if (check_access($_SESSION['privileges'], "basic", $allowed_host_array))
    {
	echo('<TH>');
	echo('<A HREF="list_entries.php">List</A>');
	echo('</TH>'); 
    }
}

if (strpos($_SERVER['PHP_SELF'], "edit_housing.php") === false)
{
    if (check_access($_SESSION['privileges'], "full", $allowed_host_array))
    {
	echo('<TH>');
	echo('<A HREF="edit_housing.php">Housings</A>');
	echo('</TH>'); 
    }
}

if (strpos($_SERVER['PHP_SELF'], "edit_pmt.php") === false)
{
    if (check_access($_SESSION['privileges'], "full", $allowed_host_array))
    {
	echo('<TH>');
	echo('<A HREF="edit_pmt.php">PMTs</A>');
	echo('</TH>'); 
    }
}


if (strpos($_SERVER['PHP_SELF'], "edit_separator.php") === false)
{
    if (check_access($_SESSION['privileges'], "full", $allowed_host_array))
    {
	echo('<TH>');
	echo('<A HREF="edit_separator.php">Optical Separators</A>');
	echo('</TH>'); 
    }
}

if (strpos($_SERVER['PHP_SELF'], "edit_pinwheel.php") === false)
{
    if (check_access($_SESSION['privileges'], "full", $allowed_host_array))
    {
	echo('<TH>');
	echo('<A HREF="edit_pinwheel.php">Pinwheels</A>');
	echo('</TH>'); 
    }
}


echo('<TH align="right">');
echo('You are logged in as '.$_SESSION['user_name']);
echo(' from '.$_SERVER['REMOTE_ADDR'].'.');

if (strpos($_SESSION['privileges'], "guest") !== false)
{
    echo('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
    echo('<input type="hidden" name="login" value="1">');
    echo('<TH width="200">');
    echo('Username: <input type="text" name="user_name" size="10">');
    echo('</TH>');
    echo('<TH width="168">');
    echo('Password: <input type="password" name="password" size="10">');
    echo('</TH>');
    echo('<TH width="20">');
    echo('<input type="image" src="pixmaps/login.png" alt="Log in" title="Log in">');
    echo('</TH>');
    echo('</FORM>');
}
else
{
    echo('<TH>');
    echo('<FORM action="'.$_SERVER['PHP_SELF'].'" method="post">');
    echo('<input type="hidden" name="logout" value="1">');
    echo('<input type="image" src="pixmaps/logout.png" alt="Log out" title="Log out">');
    echo('</FORM>');
    echo('</TH>');
}
echo('</TH>');

echo('</TR>');
echo('</TABLE>');
echo('</font>');
//////////////////////////////////////  End  Header


//////////////////////////////////////  Check for access levels
if (!check_access($_SESSION['privileges'], $req_priv, $allowed_host_array))
{  			      
    echo('<br>');
    echo('<br>');
    echo('You do not have clearance to view this page.');
    echo('<br>');
    exit();
}
?>
