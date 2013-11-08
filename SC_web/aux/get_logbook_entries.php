<?php
// get_logbook_entries.php
// Part of the CLEAN slow control.  
// James Nikkel, Yale University, 2009, 2010
// james.nikkel@yale.edu
//

$show_num = 7;

$query = "SELECT `entry_id` FROM `lug_entries` ORDER BY `entry_id` DESC LIMIT 1";
$result = mysql_query($query);
if (!$result)
    die ("Could not find logbook table. <br />" . mysql_error());

while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
  $lug_nums = (int)$row['entry_id'];

if (!isset($_SESSION['lug_indx_offset']))
  $_SESSION['lug_indx_offset'] = $lug_nums;

if (isset($_POST['lug_next']))
    $_SESSION['lug_indx_offset'] += ($show_num-1);
if (isset($_POST['lug_prev']))
   $_SESSION['lug_indx_offset'] -= ($show_num-1);
if (isset($_POST['lug_first']))
    $_SESSION['lug_indx_offset'] = $show_num+1;
if (isset($_POST['lug_last']))
    $_SESSION['lug_indx_offset'] = $lug_nums;

if ($_SESSION['lug_indx_offset'] > $lug_nums)
    $_SESSION['lug_indx_offset'] = $lug_nums;
if ($_SESSION['lug_indx_offset'] <  $show_num+1)
    $_SESSION['lug_indx_offset'] = $show_num+1;

if (isset($_POST['search_text']))
{
    if (!get_magic_quotes_gpc())   
	$_POST['search_text'] = addslashes($_POST['search_text']);
    $query = "SELECT * FROM `lug_entries` WHERE ((".$q_select.") AND (`entry_description` LIKE '%".$_POST['search_text']."%')) ORDER BY `action_time` DESC LIMIT ".$show_num;
}
else
  $query = "SELECT * FROM `lug_entries` WHERE ((".$q_select.") AND (`entry_id` <= ".$_SESSION['lug_indx_offset'].")) ORDER BY `action_time` DESC LIMIT ".$show_num;
$result = mysql_query($query);
if (!$result)
    die ("Could not find logbook table. <br />" . mysql_error());

$lug_entry_ids = array();
$lug_action_user = array();
$lug_action_time = array();
$lug_entry_user = array();
$lug_entry_time = array();
$lug_run_number = array();
$lug_category = array();
$lug_subcategory = array();
$lug_entry_description = array();
$lug_strikeme = array();
$lug_image_thumb = array();

while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
{	
    $lug_entry_ids[] = (int)$row['entry_id'];
    $lug_action_user[] = $row['action_user'];
    $lug_action_time[] = (int)$row['action_time'];
    $lug_entry_user[] = $row['edit_user'];
    $lug_entry_time[] = (int)$row['edit_time'];
    $lug_run_number[] = (int)$row['run_number'];
    $lug_category[] = $row['category'];
    $lug_subcategory[] = $row['subcategory'];
    $lug_entry_description[] = $row['entry_description'];
    $lug_strikeme[] = (int)$row['strikeme'];
    $lug_image_thumb[] = $row['entry_image_thumb'];
} 
$lug_action_user = array_combine($lug_entry_ids, $lug_action_user);
$lug_action_time = array_combine($lug_entry_ids, $lug_action_time);
$lug_entry_user = array_combine($lug_entry_ids, $lug_entry_user);
$lug_entry_time = array_combine($lug_entry_ids, $lug_entry_time);
$lug_run_number = array_combine($lug_entry_ids, $lug_run_number);
$lug_category = array_combine($lug_entry_ids, $lug_category);
$lug_subcategory = array_combine($lug_entry_ids, $lug_subcategory);
$lug_entry_description = array_combine($lug_entry_ids, $lug_entry_description);
$lug_strikeme = array_combine($lug_entry_ids, $lug_trikeme);
$lug_image_thumb = array_combine($lug_entry_ids, $lug_image_thumb);
?>
