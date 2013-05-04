-- phpMyAdmin SQL Dump
-- version 3.4.10.2
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Aug 02, 2012 at 10:50 AM
-- Server version: 5.5.25
-- PHP Version: 5.3.8

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `control2`
--

-- --------------------------------------------------------

--
-- Table structure for table `daq_control`
--

CREATE TABLE IF NOT EXISTS `daq_control` (
  `entry_id` int(11) NOT NULL AUTO_INCREMENT,
  `lug_entry_id` int(11) NOT NULL,
  `utc` int(11) NOT NULL,
  `acquire_now` tinyint(1) NOT NULL,
  PRIMARY KEY (`entry_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `daq_control_tmp`
--

CREATE TABLE IF NOT EXISTS `daq_control_tmp` (
  `utc` int(11) NOT NULL,
  `user_name` text CHARACTER SET latin1 COLLATE latin1_general_cs NOT NULL,
  `tag` text CHARACTER SET latin1 COLLATE latin1_general_cs NOT NULL,
  `acquire_now` tinyint(1) NOT NULL,
  `xml_settings_file` text CHARACTER SET latin1 COLLATE latin1_general_cs NOT NULL,
  `end_status` text CHARACTER SET latin1 COLLATE latin1_general_cs NOT NULL,
  KEY `utc` (`utc`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

--
-- Table structure for table `daq_presets`
--

CREATE TABLE IF NOT EXISTS `daq_presets` (
  `entry_id` int(11) NOT NULL AUTO_INCREMENT,
  `preset` mediumblob NOT NULL,
  PRIMARY KEY (`entry_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `dqm_channel_data`
--

CREATE TABLE IF NOT EXISTS `dqm_channel_data` (
  `utc` int(11) NOT NULL,
  `runs` text COLLATE latin1_general_cs NOT NULL,
  `prefixes` text COLLATE latin1_general_cs NOT NULL,
  `dat_files` text COLLATE latin1_general_cs NOT NULL,
  `avg_event_rate` float NOT NULL,
  `avg_livetime` float NOT NULL,
  `pulse_rates` text COLLATE latin1_general_cs NOT NULL,
  `avg_pulse_lengths` text COLLATE latin1_general_cs NOT NULL,
  `avg_baselines_mv` text COLLATE latin1_general_cs NOT NULL,
  KEY `time` (`utc`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

-- --------------------------------------------------------

--
-- Table structure for table `dqm_control`
--

CREATE TABLE IF NOT EXISTS `dqm_control` (
  `utc_time` int(11) NOT NULL,
  KEY `utc_time` (`utc_time`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

-- --------------------------------------------------------

--
-- Table structure for table `dqm_event_data`
--

CREATE TABLE IF NOT EXISTS `dqm_event_data` (
  `utc_time` int(11) NOT NULL,
  `run` int(11) NOT NULL,
  `prefix` text COLLATE latin1_general_cs NOT NULL,
  `event_rate` float NOT NULL,
  `livetime` float NOT NULL,
  `deadtime` float NOT NULL,
  KEY `utc_time` (`utc_time`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

-- --------------------------------------------------------

--
-- Table structure for table `dqm_event_display`
--

CREATE TABLE IF NOT EXISTS `dqm_event_display` (
  `utc` int(11) NOT NULL,
  `picture` longblob NOT NULL,
  KEY `utc_time` (`utc`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

-- --------------------------------------------------------

--
-- Table structure for table `globals`
--

CREATE TABLE IF NOT EXISTS `globals` (
  `name` varchar(16) COLLATE latin1_general_cs NOT NULL,
  `int1` int(11) NOT NULL DEFAULT '0',
  `int2` int(11) NOT NULL DEFAULT '0',
  `int3` int(11) NOT NULL DEFAULT '0',
  `int4` int(11) NOT NULL DEFAULT '0',
  `double1` double NOT NULL DEFAULT '0',
  `double2` double NOT NULL DEFAULT '0',
  `double3` double NOT NULL DEFAULT '0',
  `double4` double NOT NULL DEFAULT '0',
  `string1` varchar(16) COLLATE latin1_general_cs DEFAULT NULL,
  `string2` varchar(16) COLLATE latin1_general_cs DEFAULT NULL,
  `string3` varchar(16) COLLATE latin1_general_cs DEFAULT NULL,
  `string4` varchar(16) COLLATE latin1_general_cs DEFAULT NULL,
  UNIQUE KEY `names` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

--
-- Dumping data for table `globals`
--

INSERT INTO `globals` (`name`, `int1`, `int2`, `int3`, `int4`, `double1`, `double2`, `double3`, `double4`, `string1`, `string2`, `string3`, `string4`) VALUES
('Master_alarm', 0, 0, 0, 0, 0, 0, 0, 0, '', '', '', ''),
('Title', 0, 0, 0, 0, 0, 0, 0, 0, 'Slow Control Sys', NULL, NULL, NULL),
('have_TS', 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL),
('have_RGA', 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL),
('have_Cams', 1, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL),
('have_LB', 1, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL),
('web_bg_colour', 0, 0, 0, 0, 0, 0, 0, 0, 'teal', NULL, NULL, NULL),
('web_text_colour', 0, 0, 0, 0, 0, 0, 0, 0, 'black', NULL, NULL, NULL),
('have_HV_crate', 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `lug_acq_sources`
--

CREATE TABLE IF NOT EXISTS `lug_acq_sources` (
  `source_name` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`source_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `lug_acq_sources`
--

INSERT INTO `lug_acq_sources` (`source_name`) VALUES
('Ba-133'),
('BG'),
('Co-57'),
('Cs-137'),
('Kr-83'),
('LED'),
('No Source');

-- --------------------------------------------------------

--
-- Table structure for table `lug_categories`
--

CREATE TABLE IF NOT EXISTS `lug_categories` (
  `category` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`category`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `lug_categories`
--

INSERT INTO `lug_categories` (`category`) VALUES
('Data Acquisition'),
('Test');

-- --------------------------------------------------------

--
-- Table structure for table `lug_entries`
--

CREATE TABLE IF NOT EXISTS `lug_entries` (
  `entry_id` int(11) NOT NULL AUTO_INCREMENT,
  `important_flag` tinyint(1) DEFAULT '0',
  `action_user` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `action_time` datetime NOT NULL,
  `edit_user` varchar(40) COLLATE utf8_unicode_ci DEFAULT NULL,
  `edit_time` datetime DEFAULT NULL,
  `run_number` int(11) NOT NULL,
  `category` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `subcategory` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `entry_description` longtext COLLATE utf8_unicode_ci NOT NULL,
  `entry_image` mediumblob,
  `entry_image_thumb` blob,
  `entry_file` mediumblob,
  `filename` char(40) COLLATE utf8_unicode_ci DEFAULT NULL,
  `source` varchar(40) COLLATE utf8_unicode_ci DEFAULT NULL,
  `strikeme` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`entry_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `lug_gas_inventory_entries`
--

CREATE TABLE IF NOT EXISTS `lug_gas_inventory_entries` (
  `entry_id` int(11) NOT NULL AUTO_INCREMENT,
  `entry_date` datetime NOT NULL,
  `location_name` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `location_gas_type` varchar(10) COLLATE utf8_unicode_ci NOT NULL,
  `gross_mass_kg` double(7,3) DEFAULT NULL,
  `net_gas_mass_kg` double(7,3) DEFAULT NULL,
  `comments` mediumtext COLLATE utf8_unicode_ci,
  `strikeme` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`entry_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `lug_gas_locations`
--

CREATE TABLE IF NOT EXISTS `lug_gas_locations` (
  `location_id` int(11) NOT NULL AUTO_INCREMENT,
  `location_name` varchar(20) COLLATE utf8_unicode_ci NOT NULL,
  `tare_mass_kg` double(6,3) DEFAULT NULL,
  `location_description` mediumtext COLLATE utf8_unicode_ci,
  PRIMARY KEY (`location_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `lug_gas_transfers`
--

CREATE TABLE IF NOT EXISTS `lug_gas_transfers` (
  `transfer_id` int(11) NOT NULL AUTO_INCREMENT,
  `action_category` varchar(12) COLLATE utf8_unicode_ci DEFAULT 'Gas Transfer',
  `transfer_user` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `transfer_date` datetime NOT NULL,
  `entry_user` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `entry_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `run_number` int(11) NOT NULL,
  `gas_type` varchar(10) COLLATE utf8_unicode_ci NOT NULL,
  `transferred_from` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `transferred_to` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `transfer_total` double(7,3) NOT NULL,
  `transfer_total_units` varchar(10) COLLATE utf8_unicode_ci NOT NULL,
  `strikeme` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`transfer_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `lug_gas_type`
--

CREATE TABLE IF NOT EXISTS `lug_gas_type` (
  `type_id` int(11) NOT NULL AUTO_INCREMENT,
  `type_name` varchar(10) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`type_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `lug_iqs`
--

CREATE TABLE IF NOT EXISTS `lug_iqs` (
  `entry_id` int(11) NOT NULL AUTO_INCREMENT,
  `submitted_by_user` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `entry_time` datetime NOT NULL,
  `last_edited_by_user` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `last_edited_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `iq_type` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `filename` char(40) COLLATE utf8_unicode_ci NOT NULL,
  `source` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `values_xml` mediumtext COLLATE utf8_unicode_ci NOT NULL,
  `comments` mediumtext COLLATE utf8_unicode_ci,
  `file_attach` mediumblob,
  `image_attach` mediumblob,
  `image_attach_thumb` blob,
  `strikeme` tinyint(1) DEFAULT '0',
  PRIMARY KEY (`entry_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `lug_iqs_type`
--

CREATE TABLE IF NOT EXISTS `lug_iqs_type` (
  `iq_name` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `description` text COLLATE utf8_unicode_ci,
  PRIMARY KEY (`iq_name`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `lug_iqs_type`
--

INSERT INTO `lug_iqs_type` (`iq_name`, `description`) VALUES
('Energy Calibration', ''),
('PMT Gain Calibration', ''),
('Electron Mean Free Path', ''),
('Detector Tilt', ''),
('PMT Afterpulsing', 'Afterpulsing measurements for PMTs');

-- --------------------------------------------------------

--
-- Table structure for table `lug_subcategories`
--

CREATE TABLE IF NOT EXISTS `lug_subcategories` (
  `category` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  `subcategory` varchar(40) COLLATE utf8_unicode_ci NOT NULL,
  PRIMARY KEY (`subcategory`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `lug_subcategories`
--

INSERT INTO `lug_subcategories` (`category`, `subcategory`) VALUES
('Test', 'Test'),
('Test', 'Test2');

-- --------------------------------------------------------

--
-- Table structure for table `msg_log`
--

CREATE TABLE IF NOT EXISTS `msg_log` (
  `msg_id` int(11) NOT NULL AUTO_INCREMENT,
  `time` int(11) NOT NULL,
  `ip_address` varchar(16) COLLATE latin1_general_cs DEFAULT NULL,
  `subsys` varchar(16) COLLATE latin1_general_cs DEFAULT NULL,
  `msgs` text COLLATE latin1_general_cs NOT NULL,
  `type` varchar(16) COLLATE latin1_general_cs NOT NULL,
  `is_error` tinyint(4) NOT NULL DEFAULT '0',
  PRIMARY KEY (`msg_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- Table structure for table `msg_log_types`
--

CREATE TABLE IF NOT EXISTS `msg_log_types` (
  `types` varchar(16) COLLATE latin1_general_cs NOT NULL,
  UNIQUE KEY `name` (`types`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

--
-- Dumping data for table `msg_log_types`
--

INSERT INTO `msg_log_types` (`types`) VALUES
('Alarm'),
('Alert'),
('Config.'),
('Error'),
('Setpoint'),
('Shifts');

-- --------------------------------------------------------

--
-- Table structure for table `runs`
--

CREATE TABLE IF NOT EXISTS `runs` (
  `num` int(11) NOT NULL AUTO_INCREMENT,
  `start_t` int(11) NOT NULL,
  `end_t` int(11) NOT NULL,
  `note` char(128) COLLATE latin1_general_cs NOT NULL,
  KEY `num` (`num`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs AUTO_INCREMENT=4 ;

--
-- Dumping data for table `runs`
--

INSERT INTO `runs` (`num`, `start_t`, `end_t`, `note`) VALUES
(1, 1342019232, 0, 'Setup');

-- --------------------------------------------------------

--
-- Table structure for table `sc_insts`
--

CREATE TABLE IF NOT EXISTS `sc_insts` (
  `name` varchar(16) COLLATE latin1_general_cs NOT NULL,
  `description` varchar(64) COLLATE latin1_general_cs DEFAULT NULL,
  `subsys` varchar(16) COLLATE latin1_general_cs NOT NULL,
  `run` tinyint(1) NOT NULL DEFAULT '0',
  `restart` tinyint(1) NOT NULL DEFAULT '0',
  `WD_ctrl` tinyint(1) NOT NULL DEFAULT '1',
  `path` varchar(256) COLLATE latin1_general_cs NOT NULL DEFAULT 'rel_path',
  `dev_type` varchar(16) COLLATE latin1_general_cs DEFAULT NULL,
  `dev_address` varchar(24) COLLATE latin1_general_cs DEFAULT NULL,
  `start_time` int(11) NOT NULL DEFAULT '-1',
  `last_update_time` int(11) NOT NULL DEFAULT '-1',
  `PID` int(11) NOT NULL DEFAULT '-1',
  `user1` varchar(16) COLLATE latin1_general_cs DEFAULT NULL,
  `user2` varchar(16) COLLATE latin1_general_cs DEFAULT NULL,
  `parm1` double NOT NULL DEFAULT '0',
  `parm2` double NOT NULL DEFAULT '0',
  `notes` text COLLATE latin1_general_cs NOT NULL,
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

--
-- Dumping data for table `sc_insts`
--

INSERT INTO `sc_insts` (`name`, `description`, `subsys`, `run`, `restart`, `WD_ctrl`, `path`, `dev_type`, `dev_address`, `start_time`, `last_update_time`, `PID`, `user1`, `user2`, `parm1`, `parm2`, `notes`) VALUES
('alarm_trip_sys', 'Alarm trip monitor program', 'Alarm', 1, 0, 1, 'alarm_trip_system/alarm_trip_sys', 'daemon', '', -1, -1, -1, '', '', 0, 0, ''),
('alarm_alert_sys', 'Alarm alert monitor program', 'Alarm', 0, 0, 1, 'alarm_alert_system/alarm_alert_sys', 'daemon', '', -1, -1, -1, 'Alarm_Light', 'Alarm_Siren', 0, 0, ''),
('Watchdog', 'Overseeing watchdog program', 'Watchdog', 1, 0, 0, '/data/code/SC_backend/slow_control_code/', 'watchdog', '', -1, -1, -1, '', '', 0, 0, 'This should always be running.  If not, you must start it manually using the path above.  The Watchdog will start all other instruments/daemons automatically if their `run` flag is set.  '),
('alarm_siren', 'Light and Siren on top of the HV rack', 'Alarm', 0, 0, 1, 'ADAM6000/ADAM6060', 'modbus', '192.168.91.91', -1, -1, -1, NULL, NULL, 0, 0, ''),
('disk_free', 'Monitor free disk space', 'Sys', 0, 0, 1, 'disk_free/disk_free', 'daemon', NULL, -1, -1, -1, NULL, NULL, 0, 0, '');

-- --------------------------------------------------------

--
-- Table structure for table `sc_sensors`
--

CREATE TABLE IF NOT EXISTS `sc_sensors` (
  `name` varchar(16) COLLATE latin1_general_cs NOT NULL,
  `description` varchar(64) COLLATE latin1_general_cs DEFAULT NULL,
  `type` varchar(16) COLLATE latin1_general_cs NOT NULL DEFAULT 'unknown',
  `subtype` varchar(16) COLLATE latin1_general_cs DEFAULT NULL,
  `ctrl_priv` varchar(128) COLLATE latin1_general_cs NOT NULL DEFAULT 'full',
  `num` int(11) NOT NULL DEFAULT '0',
  `instrument` varchar(16) COLLATE latin1_general_cs NOT NULL DEFAULT 'inst_name',
  `units` varchar(16) COLLATE latin1_general_cs NOT NULL DEFAULT 'units',
  `discrete_vals` varchar(128) COLLATE latin1_general_cs DEFAULT NULL,
  `al_set_val_low` double NOT NULL DEFAULT '0',
  `al_set_val_high` double NOT NULL DEFAULT '0',
  `al_arm_val_low` tinyint(1) NOT NULL DEFAULT '0',
  `al_arm_val_high` tinyint(1) NOT NULL DEFAULT '0',
  `al_set_rate_low` double NOT NULL DEFAULT '0',
  `al_set_rate_high` double NOT NULL DEFAULT '0',
  `al_arm_rate_low` tinyint(1) NOT NULL DEFAULT '0',
  `al_arm_rate_high` tinyint(1) NOT NULL DEFAULT '0',
  `alarm_tripped` tinyint(1) NOT NULL DEFAULT '0',
  `grace` int(11) NOT NULL DEFAULT '0',
  `last_trip` int(11) NOT NULL DEFAULT '-1',
  `settable` tinyint(1) NOT NULL DEFAULT '0',
  `show_rate` tinyint(1) NOT NULL DEFAULT '0',
  `hide_sensor` tinyint(1) NOT NULL DEFAULT '0',
  `update_period` int(11) NOT NULL DEFAULT '60',
  `num_format` varchar(16) COLLATE latin1_general_cs DEFAULT NULL,
  `user1` varchar(16) COLLATE latin1_general_cs DEFAULT NULL,
  `user2` varchar(16) COLLATE latin1_general_cs DEFAULT NULL,
  `user3` varchar(16) COLLATE latin1_general_cs DEFAULT NULL,
  `user4` varchar(16) COLLATE latin1_general_cs DEFAULT NULL,
  `parm1` double NOT NULL DEFAULT '0',
  `parm2` double NOT NULL DEFAULT '0',
  `parm3` double NOT NULL DEFAULT '0',
  `parm4` double NOT NULL DEFAULT '0',
  `notes` text COLLATE latin1_general_cs NOT NULL,
  UNIQUE KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

--
-- Dumping data for table `sc_sensors`
--

INSERT INTO `sc_sensors` (`name`, `description`, `type`, `subtype`, `ctrl_priv`, `num`, `instrument`, `units`, `discrete_vals`, `al_set_val_low`, `al_set_val_high`, `al_arm_val_low`, `al_arm_val_high`, `al_set_rate_low`, `al_set_rate_high`, `al_arm_rate_low`, `al_arm_rate_high`, `alarm_tripped`, `grace`, `last_trip`, `settable`, `show_rate`, `hide_sensor`, `update_period`, `num_format`, `user1`, `user2`, `user3`, `user4`, `parm1`, `parm2`, `parm3`, `parm4`, `notes`) VALUES
('Alarm_Light', 'Alarm light on the HV rack', '5', 'Relay', 'siren', 0, 'alarm_siren', 'discrete', '0:1;Off:On', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 1, 0, 0, 60, NULL, NULL, NULL, NULL, NULL, 1, 0, 0, 0, ''),
('Alarm_Siren', 'Alarm siren on the HV rack', '5', 'Relay', 'siren', 1, 'alarm_siren', 'discrete', '0:1;Off:On', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 1, 0, 0, 60, NULL, NULL, NULL, NULL, NULL, 1, 0, 0, 0, ''),
('dsk_data', 'Free space on /data', '13', 'disk', 'full', 0, 'disk_free', '%', NULL, 95, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 30, NULL, '/data', NULL, NULL, NULL, 0, 0, 0, 0, ''),
('dsk_shm', 'Free space on /dev/shm', '13', 'disk', 'full', 0, 'disk_free', '%', NULL, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 30, NULL, '/dev/shm', NULL, NULL, NULL, 0, 0, 0, 0, ''),
('dsk_root', 'Free space on /', '13', 'disk', 'full', 0, 'disk_free', '%', NULL, 30, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 30, NULL, '/', NULL, NULL, NULL, 0, 0, 0, 0, '');

-- --------------------------------------------------------

--
-- Table structure for table `sc_sensor_types`
--

CREATE TABLE IF NOT EXISTS `sc_sensor_types` (
  `num` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(16) COLLATE utf8_unicode_ci NOT NULL,
  UNIQUE KEY `name` (`name`),
  KEY `num` (`num`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci AUTO_INCREMENT=3 ;

--
-- Dumping data for table `sc_sensor_types`
--

INSERT INTO `sc_sensor_types` (`num`, `name`) VALUES
(1, 'Alarm'),
(2, 'Sys');

-- --------------------------------------------------------

--
-- Table structure for table `sc_sens_Alarm_Light`
--

CREATE TABLE IF NOT EXISTS `sc_sens_Alarm_Light` (
  `time` int(11) NOT NULL,
  `value` double NOT NULL,
  `rate` double NOT NULL,
  KEY `time` (`time`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

-- --------------------------------------------------------

--
-- Table structure for table `sc_sens_Alarm_Siren`
--

CREATE TABLE IF NOT EXISTS `sc_sens_Alarm_Siren` (
  `time` int(11) NOT NULL,
  `value` double NOT NULL,
  `rate` double NOT NULL,
  KEY `time` (`time`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

-- --------------------------------------------------------

--
-- Table structure for table `sc_sens_dsk_data`
--

CREATE TABLE IF NOT EXISTS `sc_sens_dsk_data` (
  `time` int(11) NOT NULL,
  `value` double NOT NULL,
  `rate` double NOT NULL,
  KEY `time` (`time`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

-- --------------------------------------------------------

--
-- Table structure for table `sc_sens_dsk_root`
--

CREATE TABLE IF NOT EXISTS `sc_sens_dsk_root` (
  `time` int(11) NOT NULL,
  `value` double NOT NULL,
  `rate` double NOT NULL,
  KEY `time` (`time`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

-- --------------------------------------------------------

--
-- Table structure for table `sc_sens_dsk_shm`
--

CREATE TABLE IF NOT EXISTS `sc_sens_dsk_shm` (
  `time` int(11) NOT NULL,
  `value` double NOT NULL,
  `rate` double NOT NULL,
  KEY `time` (`time`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `user_name` varchar(32) COLLATE latin1_general_cs NOT NULL,
  `password` char(32) COLLATE latin1_general_cs NOT NULL,
  `full_name` varchar(32) COLLATE latin1_general_cs NOT NULL,
  `affiliation` varchar(64) COLLATE latin1_general_cs NOT NULL,
  `email` varchar(32) COLLATE latin1_general_cs NOT NULL,
  `sms` varchar(32) COLLATE latin1_general_cs NOT NULL,
  `phone` varchar(16) COLLATE latin1_general_cs NOT NULL,
  `on_call` tinyint(1) NOT NULL,
  `shift_status` varchar(16) COLLATE latin1_general_cs NOT NULL,
  `privileges` text COLLATE latin1_general_cs NOT NULL,
  UNIQUE KEY `username` (`user_name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

--
-- Dumping data for table `users`
--

INSERT INTO `users` (`user_name`, `password`, `full_name`, `affiliation`, `email`, `sms`, `phone`, `on_call`, `shift_status`, `privileges`) VALUES
('guest', '084e0343a0486ff05530df6c705c8bb4', 'guest', 'none', '', '', '', 0, 'Off', 'basic,guest'),
('root', '63a9f0ea7bb98050796b649e85481845', 'root', 'root', 'root', 'root', 'root', 0, 'Off', 'admin,basic,config,full,lug,siren');

-- --------------------------------------------------------

--
-- Table structure for table `user_privileges`
--

CREATE TABLE IF NOT EXISTS `user_privileges` (
  `name` varchar(16) COLLATE latin1_general_cs NOT NULL,
  `allowed_host` varchar(16) COLLATE latin1_general_cs NOT NULL DEFAULT 'all',
  KEY `name` (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

--
-- Dumping data for table `user_privileges`
--

INSERT INTO `user_privileges` (`name`, `allowed_host`) VALUES
('guest', 'all'),
('basic', 'all'),
('full', 'all'),
('admin', 'all'),
('config', 'all'),
('lug', 'all'),
('siren', 'all');

-- --------------------------------------------------------

--
-- Table structure for table `user_shift_status`
--

CREATE TABLE IF NOT EXISTS `user_shift_status` (
  `status` varchar(16) COLLATE latin1_general_cs NOT NULL,
  `can_manage` tinyint(1) NOT NULL DEFAULT '0',
  UNIQUE KEY `status` (`status`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COLLATE=latin1_general_cs;

--
-- Dumping data for table `user_shift_status`
--

INSERT INTO `user_shift_status` (`status`, `can_manage`) VALUES
('Off', 0),
('Shift Leader', 1),
('Shift Manager', 1),
('On Shift', 0),
('System Manager', 1);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
