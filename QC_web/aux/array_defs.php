<?php
// array_defs.php
// Part of the CLEAN slow control.  
// James Nikkel, Yale University, 2006, 2007, 2008, 2010
// james.nikkel@yale.edu
//
$select_times = array(
    "10s" => 10,
    "30s" => 30,
    "1m" => 1*60,
    "3m" => 3*60,
    "10m" => 10*60,
    "30m" => 30*60,
    "never" => -1 
    );

$num_msgs_array = array(
    "Last 10" => 10,
    "Last 50" => 50,
    "Last 100" => 100,
    "All" => -1,
    "From plot times" => -2,
    "First 10" => -10,
    "First 50" => -50,
    "First 100" => -100,
    );

$html_colours = array(
    "aqua", "black", "blue", 
    "gray", "green", "lime", "maroon", 
    "navy", "orange", "purple", "red", 
    "silver", "teal", "wheat", "white", "yellow"
    );

////////////////////////////////////////////////////////////////////////////////////////


$privilege_array = array();
$allowed_host_array = array();
$query = "SELECT `name`, `allowed_host` FROM `user_privileges` ORDER BY `name`";
$result = mysql_query($query);
if (!$result)	
  die ("Could not query the database for message types <br>" . mysql_error());

while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
{
    $privilege_array[] = $row['name'];
    $allowed_host_array[] = $row['allowed_host'];
}
$allowed_host_array = array($privilege_array, $allowed_host_array);
$privilege_array = make_unique($privilege_array);

////////////////////////////////////////////////////////////////////////////////////////

$qc_status_array = array(
			 "Unchecked", 
			 "In QC",  
			 "Complete",
			 "Failed",
			 );

$housing_qc_status_array = array(
				 "Unchecked", 
				 "First clean",
				 "Measured",  
				 "Base soldered",
				 "Stuffed",
				 "Leak checked",
				 "Burned in",
				 "Oil filled",
				 "Complete",
				 "Oil topped up",
				 "Final clean done",
				 "In detector",
				 "Fix Me!",
				 "Failed",
				 );


$pmt_qc_status_array = array(
			     "Unchecked", 
			     "In QC",
			     "Complete",
			     "Failed",
			     );


$pmt_type_array = array(
			"Hamamatsu",
			"ETL",
			"Unknown",
			);

$item_location_array = array(
			     "EAL104",
			     "EAL106",
			     "WL105",
			     "WL106",
			     "WL109",
			     "WL109A",
			     "WL110",
			     "WL111",
			     "ORNL",
			     "Other",
			     );


////////////////////////////////////////////////////////////////////////////////////////
$housing_parameter_names = array(
				 "A", 
				 "B", 
				 "C", 
				 "D", 
				 "E", 
				 "F",
				 "G",
				 "H",
				 "J",
				 "K",
				 "L",
				 "M",
				 "Cable_Length_BNC",
				 "Cable_Length_SHV",
				 /* "N", */
				 /* "P", */
				 /* "Q", */
				 /* "R", */
				 /* "S", */
				 /* "T", */
				 /* "U", */
				 /* "V", */
				 /* "W", */
				 /* "X", */
				 /* "Y", */
				 /* "Z", */
				 );
$housing_parameter_units = array(
				 "inches",
				 "inches",
				 "inches",
				 "inches",
				 "inches",
				 "inches",
				 "inches",
				 "inches",
				 "inches",
				 "inches",
				 "inches",
				 "inches",
				 "inches",
				 "inches",
				 /* "inches", */
				 /* "inches", */
				 /* "inches", */
				 /* "inches", */
				 /* "inches", */
				 /* "inches", */
				 /* "inches", */
				 /* "inches", */
				 /* "inches", */
				 /* "inches", */
				 /* "inches", */
				 /* "inches", */
				 );
$housing_parameter_units = array_combine($housing_parameter_names, $housing_parameter_units);


$housing_parameter_targets = array("A" => 5.72,
				   "B" => 5.72,
				   "C" => 5.43,
				   "D" => 5.43,
				   "E" => 5.43,
				   "F" => 5.43,
				   "G" => 5.63,
				   "H" => 5.63,
				   "J" => 13.75,
				   "K" => 13.75,
				   "L" => 13.75,
				   "M" => 13.75,
				   "Cable_Length_BNC"=> 192.0,
				   "Cable_Length_SHV"=> 192.0,
				   );

$housing_parameter_targets_plus = array("A" => 0.01,
					"B" => 0.01,
					"C" => 0.005,
					"D" => 0.005,
					"E" => 0.005,
					"F" => 0.005,
					"G" => 0.01,
					"H" => 0.01,
					"J" => 0.0,
					"K" => 0.0,
					"L" => 0.0,
					"M" => 0.0,
					"Cable_Length_BNC"=> 3.0,
					"Cable_Length_SHV"=> 3.0,
					);

$housing_parameter_targets_minus = array("A" => 0.01,
					 "B" => 0.01,
					 "C" => 0.00,
					 "D" => 0.00,
					 "E" => 0.00,
					 "F" => 0.00,
					 "G" => 0.01,
					 "H" => 0.01,
					 "J" => 0.0,
					 "K" => 0.0,
					 "L" => 0.0,
					 "M" => 0.0,
					 "Cable_Length_BNC"=> 3.0,
					 "Cable_Length_SHV"=> 3.0,
					 );

////////////////////////////////////////////////////////////////////////////////////////
$pmt_parameter_names = array(
			     "1_Cathode_Sens", 
			     "2_Anode_Sens",
			     "3_Anode_Dark_Current",
			     "4_Cathode_Blue_Sens",
			     "5_EBBV",
			     "ET_CB",
			     "ET_V_at_500A",
			     "ET_DC",
			     "ET_V_at_2000A",
			     "ET_V_at_G",
			     "ET_SER",
			     "ET_DCTS",
			     );

$pmt_parameter_units = array(
			     "uA/lm",
			     "A/lm",
			     "nA",
			     " ",
			     "V",
			     " ",
			     "V",
			     "nA",
			     "V",
			     "V",
			     " ",
			     "Kcps",
			     );

$pmt_parameter_units = array_combine($pmt_parameter_names, $pmt_parameter_units);


////////////////////////////////////////////////////////////////////////////////////////
$separator_parameter_names = array(
				   "Length", 	 	
				   "Max_Width", 
				   "Min_Width", 	
				   "Max_Thickness", 
				   "Min_Thickness", 
				   "Side_1_bubbles",
				   "Side_2_bubbles",
				   );	

$separator_parameter_units = array(
				   "inches",
				   "inches",
				   "inches",
				   "inches",
				   "inches",
				   "inch^2",
				   "inch^2",
				   );
$separator_parameter_units = array_combine($separator_parameter_names, $separator_parameter_units);



////////////////////////////////////////////////////////////////////////////////////////
$pinwheel_parameter_names = array(
				  "Length",
				  "Max_Arm_Thickness_1",
				  "Min_Arm_Thickness_1", 
				  "Max_Arm_Thickness_2", 
				  "Min_Arm_Thickness_2", 
				  "Max_Arm_Thickness_3", 
				  "Min_Arm Thickness_3", 
				  "Max_Arm_Thickness_4", 
				  "Min_Arm_Thickness_4", 
				  "(OD-ID)1", 
				  "(OD-ID)2", 
				  "(OD-ID)3",
				  "(OD-ID)4",	
				  "Spacer_OD1", 	
				  "Spacer_OD2",
				  "Pinwheel_OD_1",	
				  "Pinwheel_OD_2",
				  );
$pinwheel_parameter_units = array(
				  "inches",
				  "inches",
				  "inches",
				  "inches",
				  "inches",
				  "inches",
				  "inches",
				  "inches",
				  "inches",
				  "inches",
				  "inches",
				  "inches",
				  "inches",
				  "inches",
				  "inches",
				  "inches",
				  "inches",
				  );
$pinwheel_parameter_units = array_combine($pinwheel_parameter_names, $pinwheel_parameter_units);

/////////////////////////////////////////////////////////////////////////////////////////
?>
