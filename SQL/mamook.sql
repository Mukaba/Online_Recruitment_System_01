-- phpMyAdmin SQL Dump
-- version 2.6.2-pl1
-- http://www.phpmyadmin.net
-- 
-- Host: localhost
-- Generation Time: Sep 02, 2005 at 03:42 PM
-- Server version: 4.1.10
-- 

-- --------------------------------------------------------

-- 
-- Table structure for table `action_methods`
-- 

DROP TABLE IF EXISTS `action_methods`;
CREATE TABLE IF NOT EXISTS `action_methods` (
  `action_method_id` mediumint(8) unsigned NOT NULL auto_increment,
  `action_method_name` varchar(100) default NULL,
  PRIMARY KEY  (`action_method_id`)
) ENGINE = MyISAM;

-- 
-- Dumping data for table `action_methods`
-- 

INSERT INTO `action_methods` VALUES (1, 'In Person');
INSERT INTO `action_methods` VALUES (2, 'Phone');
INSERT INTO `action_methods` VALUES (3, 'E-mail');
INSERT INTO `action_methods` VALUES (4, 'Fax');
INSERT INTO `action_methods` VALUES (5, 'Mail');
INSERT INTO `action_methods` VALUES (6, 'Left Message');
INSERT INTO `action_methods` VALUES (7, 'Other');

-- --------------------------------------------------------

-- 
-- Table structure for table `action_types`
-- 

DROP TABLE IF EXISTS `action_types`;
CREATE TABLE IF NOT EXISTS `action_types` (
  `action_id` tinyint(4) unsigned NOT NULL auto_increment,
  `action_name` varchar(100) default NULL,
  `brief_action_name` varchar(50) default NULL,
  PRIMARY KEY  (`action_id`)
) ENGINE = MyISAM;

-- 
-- Dumping data for table `action_types`
-- 

INSERT INTO `action_types` VALUES (1, 'Sent Info Package', NULL);
INSERT INTO `action_types` VALUES (2, 'Directed to Website', NULL);
INSERT INTO `action_types` VALUES (3, 'Explained Co-op Process', NULL);
INSERT INTO `action_types` VALUES (4, 'Job Development', NULL);
INSERT INTO `action_types` VALUES (5, 'Follow-up', NULL);
INSERT INTO `action_types` VALUES (6, 'Received Reply', NULL);
INSERT INTO `action_types` VALUES (7, 'Sent End of Term Thank You', NULL);
INSERT INTO `action_types` VALUES (8, 'Conducted Worksite Visit', NULL);

-- --------------------------------------------------------

-- 
-- Table structure for table `admin_status`
-- 

DROP TABLE IF EXISTS `admin_status`;
CREATE TABLE IF NOT EXISTS `admin_status` (
  `status_id` tinyint(4) unsigned NOT NULL auto_increment,
  `status_name` varchar(50) default NULL,
  `job_status_id` tinyint(4) unsigned default NULL,
  `order_by` tinyint(4) unsigned default NULL,
  PRIMARY KEY  (`status_id`),
  KEY `job_status_id` (`job_status_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `admin_status`
-- 

INSERT INTO `admin_status` VALUES (1, 'Not Checked - Employer entered job', 1, 10);
INSERT INTO `admin_status` VALUES (2, 'Not Checked - Staff entered job', 1, 20);
INSERT INTO `admin_status` VALUES (3, 'Rejected - Inappropriate job', 6, 30);
INSERT INTO `admin_status` VALUES (4, 'Rejected - Inappropriate employer', 6, 40);
INSERT INTO `admin_status` VALUES (5, 'Rejected - Not a real employer', 6, 50);
INSERT INTO `admin_status` VALUES (6, 'Rejected - Other', 6, 60);
INSERT INTO `admin_status` VALUES (7, 'Checked - Job needs to be coded', 2, 70);
INSERT INTO `admin_status` VALUES (8, 'Checked - Ready to post but awaiting date', 2, 80);
INSERT INTO `admin_status` VALUES (9, 'Checked - Ready to post but awaiting confirmation', 2, 90);
INSERT INTO `admin_status` VALUES (10, 'Cancelled - Job deleted', 5, 100);
INSERT INTO `admin_status` VALUES (11, 'Posted - Job is student viewable', 3, 110);
INSERT INTO `admin_status` VALUES (12, 'Closed', 4, 120);
INSERT INTO `admin_status` VALUES (13, 'Not Interviewing - No suitable applicants', 4, 140);
INSERT INTO `admin_status` VALUES (14, 'Not Interviewing - No funding', 4, 150);
INSERT INTO `admin_status` VALUES (15, 'Not Interviewing - Other', 4, 160);
INSERT INTO `admin_status` VALUES (16, 'Interviewing', 4, 170);
INSERT INTO `admin_status` VALUES (17, 'Waiting for Ranking', 4, 180);
INSERT INTO `admin_status` VALUES (18, 'Rankings Received', 4, 190);
INSERT INTO `admin_status` VALUES (19, 'Filled - Here', 4, 200);
INSERT INTO `admin_status` VALUES (20, 'Filled - Elsewhere', 4, 210);
INSERT INTO `admin_status` VALUES (21, 'Unfilled - No suitable applicants', 4, 220);
INSERT INTO `admin_status` VALUES (22, 'Unfilled - Employer not hiring', 4, 230);
INSERT INTO `admin_status` VALUES (23, 'Hiring on hold', 4, 240);
INSERT INTO `admin_status` VALUES (25, 'Posted - 24 Hour Hold', 8, 115);

-- --------------------------------------------------------

-- 
-- Table structure for table `application_status`
-- 

DROP TABLE IF EXISTS `application_status`;
CREATE TABLE IF NOT EXISTS `application_status` (
  `app_status_id` tinyint(3) unsigned NOT NULL default '0',
  `app_status_description` text,
  PRIMARY KEY  (`app_status_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `application_status`
-- 

INSERT INTO `application_status` VALUES (0, 'Applied Online');
INSERT INTO `application_status` VALUES (1, 'Activated Online');
INSERT INTO `application_status` VALUES (2, 'Admin Deleted');
INSERT INTO `application_status` VALUES (3, 'Student Deleted');
INSERT INTO `application_status` VALUES (4, 'Unavailable');
INSERT INTO `application_status` VALUES (5, 'Paper Applied');
INSERT INTO `application_status` VALUES (6, 'Paper Sent');
INSERT INTO `application_status` VALUES (7, 'Special - Signed up/Applied');

-- --------------------------------------------------------

-- 
-- Table structure for table `applications`
-- 

DROP TABLE IF EXISTS `applications`;
CREATE TABLE IF NOT EXISTS `applications` (
  `application_id` mediumint(8) unsigned NOT NULL auto_increment,
  `resume_id` mediumint(8) unsigned NOT NULL default '0',
  `student_number` varchar(10) default NULL,
  `job_id` mediumint(8) unsigned default NULL,
  `transcript` tinyint(3) unsigned default '0',
  `applied_date` datetime default NULL,
  `coverletter_id` mediumint(8) unsigned default NULL,
  `app_status` tinyint(3) unsigned default '0',
  `added_by_admin` tinyint(1) default '0',
  PRIMARY KEY  (`application_id`),
  KEY `job_id` (`job_id`),
  KEY `app_status` (`app_status`),
  KEY `student_number` (`student_number`),
  KEY `coverletter_id` (`coverletter_id`),
  KEY `resume_id` (`resume_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `applications`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `area_list`
-- 

DROP TABLE IF EXISTS `area_list`;
CREATE TABLE IF NOT EXISTS `area_list` (
  `area_id` mediumint(8) unsigned NOT NULL auto_increment,
  `area_name` varchar(50) default NULL,
  PRIMARY KEY  (`area_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `area_list`
-- 

INSERT INTO `area_list` VALUES (1, 'Canada');
INSERT INTO `area_list` VALUES (2, 'United States');
INSERT INTO `area_list` VALUES (3, 'Central America');
INSERT INTO `area_list` VALUES (4, 'South America');
INSERT INTO `area_list` VALUES (5, 'Africa');
INSERT INTO `area_list` VALUES (6, 'Western Europe');
INSERT INTO `area_list` VALUES (7, 'Eastern Europe');
INSERT INTO `area_list` VALUES (8, 'Asia');
INSERT INTO `area_list` VALUES (9, 'Australia and South Pacific');

-- --------------------------------------------------------

-- 
-- Table structure for table `broken_queries`
-- 

DROP TABLE IF EXISTS `broken_queries`;
CREATE TABLE IF NOT EXISTS `broken_queries` (
  `query_id` smallint(6) NOT NULL auto_increment,
  `query` text,
  `date_time` datetime default NULL,
  `contact_id` bigint(20) unsigned default NULL,
  `student_number` varchar(10) default NULL,
  `IP_address` varchar(50) default NULL,
  `counter` mediumint(8) unsigned default '0',
  `file_name` text,
  `line_number` smallint(6) default NULL,
  PRIMARY KEY  (`query_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `broken_queries`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `building`
-- 

DROP TABLE IF EXISTS `building`;
CREATE TABLE IF NOT EXISTS `building` (
  `building_id` smallint(5) unsigned NOT NULL auto_increment,
  `building_name` varchar(50) NOT NULL default '',
  `building_code` varchar(10) NOT NULL default '',
  PRIMARY KEY  (`building_id`),
  UNIQUE KEY `building_name_index` (`building_name`),
  UNIQUE KEY `building_code_index` (`building_code`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `building`
-- 

INSERT INTO `building` VALUES (1, 'Engineering Lab Wing', 'ELW');
INSERT INTO `building` VALUES (2, 'Engineering Office Wing', 'EOW');

-- --------------------------------------------------------

-- 
-- Table structure for table `company_flags`
-- 

DROP TABLE IF EXISTS `company_flags`;
CREATE TABLE IF NOT EXISTS `company_flags` (
  `flag_name` varchar(50) default NULL,
  `flag_id` mediumint(8) unsigned NOT NULL auto_increment,
  `comment` tinytext,
  PRIMARY KEY  (`flag_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `company_flags`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `company_flags_set`
-- 

DROP TABLE IF EXISTS `company_flags_set`;
CREATE TABLE IF NOT EXISTS `company_flags_set` (
  `employer_id` mediumint(8) unsigned default NULL,
  `flag_id` mediumint(8) unsigned default NULL,
  KEY `employer_id` (`employer_id`),
  KEY `flag_id` (`flag_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `company_flags_set`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `company_notes`
-- 

DROP TABLE IF EXISTS `company_notes`;
CREATE TABLE IF NOT EXISTS `company_notes` (
  `employer_id` mediumint(8) unsigned default NULL,
  `notes` text,
  `date_entered` datetime default NULL,
  `entered_by` bigint(20) unsigned default NULL,
  `note_id` mediumint(8) unsigned NOT NULL auto_increment,
  PRIMARY KEY  (`note_id`),
  KEY `employer_id` (`employer_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `company_notes`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `company_type`
-- 

DROP TABLE IF EXISTS `company_type`;
CREATE TABLE IF NOT EXISTS `company_type` (
  `type_id` tinyint(4) unsigned NOT NULL auto_increment,
  `type_code` varchar(25) default NULL,
  `type_name` varchar(50) default NULL,
  PRIMARY KEY  (`type_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `company_type`
-- 

INSERT INTO `company_type` VALUES (1, 'FG', 'Federal Government');
INSERT INTO `company_type` VALUES (2, 'PG', 'Provincial Government');
INSERT INTO `company_type` VALUES (3, 'MG', 'Municipal Government');
INSERT INTO `company_type` VALUES (4, 'PB', 'Private Business');
INSERT INTO `company_type` VALUES (5, 'FA', 'Federal Agency');
INSERT INTO `company_type` VALUES (6, 'PA', 'Provincial Agency');
INSERT INTO `company_type` VALUES (7, 'NPO', 'Non Profit Organization');

-- --------------------------------------------------------

-- 
-- Table structure for table `contact`
-- 

DROP TABLE IF EXISTS `contact`;
CREATE TABLE IF NOT EXISTS `contact` (
  `contact_id` bigint(20) unsigned NOT NULL auto_increment,
  `type` enum('internal','employer','potential_employer','alumni','faculty') default 'employer',
  `title` varchar(10) default NULL,
  `first_name` varchar(40) NOT NULL default '',
  `last_name` varchar(40) NOT NULL default '',
  `called_name` varchar(40) default NULL,
  `middle_name` varchar(40) default NULL,
  `greeting` varchar(75) default NULL,
  `email` varchar(100) default NULL,
  `phone` varchar(25) default NULL,
  `pager` varchar(25) default NULL,
  `cellphone` varchar(25) default NULL,
  `fax` varchar(25) default NULL,
  `street_address_1` varchar(75) default NULL,
  `street_address_2` varchar(75) default NULL,
  `street_address_3` varchar(75) default NULL,
  `city` varchar(40) default NULL,
  `provstate_id` mediumint(8) unsigned NOT NULL default '0',
  `country_id` mediumint(8) unsigned NOT NULL default '0',
  `postal_code` varchar(10) default NULL,
  `region_id` mediumint(8) unsigned NOT NULL default '0',
  `location_info` varchar(25) NOT NULL default '0',
  PRIMARY KEY  (`contact_id`),
  KEY `first_name` (`first_name`),
  KEY `last_name` (`last_name`),
  KEY `phone` (`phone`),
  KEY `provstate_id` (`provstate_id`),
  KEY `country_id` (`country_id`),
  KEY `region_id` (`region_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `contact`
-- 

INSERT INTO `contact` VALUES (1, 'internal', NULL, 'Internal', 'Contact', NULL, NULL, NULL, 'intcont@email.ca', '', NULL, NULL, '', NULL, NULL, NULL, NULL, 0, 0, NULL, 0, '');

-- --------------------------------------------------------

-- 
-- Table structure for table `contact_actions_set`
-- 

DROP TABLE IF EXISTS `contact_actions_set`;
CREATE TABLE IF NOT EXISTS `contact_actions_set` (
  `action_id` tinyint(4) unsigned default NULL,
  `contact_id` bigint(20) unsigned default NULL,
  `action_by` bigint(20) unsigned default NULL,
  `action_on` date default NULL,
  `action_method_id` mediumint(8) unsigned NOT NULL default '0',
  `history_id` int(11) default NULL,
  `action_note` text NOT NULL,
  `bring_forward_date` date default '0000-00-00',
  `contact_actions_id` bigint(20) unsigned NOT NULL auto_increment,
  `multiple_bring_forward_date` mediumint(8) unsigned default NULL,
  PRIMARY KEY  (`contact_actions_id`),
  KEY `action_id` (`action_id`),
  KEY `contact_id` (`contact_id`),
  KEY `action_by` (`action_by`),
  KEY `action_on` (`action_on`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `contact_actions_set`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `contact_employer`
-- 

DROP TABLE IF EXISTS `contact_employer`;
CREATE TABLE IF NOT EXISTS `contact_employer` (
  `contact_id` bigint(20) unsigned NOT NULL default '0',
  `employer_id` mediumint(8) unsigned default NULL,
  `department_id` mediumint(8) unsigned NOT NULL default '0',
  `employer_type` smallint(5) unsigned default '1',
  `general_comments` text,
  `changes_recorded_1` text,
  `changes_recorded_2` text,
  `changes_recorded_3` text,
  `change_by_1` bigint(20) unsigned default NULL,
  `change_by_2` bigint(20) unsigned default NULL,
  `change_by_3` bigint(20) unsigned default NULL,
  `change_date_1` date default '0000-00-00',
  `change_date_2` date default '0000-00-00',
  `change_date_3` date default '0000-00-00',
  `entered_on` date default NULL,
  `entered_by` varchar(40) default NULL,
  `position` varchar(100) default NULL,
  `staff_mem` bigint(20) unsigned default NULL,
  `department_name` varchar(150) default NULL,
  `deleted_flag` tinyint(1) unsigned default '0',
  PRIMARY KEY  (`contact_id`),
  KEY `employer_id` (`employer_id`),
  KEY `department_id` (`department_id`),
  KEY `employer_type` (`employer_type`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `contact_employer`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `contact_flags`
-- 

DROP TABLE IF EXISTS `contact_flags`;
CREATE TABLE IF NOT EXISTS `contact_flags` (
  `flag_name` varchar(50) default NULL,
  `flag_id` mediumint(8) unsigned NOT NULL auto_increment,
  `comment` tinytext,
  PRIMARY KEY  (`flag_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `contact_flags`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `contact_flags_set`
-- 

DROP TABLE IF EXISTS `contact_flags_set`;
CREATE TABLE IF NOT EXISTS `contact_flags_set` (
  `contact_id` bigint(20) unsigned default NULL,
  `flag_id` mediumint(8) unsigned default NULL,
  KEY `contact_id` (`contact_id`),
  KEY `flag_id` (`flag_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `contact_flags_set`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `contact_internal`
-- 

DROP TABLE IF EXISTS `contact_internal`;
CREATE TABLE IF NOT EXISTS `contact_internal` (
  `contact_id` bigint(20) unsigned NOT NULL default '0',
  `department_id` smallint(5) unsigned NOT NULL default '0',
  `interview_contact_list` tinyint(3) unsigned NOT NULL default '0',
  `login_id` varchar(8) NOT NULL default '*',
  `password` varchar(10) default NULL,
  `employee_number` varchar(8) default NULL,
  `netlink_id` varchar(10) default '*',
  `interview_lunch_list` tinyint(3) unsigned NOT NULL default '1',
  `pulldown_menu_group` tinyint(3) unsigned NOT NULL default '0',
  `search_use_advanced` tinyint(3) unsigned NOT NULL default '0',
  `site_visit_list` tinyint(3) unsigned NOT NULL default '0',
  `search_use_advanced_emp` tinyint(3) unsigned NOT NULL default '0',
  `search_use_advanced_hist` tinyint(3) unsigned NOT NULL default '0',
  `menu_use_javascript` tinyint(3) unsigned NOT NULL default '0',
  `application_replyto_list` tinyint(3) unsigned NOT NULL default '0',
  `flash_use` tinyint(3) unsigned NOT NULL default '1',
  `general_email` tinyint(3) unsigned NOT NULL default '1',
  `traffic_director_email` tinyint(3) unsigned NOT NULL default '0',
  `email_signature` text,
  `show_closed_cancelled` tinyint(3) unsigned NOT NULL default '0',
  `view_sent_apps` tinyint(3) unsigned NOT NULL default '0',
  `send_sent_apps` tinyint(3) unsigned NOT NULL default '0',
  `employer_info_menu` tinyint(3) unsigned NOT NULL default '0',
  `coop_advisor_list` tinyint(3) unsigned NOT NULL default '0',
  `user_font_size` varchar(32) default NULL,
  PRIMARY KEY  (`contact_id`),
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `contact_internal`
-- 

INSERT INTO `contact_internal` VALUES (1, 1, 1, '*', NULL, '0', '*', 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, '', 0, 0, 0, 0, 0, NULL);

-- --------------------------------------------------------

-- 
-- Table structure for table `contact_job`
-- 

DROP TABLE IF EXISTS `contact_job`;
CREATE TABLE IF NOT EXISTS `contact_job` (
  `contact_id` bigint(20) unsigned default NULL,
  `job_id` mediumint(8) unsigned default NULL,
  KEY `contact_id` (`contact_id`),
  KEY `job_id` (`job_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `contact_job`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `contact_notes`
-- 

DROP TABLE IF EXISTS `contact_notes`;
CREATE TABLE IF NOT EXISTS `contact_notes` (
  `contact_id` bigint(20) unsigned default NULL,
  `notes` text,
  `date_entered` datetime default NULL,
  `entered_by` bigint(20) unsigned default NULL,
  `note_id` mediumint(8) unsigned NOT NULL auto_increment,
  PRIMARY KEY  (`note_id`),
  KEY `contact_id` (`contact_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `contact_notes`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `convocation_month`
-- 

DROP TABLE IF EXISTS `convocation_month`;
CREATE TABLE IF NOT EXISTS `convocation_month` (
  `month` tinyint(3) unsigned NOT NULL default '0',
  PRIMARY KEY  (`month`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `convocation_month`
-- 

INSERT INTO `convocation_month` VALUES (5);
INSERT INTO `convocation_month` VALUES (11);

-- --------------------------------------------------------

-- 
-- Table structure for table `country_list`
-- 

DROP TABLE IF EXISTS `country_list`;
CREATE TABLE IF NOT EXISTS `country_list` (
  `country_id` mediumint(8) unsigned NOT NULL auto_increment,
  `country_name` varchar(75) default NULL,
  `area_id` mediumint(8) unsigned NOT NULL default '0',
  `order_by` mediumint(8) unsigned default NULL,
  PRIMARY KEY  (`country_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `country_list`
-- 

INSERT INTO `country_list` VALUES (1, 'Canada', 1, 10);
INSERT INTO `country_list` VALUES (2, 'United States of America', 2, 20);
INSERT INTO `country_list` VALUES (3, 'Afghanistan', 8, 30);
INSERT INTO `country_list` VALUES (4, 'Albania', 7, 40);
INSERT INTO `country_list` VALUES (5, 'Algeria', 5, 50);
INSERT INTO `country_list` VALUES (6, 'American Samoa', 3, 60);
INSERT INTO `country_list` VALUES (7, 'Andorra', 6, 70);
INSERT INTO `country_list` VALUES (8, 'Angola', 5, 80);
INSERT INTO `country_list` VALUES (9, 'Anguilla', 3, 90);
INSERT INTO `country_list` VALUES (10, 'Antigua and Barbuda', 3, 100);
INSERT INTO `country_list` VALUES (11, 'Argentina', 4, 110);
INSERT INTO `country_list` VALUES (12, 'Armenia', 8, 120);
INSERT INTO `country_list` VALUES (13, 'Aruba', 4, 130);
INSERT INTO `country_list` VALUES (14, 'Australia', 9, 140);
INSERT INTO `country_list` VALUES (15, 'Austria', 6, 150);
INSERT INTO `country_list` VALUES (16, 'Azerbaijan', 8, 160);
INSERT INTO `country_list` VALUES (17, 'Bahamas', 3, 170);
INSERT INTO `country_list` VALUES (18, 'Bahrain', 8, 180);
INSERT INTO `country_list` VALUES (19, 'Bangladesh', 8, 190);
INSERT INTO `country_list` VALUES (20, 'Barbados', 3, 200);
INSERT INTO `country_list` VALUES (21, 'Belarus', 7, 210);
INSERT INTO `country_list` VALUES (22, 'Belgium', 6, 220);
INSERT INTO `country_list` VALUES (23, 'Belize', 3, 230);
INSERT INTO `country_list` VALUES (24, 'Benin', 5, 240);
INSERT INTO `country_list` VALUES (25, 'Bermuda', 3, 250);
INSERT INTO `country_list` VALUES (26, 'Bhutan', 8, 260);
INSERT INTO `country_list` VALUES (27, 'Bolivia (Republic of)', 4, 270);
INSERT INTO `country_list` VALUES (28, 'Bosnia and Herzegovina', 7, 280);
INSERT INTO `country_list` VALUES (29, 'Botswana', 5, 290);
INSERT INTO `country_list` VALUES (30, 'Brazil', 4, 300);
INSERT INTO `country_list` VALUES (31, 'British Virgin Islands', 3, 310);
INSERT INTO `country_list` VALUES (32, 'Brunei Darussalam', 8, 320);
INSERT INTO `country_list` VALUES (33, 'Bulgaria (Republic of)', 7, 330);
INSERT INTO `country_list` VALUES (34, 'Burkina Faso', 5, 340);
INSERT INTO `country_list` VALUES (36, 'Burundi', 5, 360);
INSERT INTO `country_list` VALUES (37, 'Cambodia', 8, 370);
INSERT INTO `country_list` VALUES (38, 'Cameroon', 5, 380);
INSERT INTO `country_list` VALUES (39, 'Cape Verde', 5, 390);
INSERT INTO `country_list` VALUES (40, 'Cayman Islands', 3, 400);
INSERT INTO `country_list` VALUES (41, 'Central African Republic', 5, 410);
INSERT INTO `country_list` VALUES (42, 'Chad', 5, 420);
INSERT INTO `country_list` VALUES (43, 'Chile', 4, 430);
INSERT INTO `country_list` VALUES (44, 'China (People''s Republic of)', 8, 440);
INSERT INTO `country_list` VALUES (45, 'Colombia', 4, 450);
INSERT INTO `country_list` VALUES (46, 'Comoros', 5, 460);
INSERT INTO `country_list` VALUES (47, 'Congo (Democratic Republic of)', 5, 470);
INSERT INTO `country_list` VALUES (48, 'Cook Islands', 9, 480);
INSERT INTO `country_list` VALUES (49, 'Costa Rica', 3, 490);
INSERT INTO `country_list` VALUES (50, 'Cote d''Ivoire (Republic of)', 5, 500);
INSERT INTO `country_list` VALUES (51, 'Croatia', 7, 510);
INSERT INTO `country_list` VALUES (52, 'Cuba (Republic of)', 3, 520);
INSERT INTO `country_list` VALUES (53, 'Cyprus', 8, 530);
INSERT INTO `country_list` VALUES (54, 'Czech Republic', 7, 540);
INSERT INTO `country_list` VALUES (55, 'Denmark', 6, 550);
INSERT INTO `country_list` VALUES (56, 'Djibouti', 5, 560);
INSERT INTO `country_list` VALUES (57, 'Dominica', 3, 570);
INSERT INTO `country_list` VALUES (58, 'Dominican Republic', 3, 580);
INSERT INTO `country_list` VALUES (59, 'Ecuador', 4, 590);
INSERT INTO `country_list` VALUES (60, 'Egypt', 5, 600);
INSERT INTO `country_list` VALUES (61, 'El Salvador', 3, 610);
INSERT INTO `country_list` VALUES (62, 'Equatorial Guinea', 5, 620);
INSERT INTO `country_list` VALUES (63, 'Eritrea', 5, 630);
INSERT INTO `country_list` VALUES (64, 'Estonia', 7, 640);
INSERT INTO `country_list` VALUES (65, 'Ethiopia', 5, 650);
INSERT INTO `country_list` VALUES (66, 'Falkland Islands (Malvinas)', 6, 660);
INSERT INTO `country_list` VALUES (67, 'Fiji', 9, 670);
INSERT INTO `country_list` VALUES (68, 'Finland', 6, 680);
INSERT INTO `country_list` VALUES (69, 'France', 6, 690);
INSERT INTO `country_list` VALUES (70, 'French Guiana', 4, 700);
INSERT INTO `country_list` VALUES (71, 'French Polynesia', 9, 710);
INSERT INTO `country_list` VALUES (72, 'Gabon', 5, 720);
INSERT INTO `country_list` VALUES (73, 'Gambia', 5, 730);
INSERT INTO `country_list` VALUES (74, 'Georgia', 8, 740);
INSERT INTO `country_list` VALUES (75, 'Germany', 6, 750);
INSERT INTO `country_list` VALUES (76, 'Ghana (Republic of)', 5, 760);
INSERT INTO `country_list` VALUES (77, 'Greece', 6, 770);
INSERT INTO `country_list` VALUES (78, 'Greenland', 6, 780);
INSERT INTO `country_list` VALUES (79, 'Grenada', 3, 790);
INSERT INTO `country_list` VALUES (80, 'Guadeloupe', 3, 800);
INSERT INTO `country_list` VALUES (81, 'Guam', 3, 810);
INSERT INTO `country_list` VALUES (82, 'Guatemala', 3, 820);
INSERT INTO `country_list` VALUES (83, 'Guernsey', 6, 830);
INSERT INTO `country_list` VALUES (84, 'Guinea', 5, 840);
INSERT INTO `country_list` VALUES (85, 'Guinea-Bissau', 5, 850);
INSERT INTO `country_list` VALUES (86, 'Guyana', 4, 860);
INSERT INTO `country_list` VALUES (87, 'Haiti', 3, 870);
INSERT INTO `country_list` VALUES (88, 'Honduras (Republic of)', 3, 880);
INSERT INTO `country_list` VALUES (89, 'Hong Kong', 8, 890);
INSERT INTO `country_list` VALUES (90, 'Hungary (Republic of)', 7, 900);
INSERT INTO `country_list` VALUES (91, 'Iceland', 6, 910);
INSERT INTO `country_list` VALUES (92, 'India', 8, 920);
INSERT INTO `country_list` VALUES (93, 'Indonesia', 8, 930);
INSERT INTO `country_list` VALUES (94, 'Iran (Islamic Republic of)', 8, 940);
INSERT INTO `country_list` VALUES (95, 'Iraq', 8, 950);
INSERT INTO `country_list` VALUES (96, 'Ireland', 6, 960);
INSERT INTO `country_list` VALUES (97, 'Israel', 8, 970);
INSERT INTO `country_list` VALUES (98, 'Italy', 6, 980);
INSERT INTO `country_list` VALUES (99, 'Jamaica', 3, 990);
INSERT INTO `country_list` VALUES (100, 'Japan', 8, 1000);
INSERT INTO `country_list` VALUES (101, 'Jersey', 6, 1010);
INSERT INTO `country_list` VALUES (102, 'Jordan', 8, 1020);
INSERT INTO `country_list` VALUES (103, 'Kazakhstan', 8, 1030);
INSERT INTO `country_list` VALUES (104, 'Kenya', 5, 1040);
INSERT INTO `country_list` VALUES (105, 'Kiribati', 9, 1050);
INSERT INTO `country_list` VALUES (106, 'North Korea (Democratic People''s Republic of Korea)', 8, 1465);
INSERT INTO `country_list` VALUES (107, 'South Korea (Republic of)', 8, 1785);
INSERT INTO `country_list` VALUES (108, 'Kuwait', 8, 1080);
INSERT INTO `country_list` VALUES (109, 'Kyrgyzstan', 8, 1090);
INSERT INTO `country_list` VALUES (110, 'Lao People''s Democratic Republic', 8, 1100);
INSERT INTO `country_list` VALUES (111, 'Latvia', 7, 1110);
INSERT INTO `country_list` VALUES (112, 'Lebanon', 8, 1120);
INSERT INTO `country_list` VALUES (113, 'Liechtenstein', 6, 1130);
INSERT INTO `country_list` VALUES (114, 'Lithuania', 7, 1140);
INSERT INTO `country_list` VALUES (115, 'Luxembourg', 6, 1150);
INSERT INTO `country_list` VALUES (116, 'Macao', 4, 1160);
INSERT INTO `country_list` VALUES (117, 'Macedonia (Former Yugoslav Republic of)', 7, 1170);
INSERT INTO `country_list` VALUES (118, 'Madagascar', 5, 1180);
INSERT INTO `country_list` VALUES (119, 'Malawi', 5, 1190);
INSERT INTO `country_list` VALUES (120, 'Malaysia', 8, 1200);
INSERT INTO `country_list` VALUES (121, 'Maldives', 8, 1210);
INSERT INTO `country_list` VALUES (122, 'Mali', 5, 1220);
INSERT INTO `country_list` VALUES (123, 'Malta', 6, 1230);
INSERT INTO `country_list` VALUES (124, 'Marshall Islands', 9, 1240);
INSERT INTO `country_list` VALUES (125, 'Martinique', 3, 1250);
INSERT INTO `country_list` VALUES (126, 'Mauritania', 5, 1260);
INSERT INTO `country_list` VALUES (127, 'Mauritius', 5, 1270);
INSERT INTO `country_list` VALUES (128, 'Mayotte', 5, 1280);
INSERT INTO `country_list` VALUES (129, 'Mexico', 3, 1290);
INSERT INTO `country_list` VALUES (130, 'Moldova', 7, 1300);
INSERT INTO `country_list` VALUES (131, 'Monaco', 6, 1310);
INSERT INTO `country_list` VALUES (132, 'Mongolia', 8, 1320);
INSERT INTO `country_list` VALUES (133, 'Montserrat', 3, 1330);
INSERT INTO `country_list` VALUES (134, 'Morocco', 5, 1340);
INSERT INTO `country_list` VALUES (135, 'Mozambique', 5, 1350);
INSERT INTO `country_list` VALUES (136, 'Namibia', 5, 1360);
INSERT INTO `country_list` VALUES (137, 'Nauru', 9, 1370);
INSERT INTO `country_list` VALUES (138, 'Nepal', 8, 1380);
INSERT INTO `country_list` VALUES (139, 'Netherlands', 6, 1390);
INSERT INTO `country_list` VALUES (140, 'Netherlands Antilles', 6, 1400);
INSERT INTO `country_list` VALUES (141, 'New Caledonia', 9, 1410);
INSERT INTO `country_list` VALUES (142, 'New Zealand', 9, 1420);
INSERT INTO `country_list` VALUES (143, 'Nicaragua', 3, 1430);
INSERT INTO `country_list` VALUES (144, 'Niger', 5, 1440);
INSERT INTO `country_list` VALUES (145, 'Nigeria', 5, 1450);
INSERT INTO `country_list` VALUES (146, 'Niue Island', 9, 1460);
INSERT INTO `country_list` VALUES (147, 'Norway', 6, 1470);
INSERT INTO `country_list` VALUES (148, 'Oman', 8, 1480);
INSERT INTO `country_list` VALUES (149, 'Pakistan', 8, 1490);
INSERT INTO `country_list` VALUES (150, 'Palau', 9, 1500);
INSERT INTO `country_list` VALUES (151, 'Panama (Republic of)', 3, 1510);
INSERT INTO `country_list` VALUES (152, 'Papua New Guinea', 9, 1520);
INSERT INTO `country_list` VALUES (153, 'Paraguay', 4, 1530);
INSERT INTO `country_list` VALUES (154, 'Peru', 4, 1540);
INSERT INTO `country_list` VALUES (155, 'Philippines', 8, 1550);
INSERT INTO `country_list` VALUES (156, 'Poland (Republic of)', 7, 1560);
INSERT INTO `country_list` VALUES (157, 'Portugal', 6, 1570);
INSERT INTO `country_list` VALUES (158, 'Puerto Rico', 3, 1580);
INSERT INTO `country_list` VALUES (159, 'Qatar', 8, 1590);
INSERT INTO `country_list` VALUES (160, 'Romania', 7, 1600);
INSERT INTO `country_list` VALUES (161, 'Russian Federation', 7, 1610);
INSERT INTO `country_list` VALUES (162, 'Rwanda', 5, 1620);
INSERT INTO `country_list` VALUES (163, 'St. Helena', 5, 1803);
INSERT INTO `country_list` VALUES (164, 'Saint Christopher (St. Kitts) and Nevis', 3, 1640);
INSERT INTO `country_list` VALUES (165, 'Saint Lucia', 3, 1650);
INSERT INTO `country_list` VALUES (166, 'St. Pierre and Miquelon', 6, 1806);
INSERT INTO `country_list` VALUES (167, 'Saint Vincent and the Grenadines', 3, 1670);
INSERT INTO `country_list` VALUES (169, 'Saudi Arabia', 8, 1690);
INSERT INTO `country_list` VALUES (170, 'Senegal', 5, 1700);
INSERT INTO `country_list` VALUES (171, 'Seychelles', 5, 1710);
INSERT INTO `country_list` VALUES (172, 'Sierra Leone', 5, 1720);
INSERT INTO `country_list` VALUES (173, 'Singapore', 8, 1730);
INSERT INTO `country_list` VALUES (174, 'Slovakia', 7, 1740);
INSERT INTO `country_list` VALUES (175, 'Slovenia', 7, 1750);
INSERT INTO `country_list` VALUES (176, 'Solomon Islands', 9, 1760);
INSERT INTO `country_list` VALUES (177, 'Somalia', 5, 1770);
INSERT INTO `country_list` VALUES (178, 'South Africa', 5, 1780);
INSERT INTO `country_list` VALUES (179, 'Spain', 6, 1790);
INSERT INTO `country_list` VALUES (180, 'Sri Lanka', 8, 1800);
INSERT INTO `country_list` VALUES (181, 'Sudan', 5, 1810);
INSERT INTO `country_list` VALUES (182, 'Suriname', 4, 1820);
INSERT INTO `country_list` VALUES (184, 'Swaziland', 5, 1840);
INSERT INTO `country_list` VALUES (185, 'Sweden', 6, 1850);
INSERT INTO `country_list` VALUES (186, 'Switzerland', 6, 1860);
INSERT INTO `country_list` VALUES (187, 'Syrian Arab Republic', 8, 1870);
INSERT INTO `country_list` VALUES (188, 'Tahiti', 9, 1880);
INSERT INTO `country_list` VALUES (189, 'Taiwan', 8, 1890);
INSERT INTO `country_list` VALUES (190, 'Tajikistan', 8, 1900);
INSERT INTO `country_list` VALUES (191, 'Tanzania (United Republic of)', 5, 1910);
INSERT INTO `country_list` VALUES (192, 'Thailand', 8, 1920);
INSERT INTO `country_list` VALUES (193, 'Togo', 5, 1930);
INSERT INTO `country_list` VALUES (194, 'Tonga', 9, 1940);
INSERT INTO `country_list` VALUES (195, 'Trinidad and Tobago', 3, 1950);
INSERT INTO `country_list` VALUES (196, 'Tunisia', 5, 1960);
INSERT INTO `country_list` VALUES (197, 'Turkey', 8, 1970);
INSERT INTO `country_list` VALUES (198, 'Turkmenistan', 8, 1980);
INSERT INTO `country_list` VALUES (199, 'Turks and Caicos Islands', 3, 1990);
INSERT INTO `country_list` VALUES (200, 'Tuvalu', 9, 2000);
INSERT INTO `country_list` VALUES (201, 'Uganda', 5, 2010);
INSERT INTO `country_list` VALUES (202, 'Ukraine', 7, 2020);
INSERT INTO `country_list` VALUES (203, 'United Arab Emirates', 8, 2030);
INSERT INTO `country_list` VALUES (205, 'Uruguay', 4, 2050);
INSERT INTO `country_list` VALUES (206, 'Uzbekistan', 8, 2060);
INSERT INTO `country_list` VALUES (207, 'Vanuatu', 9, 2070);
INSERT INTO `country_list` VALUES (208, 'Vatican', 6, 2080);
INSERT INTO `country_list` VALUES (209, 'Venezuela', 4, 2090);
INSERT INTO `country_list` VALUES (210, 'Viet Nam', 8, 2100);
INSERT INTO `country_list` VALUES (211, 'Virgin Islands (U.S.A.)', 3, 2110);
INSERT INTO `country_list` VALUES (212, 'Wallis and Futuna Islands', 9, 2120);
INSERT INTO `country_list` VALUES (215, 'Yemen', 8, 2150);
INSERT INTO `country_list` VALUES (216, 'Yugoslavia', 7, 2160);
INSERT INTO `country_list` VALUES (217, 'Zambia', 5, 2170);
INSERT INTO `country_list` VALUES (218, 'Zimbabwe', 5, 2180);
INSERT INTO `country_list` VALUES (219, 'Ascension', 5, 135);
INSERT INTO `country_list` VALUES (220, 'Congo (Republic of)', 5, 475);
INSERT INTO `country_list` VALUES (221, 'East Timor', 8, 585);
INSERT INTO `country_list` VALUES (222, 'England', 6, 615);
INSERT INTO `country_list` VALUES (223, 'Faroe Islands', 6, 665);
INSERT INTO `country_list` VALUES (224, 'Gibraltar', 6, 765);
INSERT INTO `country_list` VALUES (225, 'Lesotho', 5, 1122);
INSERT INTO `country_list` VALUES (226, 'Liberia', 5, 1125);
INSERT INTO `country_list` VALUES (227, 'Libyan Arab Jamahiriya (Socialist People''s)', 5, 1127);
INSERT INTO `country_list` VALUES (228, 'Mariana Islands', 2, 1235);
INSERT INTO `country_list` VALUES (229, 'Minor Outlying Islands (U.S.A.)', 2, 1295);
INSERT INTO `country_list` VALUES (230, 'Micronesia (Federated States)', 9, 1292);
INSERT INTO `country_list` VALUES (231, 'Myanmar', 8, 1355);
INSERT INTO `country_list` VALUES (232, 'Northern Ireland', 6, 1425);
INSERT INTO `country_list` VALUES (233, 'Pitcairn Islands', 9, 1555);
INSERT INTO `country_list` VALUES (234, 'Reunion', 5, 1595);
INSERT INTO `country_list` VALUES (235, 'Samoa', 9, 1675);
INSERT INTO `country_list` VALUES (236, 'Sao Tome and Principe', 5, 1677);
INSERT INTO `country_list` VALUES (237, 'Scotland', 6, 1695);
INSERT INTO `country_list` VALUES (238, 'Tokelau Islands', 9, 1935);
INSERT INTO `country_list` VALUES (239, 'Tristan da Cunha', 5, 1955);
INSERT INTO `country_list` VALUES (240, 'Wales', 6, 2115);
INSERT INTO `country_list` VALUES (241, 'Isle of Man', 6, 965);

-- --------------------------------------------------------

-- 
-- Table structure for table `coverletter`
-- 

DROP TABLE IF EXISTS `coverletter`;
CREATE TABLE IF NOT EXISTS `coverletter` (
  `student_number` varchar(10) NOT NULL default '',
  `job_id` mediumint(8) unsigned NOT NULL default '0',
  `last_modified` datetime default NULL,
  `type_id` smallint(5) unsigned NOT NULL default '0',
  `status_id` smallint(5) unsigned default NULL,
  `letter` mediumtext,
  `include_transcript` tinyint(3) unsigned default '1',
  `resume_id` mediumint(8) unsigned default NULL,
  `coverletter_id` mediumint(8) unsigned NOT NULL auto_increment,
  `builder_class` mediumtext,
  PRIMARY KEY  (`coverletter_id`),
  KEY `student_number` (`student_number`),
  KEY `job_id` (`job_id`),
  KEY `resume_id` (`resume_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `coverletter`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `coverletter_status`
-- 

DROP TABLE IF EXISTS `coverletter_status`;
CREATE TABLE IF NOT EXISTS `coverletter_status` (
  `status_id` smallint(5) unsigned NOT NULL auto_increment,
  `description` varchar(50) NOT NULL default '',
  PRIMARY KEY  (`status_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `coverletter_status`
-- 

INSERT INTO `coverletter_status` VALUES (1, 'Online');
INSERT INTO `coverletter_status` VALUES (2, 'Old');

-- --------------------------------------------------------

-- 
-- Table structure for table `coverletter_type`
-- 

DROP TABLE IF EXISTS `coverletter_type`;
CREATE TABLE IF NOT EXISTS `coverletter_type` (
  `type_id` smallint(5) unsigned NOT NULL auto_increment,
  `description` varchar(50) NOT NULL default '',
  PRIMARY KEY  (`type_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `coverletter_type`
-- 

INSERT INTO `coverletter_type` VALUES (1, 'Specific');
INSERT INTO `coverletter_type` VALUES (2, 'Template');
INSERT INTO `coverletter_type` VALUES (3, 'Generic');

-- --------------------------------------------------------

-- 
-- Table structure for table `department`
-- 

DROP TABLE IF EXISTS `department`;
CREATE TABLE IF NOT EXISTS `department` (
  `department_id` smallint(5) unsigned NOT NULL auto_increment,
  `department_code` varchar(10) NOT NULL default '',
  `department_name` varchar(50) NOT NULL default '',
  `default_building_id` smallint(5) unsigned default NULL,
  `email` varchar(100) default NULL,
  `general_instructions` text,
  `oncampus_instructions` text,
  `offcampus_instructions` text,
  `phone_instructions` text,
  `newsgroup_email` varchar(100) default NULL,
  `video_instructions` text,
  `department_faculty` varchar(100) default NULL,
  `phone_assistance` varchar(75) default NULL,
  `default_int_phone` varchar(75) default NULL,
  `default_login` varchar(50) default 'NETLINK',
  `student_interview_instructions` text,
  `job_list` tinyint(3) unsigned default NULL,
  `resume_html_editable` tinyint(1) unsigned default '0',
  `application_employer_body` text,
  `primary_contact_id` bigint(20) unsigned default NULL,
  `resume_allow_html_editor` char(1) binary default '1',
  `resume_allow_builder` char(1) binary default '0',
  `placement_contact_id` bigint(20) unsigned default NULL,
  `using_full_system` tinyint(1) unsigned default '0',
  `menus_being_used_id` mediumint(8) unsigned default '2',
  `coverletter_allow_html_editor` char(1) binary default '1',
  `coverletter_allow_builder` char(1) binary default '1',
  `job_app_paper_instructions` text,
  `shortlist_contact_id` bigint(20) unsigned default NULL,
  `interview_contact_id` bigint(20) unsigned default NULL,
  `jobhost_contact_id` bigint(20) unsigned default NULL,
  PRIMARY KEY  (`department_id`),
  UNIQUE KEY `department_name_index` (`department_name`),
  UNIQUE KEY `department_code_index` (`department_code`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department`
-- 

INSERT INTO `department` VALUES (1, 'ENGR', 'Engineering', 2, '', '', '', '', '', '', '', 'Faculty of Engineering', '', '', 'NIS', '', 1, 1, '', 19, 0x31, 0x30, 19, 1, 1, 0x31, 0x31, '', 19, 19, 19);
INSERT INTO `department` VALUES (2, 'CSMA', 'Computer Science / Math', 1, '', '', '', '', '', '', '', 'Faculty of Engineering', '', '', 'netlink', '', 1, 1, '', 19, 0x31, 0x30, 19, 1, 1, 0x31, 0x31, '', 19, 19, 19);
INSERT INTO `department` VALUES (15, 'COOP', 'Director''s Office of Co-operative Education', NULL, '', '', '', '', '', '', '', NULL, '', '250-721-7628', 'netlink', '', NULL, 1, '', 19, 0x31, 0x30, 19, 0, 4, 0x31, 0x31, '', 19, 19, 19);
INSERT INTO `department` VALUES (18, 'SERVICE', 'Employer Service', NULL, '', NULL, NULL, NULL, NULL, NULL, NULL, NULL, '', NULL, 'netlink', NULL, NULL, 1, '', 19, 0x31, 0x30, 19, 0, 5, 0x31, 0x31, '', 19, 19, 19);

-- --------------------------------------------------------

-- 
-- Table structure for table `department_company_flags`
-- 

DROP TABLE IF EXISTS `department_company_flags`;
CREATE TABLE IF NOT EXISTS `department_company_flags` (
  `department_id` mediumint(8) unsigned default NULL,
  `flag_id` mediumint(8) unsigned default NULL,
  KEY `department_id` (`department_id`),
  KEY `flag_id` (`flag_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_company_flags`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `department_company_status`
-- 

DROP TABLE IF EXISTS `department_company_status`;
CREATE TABLE IF NOT EXISTS `department_company_status` (
  `employer_id` mediumint(8) unsigned NOT NULL default '0',
  `status_id` mediumint(8) unsigned NOT NULL default '0',
  `department_id` smallint(5) unsigned NOT NULL default '0',
  `entered_on` datetime default NULL,
  `current_status` tinyint(1) NOT NULL default '0',
  `department_company_status_id` bigint(20) unsigned NOT NULL auto_increment,
  PRIMARY KEY  (`department_company_status_id`),
  KEY `employer_id` (`employer_id`),
  KEY `department_id` (`department_id`),
  KEY `status_id` (`status_id`),
  KEY `entered_on` (`entered_on`),
  KEY `current_status` (`current_status`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_company_status`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `department_contact_flags`
-- 

DROP TABLE IF EXISTS `department_contact_flags`;
CREATE TABLE IF NOT EXISTS `department_contact_flags` (
  `department_id` mediumint(8) unsigned default NULL,
  `flag_id` mediumint(8) unsigned default NULL,
  KEY `department_id` (`department_id`),
  KEY `flag_id` (`flag_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_contact_flags`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `department_contact_status`
-- 

DROP TABLE IF EXISTS `department_contact_status`;
CREATE TABLE IF NOT EXISTS `department_contact_status` (
  `contact_id` bigint(20) unsigned NOT NULL default '0',
  `status_id` smallint(5) unsigned NOT NULL default '0',
  `department_id` smallint(5) unsigned NOT NULL default '0',
  `entered_on` datetime default NULL,
  `activity_type_id` tinyint(3) unsigned default NULL,
  `history_id` int(11) unsigned NOT NULL default '0',
  `job_id` mediumint(8) unsigned NOT NULL default '0',
  `term_id` smallint(5) unsigned NOT NULL default '0',
  `year` year(4) NOT NULL default '0000',
  `current_status` tinyint(1) NOT NULL default '0',
  `activity_date` date default NULL,
  `department_contact_status_id` bigint(20) unsigned NOT NULL auto_increment,
  PRIMARY KEY  (`department_contact_status_id`),
  KEY `contact_id` (`contact_id`),
  KEY `status_id` (`status_id`),
  KEY `department_id` (`department_id`),
  KEY `current_status` (`current_status`),
  KEY `activity_date` (`activity_date`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_contact_status`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `department_default_semesters`
-- 

DROP TABLE IF EXISTS `department_default_semesters`;
CREATE TABLE IF NOT EXISTS `department_default_semesters` (
  `department_id` smallint(5) unsigned default NULL,
  `timetable_num` tinyint(3) default NULL,
  `term_id` tinyint(3) unsigned default NULL,
  `semesters_id` smallint(6) default NULL,
  `year_num` smallint(1) unsigned default NULL,
  KEY `department_id` (`department_id`),
  KEY `timetable_num` (`timetable_num`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_default_semesters`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `department_discipline_join`
-- 

DROP TABLE IF EXISTS `department_discipline_join`;
CREATE TABLE IF NOT EXISTS `department_discipline_join` (
  `department_id` smallint(5) unsigned default NULL,
  `discipline_id` mediumint(8) unsigned default NULL,
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_discipline_join`
-- 

INSERT INTO `department_discipline_join` VALUES (2, 2);
INSERT INTO `department_discipline_join` VALUES (2, 3);
INSERT INTO `department_discipline_join` VALUES (2, 4);
INSERT INTO `department_discipline_join` VALUES (2, 5);
INSERT INTO `department_discipline_join` VALUES (2, 6);
INSERT INTO `department_discipline_join` VALUES (1, 7);
INSERT INTO `department_discipline_join` VALUES (1, 8);
INSERT INTO `department_discipline_join` VALUES (1, 9);
INSERT INTO `department_discipline_join` VALUES (1, 3);

-- --------------------------------------------------------

-- 
-- Table structure for table `department_disciplines`
-- 

DROP TABLE IF EXISTS `department_disciplines`;
CREATE TABLE IF NOT EXISTS `department_disciplines` (
  `discipline_id` mediumint(8) unsigned NOT NULL auto_increment,
  `discipline_name` varchar(150) default NULL,
  `discipline_code` varchar(50) default NULL,
  PRIMARY KEY  (`discipline_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_disciplines`
-- 

INSERT INTO `department_disciplines` VALUES (1, 'All Disciplines', 'ALL');
INSERT INTO `department_disciplines` VALUES (2, 'CSc Major & Honours', 'CSc');
INSERT INTO `department_disciplines` VALUES (3, 'Software Engineering', 'SEng');
INSERT INTO `department_disciplines` VALUES (4, 'Math & Statistics', 'Math');
INSERT INTO `department_disciplines` VALUES (5, 'CSc - Business Option', 'CSc-Bus');
INSERT INTO `department_disciplines` VALUES (6, 'CSc & Physics Combined', 'CSc-Phy');
INSERT INTO `department_disciplines` VALUES (7, 'Computer Engineering', 'CEng');
INSERT INTO `department_disciplines` VALUES (8, 'Electrical Engineering', 'EEng');
INSERT INTO `department_disciplines` VALUES (9, 'Mechanical Engineering', 'MEng');

-- --------------------------------------------------------

-- 
-- Table structure for table `department_division_flags`
-- 

DROP TABLE IF EXISTS `department_division_flags`;
CREATE TABLE IF NOT EXISTS `department_division_flags` (
  `department_id` mediumint(8) unsigned default NULL,
  `flag_id` mediumint(8) unsigned default NULL,
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_division_flags`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `department_division_status`
-- 

DROP TABLE IF EXISTS `department_division_status`;
CREATE TABLE IF NOT EXISTS `department_division_status` (
  `division_id` mediumint(8) unsigned NOT NULL default '0',
  `status_id` mediumint(8) unsigned NOT NULL default '0',
  `department_id` smallint(5) unsigned NOT NULL default '0',
  `entered_on` datetime default NULL,
  `current_status` tinyint(1) NOT NULL default '0',
  `department_division_status_id` bigint(20) unsigned NOT NULL auto_increment,
  PRIMARY KEY  (`department_division_status_id`),
  KEY `division_id` (`division_id`),
  KEY `status_id` (`status_id`),
  KEY `department_id` (`department_id`),
  KEY `entered_on` (`entered_on`),
  KEY `current_status` (`current_status`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_division_status`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `department_flags_set`
-- 

DROP TABLE IF EXISTS `department_flags_set`;
CREATE TABLE IF NOT EXISTS `department_flags_set` (
  `department_id` mediumint(8) unsigned default NULL,
  `flag_id` mediumint(8) unsigned default NULL,
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_flags_set`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `department_group`
-- 

DROP TABLE IF EXISTS `department_group`;
CREATE TABLE IF NOT EXISTS `department_group` (
  `group_id` smallint(5) unsigned default NULL,
  `department_id` smallint(5) unsigned default NULL,
  UNIQUE KEY `department_group_indx` (`group_id`,`department_id`),
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_group`
-- 

INSERT INTO `department_group` VALUES (1, 1);
INSERT INTO `department_group` VALUES (1, 2);

-- --------------------------------------------------------

-- 
-- Table structure for table `department_history_flags`
-- 

DROP TABLE IF EXISTS `department_history_flags`;
CREATE TABLE IF NOT EXISTS `department_history_flags` (
  `department_id` smallint(5) unsigned default NULL,
  `history_flags_id` int(11) default NULL,
  KEY `department_id` (`department_id`),
  KEY `history_flags_id` (`history_flags_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_history_flags`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `department_history_report_subjects`
-- 

DROP TABLE IF EXISTS `department_history_report_subjects`;
CREATE TABLE IF NOT EXISTS `department_history_report_subjects` (
  `report_subject_id` int(11) unsigned default NULL,
  `department_id` smallint(5) unsigned default NULL,
  KEY `report_subject_id` (`report_subject_id`),
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_history_report_subjects`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `department_job_join`
-- 

DROP TABLE IF EXISTS `department_job_join`;
CREATE TABLE IF NOT EXISTS `department_job_join` (
  `department_id` smallint(5) unsigned default NULL,
  `job_id` mediumint(8) unsigned default NULL,
  KEY `department_id` (`department_id`),
  KEY `job_id` (`job_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_job_join`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `department_module`
-- 

DROP TABLE IF EXISTS `department_module`;
CREATE TABLE IF NOT EXISTS `department_module` (
  `module_id` smallint(5) unsigned default NULL,
  `department_id` smallint(5) unsigned default NULL,
  UNIQUE KEY `department_module_indx` (`module_id`,`department_id`),
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_module`
-- 

INSERT INTO `department_module` VALUES (3, 1);
INSERT INTO `department_module` VALUES (3, 2);
INSERT INTO `department_module` VALUES (3, 18);
INSERT INTO `department_module` VALUES (5, 1);
INSERT INTO `department_module` VALUES (5, 2);
INSERT INTO `department_module` VALUES (5, 18);
INSERT INTO `department_module` VALUES (6, 1);
INSERT INTO `department_module` VALUES (6, 2);
INSERT INTO `department_module` VALUES (6, 18);
INSERT INTO `department_module` VALUES (10, 1);
INSERT INTO `department_module` VALUES (10, 2);
INSERT INTO `department_module` VALUES (10, 18);
INSERT INTO `department_module` VALUES (12, 1);
INSERT INTO `department_module` VALUES (12, 2);
INSERT INTO `department_module` VALUES (12, 18);
INSERT INTO `department_module` VALUES (14, 1);
INSERT INTO `department_module` VALUES (14, 2);
INSERT INTO `department_module` VALUES (15, 1);
INSERT INTO `department_module` VALUES (15, 2);
INSERT INTO `department_module` VALUES (15, 18);
INSERT INTO `department_module` VALUES (18, 1);
INSERT INTO `department_module` VALUES (18, 2);
INSERT INTO `department_module` VALUES (19, 1);
INSERT INTO `department_module` VALUES (19, 2);
INSERT INTO `department_module` VALUES (19, 18);
INSERT INTO `department_module` VALUES (20, 1);
INSERT INTO `department_module` VALUES (20, 2);
INSERT INTO `department_module` VALUES (20, 18);
INSERT INTO `department_module` VALUES (40, 1);
INSERT INTO `department_module` VALUES (40, 2);
INSERT INTO `department_module` VALUES (41, 1);
INSERT INTO `department_module` VALUES (41, 2);
INSERT INTO `department_module` VALUES (41, 18);

-- --------------------------------------------------------

-- 
-- Table structure for table `department_semesters`
-- 

DROP TABLE IF EXISTS `department_semesters`;
CREATE TABLE IF NOT EXISTS `department_semesters` (
  `department_id` smallint(5) unsigned default NULL,
  `semesters_id` int(11) default NULL,
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_semesters`
-- 

INSERT INTO `department_semesters` VALUES (1, 1);
INSERT INTO `department_semesters` VALUES (1, 7);
INSERT INTO `department_semesters` VALUES (1, 8);
INSERT INTO `department_semesters` VALUES (1, 5);
INSERT INTO `department_semesters` VALUES (1, 3);
INSERT INTO `department_semesters` VALUES (1, 2);
INSERT INTO `department_semesters` VALUES (1, 17);
INSERT INTO `department_semesters` VALUES (1, 13);
INSERT INTO `department_semesters` VALUES (1, 15);
INSERT INTO `department_semesters` VALUES (1, 14);
INSERT INTO `department_semesters` VALUES (1, 16);
INSERT INTO `department_semesters` VALUES (1, 4);
INSERT INTO `department_semesters` VALUES (1, 11);
INSERT INTO `department_semesters` VALUES (1, 10);
INSERT INTO `department_semesters` VALUES (1, 12);
INSERT INTO `department_semesters` VALUES (1, 6);
INSERT INTO `department_semesters` VALUES (1, 9);
INSERT INTO `department_semesters` VALUES (2, 20);
INSERT INTO `department_semesters` VALUES (2, 3);
INSERT INTO `department_semesters` VALUES (2, 1);
INSERT INTO `department_semesters` VALUES (2, 2);
INSERT INTO `department_semesters` VALUES (2, 4);
INSERT INTO `department_semesters` VALUES (2, 5);
INSERT INTO `department_semesters` VALUES (2, 6);
INSERT INTO `department_semesters` VALUES (2, 7);
INSERT INTO `department_semesters` VALUES (2, 8);
INSERT INTO `department_semesters` VALUES (2, 17);
INSERT INTO `department_semesters` VALUES (2, 29);
INSERT INTO `department_semesters` VALUES (2, 26);
INSERT INTO `department_semesters` VALUES (2, 27);
INSERT INTO `department_semesters` VALUES (2, 21);
INSERT INTO `department_semesters` VALUES (2, 30);
INSERT INTO `department_semesters` VALUES (1, 30);
INSERT INTO `department_semesters` VALUES (1, 22);
INSERT INTO `department_semesters` VALUES (1, 23);
INSERT INTO `department_semesters` VALUES (1, 24);
INSERT INTO `department_semesters` VALUES (1, 25);
INSERT INTO `department_semesters` VALUES (1, 26);
INSERT INTO `department_semesters` VALUES (1, 27);
INSERT INTO `department_semesters` VALUES (1, 28);
INSERT INTO `department_semesters` VALUES (1, 21);
INSERT INTO `department_semesters` VALUES (1, 29);

-- --------------------------------------------------------

-- 
-- Table structure for table `department_student_flags`
-- 

DROP TABLE IF EXISTS `department_student_flags`;
CREATE TABLE IF NOT EXISTS `department_student_flags` (
  `department_id` smallint(5) unsigned default NULL,
  `student_flags_id` int(11) default NULL,
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `department_student_flags`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `discipline`
-- 

DROP TABLE IF EXISTS `discipline`;
CREATE TABLE IF NOT EXISTS `discipline` (
  `discipline_id` smallint(6) unsigned NOT NULL auto_increment,
  `department_id` smallint(5) unsigned default NULL,
  `discipline_name` varchar(50) default NULL,
  `obsolete` tinyint(3) NOT NULL default '0',
  `use_history_containers_hours` tinyint(1) NOT NULL default '0',
  `use_history_containers_term` tinyint(1) NOT NULL default '0',
  `history_containers_spill_over_hours` smallint(5) unsigned NOT NULL default '0',
  `history_containers_min_days_for_wt` smallint(5) unsigned NOT NULL default '0',
  `history_containers_length_hours` smallint(5) unsigned NOT NULL default '0',
  `history_containers_length_term` smallint(5) unsigned NOT NULL default '0',
  `max_num_workterms` smallint(5) unsigned NOT NULL default '0',
  `edit_history_grace_period_days` smallint(5) unsigned default NULL,
  PRIMARY KEY  (`discipline_id`),
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `discipline`
-- 

INSERT INTO `discipline` VALUES (4, 2, 'CSc Major', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (5, 2, 'CSc/SEng Opt', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (6, 2, 'Csc/Business', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (7, 1, 'Engr - Me', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (8, 1, 'Engr - El', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (9, 1, 'Engr - Ce', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (10, 1, 'Engr - Und', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (11, 2, 'Math Honors', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (12, 2, 'CSc/Physics', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (13, 2, 'CSc Honors', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (14, 2, 'CSc Major/Geog Major', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (15, 2, 'Math Major', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (16, 2, 'CSc/Math Major', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (17, 2, 'CSc/Math Hons', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (18, 2, 'CSc/Stats', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (19, 2, 'M.Sc. in CSc', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (20, 1, 'ENGR', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (22, 2, 'CSc Undecided', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (27, 2, 'Ph.D. in CSc', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (54, 2, 'Statistic Honours', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (39, 2, 'M.Sc. in Math', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (41, 2, 'Ph.D. in Math', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (44, 2, 'Exchange', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (52, 2, 'CSc Double Major', 0, 0, 0, 0, 0, 0, 0, 0, NULL);
INSERT INTO `discipline` VALUES (53, 2, 'CSc Major + Minor', 0, 0, 0, 0, 0, 0, 0, 0, NULL);

-- --------------------------------------------------------

-- 
-- Table structure for table `division_flags`
-- 

DROP TABLE IF EXISTS `division_flags`;
CREATE TABLE IF NOT EXISTS `division_flags` (
  `flag_name` varchar(50) default NULL,
  `flag_id` mediumint(8) unsigned NOT NULL auto_increment,
  `comment` tinytext,
  PRIMARY KEY  (`flag_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `division_flags`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `division_notes`
-- 

DROP TABLE IF EXISTS `division_notes`;
CREATE TABLE IF NOT EXISTS `division_notes` (
  `department_id` mediumint(8) unsigned default NULL,
  `notes` text,
  `date_entered` datetime default NULL,
  `entered_by` bigint(20) unsigned default NULL,
  `note_id` mediumint(8) unsigned NOT NULL auto_increment,
  PRIMARY KEY  (`note_id`),
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `division_notes`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `edit_student_flags`
-- 

DROP TABLE IF EXISTS `edit_student_flags`;
CREATE TABLE IF NOT EXISTS `edit_student_flags` (
  `student_flags_id` int(11) default NULL,
  `term_id` tinyint(3) unsigned default NULL,
  `year_space` tinyint(4) default NULL,
  `order_by` int(11) default NULL,
  KEY `student_flags_id` (`student_flags_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `edit_student_flags`
-- 

INSERT INTO `edit_student_flags` VALUES (5, 2, -1, 10);
INSERT INTO `edit_student_flags` VALUES (5, 3, -1, 11);
INSERT INTO `edit_student_flags` VALUES (5, 1, -1, 12);
INSERT INTO `edit_student_flags` VALUES (5, 2, 1, 16);
INSERT INTO `edit_student_flags` VALUES (5, 3, 0, 14);
INSERT INTO `edit_student_flags` VALUES (5, 1, 0, 15);
INSERT INTO `edit_student_flags` VALUES (5, 2, 0, 13);
INSERT INTO `edit_student_flags` VALUES (5, 3, 1, 17);
INSERT INTO `edit_student_flags` VALUES (5, 1, 1, 18);
INSERT INTO `edit_student_flags` VALUES (4, 3, -1, 2);
INSERT INTO `edit_student_flags` VALUES (4, 2, -1, 1);
INSERT INTO `edit_student_flags` VALUES (4, 1, -1, 3);
INSERT INTO `edit_student_flags` VALUES (4, 2, 0, 4);
INSERT INTO `edit_student_flags` VALUES (4, 3, 0, 5);
INSERT INTO `edit_student_flags` VALUES (4, 1, 0, 6);
INSERT INTO `edit_student_flags` VALUES (4, 2, 1, 7);
INSERT INTO `edit_student_flags` VALUES (4, 3, 1, 8);
INSERT INTO `edit_student_flags` VALUES (4, 1, 1, 9);

-- --------------------------------------------------------

-- 
-- Table structure for table `eligible_ever`
-- 

DROP TABLE IF EXISTS `eligible_ever`;
CREATE TABLE IF NOT EXISTS `eligible_ever` (
  `record_id` int(11) default NULL,
  `term_id` tinyint(3) unsigned default NULL,
  `year` year(4) default NULL,
  KEY `record_id` (`record_id`),
  KEY `term_id` (`term_id`),
  KEY `year` (`year`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `eligible_ever`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `eligible_placed`
-- 

DROP TABLE IF EXISTS `eligible_placed`;
CREATE TABLE IF NOT EXISTS `eligible_placed` (
  `student_flags_id` int(11) default NULL,
  `record_id` int(11) default NULL,
  `term_id` tinyint(3) unsigned default NULL,
  `year` year(4) default NULL,
  `eval_stu_coop_completed` char(1) binary default '0',
  KEY `record_id` (`record_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `eligible_placed`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `employer_apps_flagged`
-- 

DROP TABLE IF EXISTS `employer_apps_flagged`;
CREATE TABLE IF NOT EXISTS `employer_apps_flagged` (
  `application_id` varchar(10) default NULL,
  `contact_id` mediumint(8) unsigned default NULL,
  KEY `contact_id` (`contact_id`),
  KEY `application_id` (`application_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `employer_apps_flagged`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `employer_company`
-- 

DROP TABLE IF EXISTS `employer_company`;
CREATE TABLE IF NOT EXISTS `employer_company` (
  `employer_id` mediumint(8) unsigned NOT NULL auto_increment,
  `company_name` varchar(75) NOT NULL default '',
  `website` varchar(150) default NULL,
  `company_description` text,
  `street_address_1` varchar(75) default NULL,
  `street_address_2` varchar(75) default NULL,
  `street_address_3` varchar(75) default NULL,
  `city` varchar(40) default NULL,
  `provstate_id` mediumint(8) unsigned NOT NULL default '0',
  `country_id` mediumint(8) unsigned NOT NULL default '0',
  `postal_code` varchar(10) default NULL,
  `phone` varchar(25) default NULL,
  `fax` varchar(25) default NULL,
  `email` varchar(100) default NULL,
  `changes_recorded_1` text,
  `changes_recorded_2` text,
  `changes_recorded_3` text,
  `change_by_1` bigint(20) unsigned default NULL,
  `change_by_2` bigint(20) unsigned default NULL,
  `change_by_3` bigint(20) unsigned default NULL,
  `change_date_1` date default NULL,
  `change_date_2` date default NULL,
  `change_date_3` date default NULL,
  `industry_id` mediumint(8) unsigned default '45',
  `entered_by` varchar(40) default NULL,
  `entered_on` date default NULL,
  `general_comments` text,
  `size_id` mediumint(8) unsigned default '7',
  `region_id` mediumint(8) unsigned NOT NULL default '0',
  `company_display` tinyint(3) unsigned default '0',
  `company_type_id` tinyint(4) unsigned default NULL,
  `company_name_legitimate` tinyint(3) unsigned default '1',
  PRIMARY KEY  (`employer_id`),
  KEY `company_name` (`company_name`),
  KEY `provstate_id` (`provstate_id`),
  KEY `country_id` (`country_id`),
  KEY `phone` (`phone`),
  KEY `industry_id` (`industry_id`),
  KEY `region_id` (`region_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `employer_company`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `employer_company_comments`
-- 

DROP TABLE IF EXISTS `employer_company_comments`;
CREATE TABLE IF NOT EXISTS `employer_company_comments` (
  `employer_company_id` mediumint(8) unsigned default NULL,
  `department_id` smallint(5) unsigned default NULL,
  `comments` text,
  KEY `employer_company_id` (`employer_company_id`),
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `employer_company_comments`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `employer_contact_comments`
-- 

DROP TABLE IF EXISTS `employer_contact_comments`;
CREATE TABLE IF NOT EXISTS `employer_contact_comments` (
  `employer_contact_id` bigint(20) unsigned default NULL,
  `department_id` smallint(5) unsigned default NULL,
  `comments` text,
  KEY `employer_contact_id` (`employer_contact_id`),
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `employer_contact_comments`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `employer_department`
-- 

DROP TABLE IF EXISTS `employer_department`;
CREATE TABLE IF NOT EXISTS `employer_department` (
  `department_id` mediumint(8) unsigned NOT NULL auto_increment,
  `employer_id` mediumint(8) unsigned default NULL,
  `department_name` varchar(75) default NULL,
  `department_website` varchar(150) default NULL,
  `department_description` text,
  `street_address_1` varchar(75) default NULL,
  `street_address_2` varchar(75) default NULL,
  `street_address_3` varchar(75) default NULL,
  `city` varchar(40) default NULL,
  `provstate_id` mediumint(8) unsigned NOT NULL default '0',
  `country_id` mediumint(8) unsigned NOT NULL default '0',
  `postal_code` varchar(10) default NULL,
  `phone` varchar(25) default NULL,
  `fax` varchar(25) default NULL,
  `email` varchar(100) default NULL,
  `changes_recorded_1` text,
  `changes_recorded_2` text,
  `changes_recorded_3` text,
  `change_by_1` bigint(20) unsigned default NULL,
  `change_by_2` bigint(20) unsigned default NULL,
  `change_by_3` bigint(20) unsigned default NULL,
  `change_date_1` date default NULL,
  `change_date_2` date default NULL,
  `change_date_3` date default NULL,
  `industry_id` mediumint(8) unsigned default '45',
  `entered_by` varchar(40) default NULL,
  `entered_on` date default NULL,
  `general_comments` text,
  `region_id` mediumint(8) unsigned NOT NULL default '0',
  `size_id` mediumint(8) unsigned default NULL,
  `location_info` varchar(25) NOT NULL default '0',
  `department_name_legitimate` tinyint(3) unsigned default '1',
  PRIMARY KEY  (`department_id`),
  KEY `employer_id` (`employer_id`),
  KEY `department_name` (`department_name`),
  KEY `provstate_id` (`provstate_id`),
  KEY `country_id` (`country_id`),
  KEY `phone` (`phone`),
  KEY `industry_id` (`industry_id`),
  KEY `region_id` (`region_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `employer_department`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `employer_department_comments`
-- 

DROP TABLE IF EXISTS `employer_department_comments`;
CREATE TABLE IF NOT EXISTS `employer_department_comments` (
  `employer_department_id` mediumint(8) unsigned default NULL,
  `department_id` smallint(5) unsigned default NULL,
  `comments` text,
  KEY `employer_department_id` (`employer_department_id`),
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `employer_department_comments`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `employer_info_activity_type`
-- 

DROP TABLE IF EXISTS `employer_info_activity_type`;
CREATE TABLE IF NOT EXISTS `employer_info_activity_type` (
  `activity_type_id` tinyint(3) unsigned NOT NULL auto_increment,
  `activity_type_name` varchar(80) default NULL,
  `description` varchar(80) default NULL,
  PRIMARY KEY  (`activity_type_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `employer_info_activity_type`
-- 

INSERT INTO `employer_info_activity_type` VALUES (1, 'Activity', 'Status changed due to activity');
INSERT INTO `employer_info_activity_type` VALUES (2, 'Automated', 'Status changed due to automated script');

-- --------------------------------------------------------

-- 
-- Table structure for table `employer_info_status_flags`
-- 

DROP TABLE IF EXISTS `employer_info_status_flags`;
CREATE TABLE IF NOT EXISTS `employer_info_status_flags` (
  `status_flag_id` smallint(5) unsigned NOT NULL auto_increment,
  `status_flag_name` varchar(80) default NULL,
  PRIMARY KEY  (`status_flag_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `employer_info_status_flags`
-- 

INSERT INTO `employer_info_status_flags` VALUES (1, 'Do Not Contact');
INSERT INTO `employer_info_status_flags` VALUES (2, 'Deleted');

-- --------------------------------------------------------

-- 
-- Table structure for table `employer_info_status_flags_join`
-- 

DROP TABLE IF EXISTS `employer_info_status_flags_join`;
CREATE TABLE IF NOT EXISTS `employer_info_status_flags_join` (
  `contact_id` bigint(20) unsigned default NULL,
  `status_flag_id` smallint(5) unsigned default NULL,
  `entered_by` bigint(20) unsigned default NULL,
  `entered_on` datetime default NULL,
  `expiry_date` date default NULL,
  KEY `contact_id` (`contact_id`),
  KEY `status_flag_id` (`status_flag_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `employer_info_status_flags_join`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `employer_info_statuses`
-- 

DROP TABLE IF EXISTS `employer_info_statuses`;
CREATE TABLE IF NOT EXISTS `employer_info_statuses` (
  `status_id` smallint(5) unsigned NOT NULL auto_increment,
  `status_name` varchar(100) default NULL,
  `level_of_activity` mediumint(8) unsigned default NULL,
  PRIMARY KEY  (`status_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `employer_info_statuses`
-- 

INSERT INTO `employer_info_statuses` VALUES (1, 'Active Veteran', 10);
INSERT INTO `employer_info_statuses` VALUES (2, 'Active Lapsed', 20);
INSERT INTO `employer_info_statuses` VALUES (3, 'Active New', 30);
INSERT INTO `employer_info_statuses` VALUES (4, 'Inactive Veteran', 40);
INSERT INTO `employer_info_statuses` VALUES (5, 'Inactive Lapsed', 50);
INSERT INTO `employer_info_statuses` VALUES (6, 'No Activity', 60);

-- --------------------------------------------------------

-- 
-- Table structure for table `employer_login`
-- 

DROP TABLE IF EXISTS `employer_login`;
CREATE TABLE IF NOT EXISTS `employer_login` (
  `employer_id` mediumint(8) unsigned default '0',
  `contact_id` bigint(20) unsigned NOT NULL default '0',
  `login_id` varchar(20) default NULL,
  `password` varchar(30) default NULL,
  `department_id` smallint(5) unsigned default NULL,
  PRIMARY KEY  (`contact_id`),
  KEY `employer_id` (`employer_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `employer_login`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `employer_sizes`
-- 

DROP TABLE IF EXISTS `employer_sizes`;
CREATE TABLE IF NOT EXISTS `employer_sizes` (
  `size_id` mediumint(8) unsigned NOT NULL auto_increment,
  `size_range` varchar(75) default NULL,
  PRIMARY KEY  (`size_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `employer_sizes`
-- 

INSERT INTO `employer_sizes` VALUES (1, '1-20');
INSERT INTO `employer_sizes` VALUES (2, '21-50');
INSERT INTO `employer_sizes` VALUES (3, '51-100');
INSERT INTO `employer_sizes` VALUES (4, '101-500');
INSERT INTO `employer_sizes` VALUES (6, 'Over 500');
INSERT INTO `employer_sizes` VALUES (7, 'Unknown');

-- --------------------------------------------------------

-- 
-- Table structure for table `forgot_password_blacklist`
-- 

DROP TABLE IF EXISTS `forgot_password_blacklist`;
CREATE TABLE IF NOT EXISTS `forgot_password_blacklist` (
  `blacklisted_IP` varchar(50) default NULL,
  `emails_attempted` text,
  `datetime_blacklisted` datetime default NULL
) ENGINE=MyISAM;

-- 
-- Dumping data for table `forgot_password_blacklist`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `forgot_password_blacklist_cleared`
-- 

DROP TABLE IF EXISTS `forgot_password_blacklist_cleared`;
CREATE TABLE IF NOT EXISTS `forgot_password_blacklist_cleared` (
  `blacklisted_IP` varchar(50) default NULL,
  `emails_attempted` text,
  `datetime_blacklisted` datetime default NULL,
  `datetime_cleared` datetime default NULL,
  `notes` text
) ENGINE=MyISAM;

-- 
-- Dumping data for table `forgot_password_blacklist_cleared`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `history`
-- 

DROP TABLE IF EXISTS `history`;
CREATE TABLE IF NOT EXISTS `history` (
  `history_id` int(11) NOT NULL auto_increment,
  `employer_id` mediumint(8) unsigned default NULL,
  `contact_id` bigint(20) unsigned default NULL,
  `contact_id2` bigint(20) unsigned default NULL,
  `student_number` varchar(10) default NULL,
  `work_term_number` tinyint(4) default NULL,
  `student_work_phone` varchar(25) default NULL,
  `job_title` varchar(100) default NULL,
  `site_visit_by` bigint(20) unsigned default NULL,
  `report_title` varchar(255) default NULL,
  `report_code` varchar(50) default NULL,
  `term_id` tinyint(3) unsigned default NULL,
  `year` year(4) default NULL,
  `work_term_length` tinyint(4) default NULL,
  `work_email` varchar(100) default NULL,
  `salary` decimal(7,2) default NULL,
  `company_department_id` mediumint(8) unsigned default NULL,
  `company_city` varchar(40) default NULL,
  `report_subject` tinyint(4) unsigned NOT NULL default '0',
  `comment` text,
  `work_site_visit_notes` text,
  `company_name` varchar(75) default NULL,
  `department_name` varchar(75) default NULL,
  `company_street_address_1` varchar(75) default NULL,
  `company_street_address_2` varchar(75) default NULL,
  `company_postal_code` varchar(10) default NULL,
  `company_position` varchar(100) default NULL,
  `supervisor_phone_number` varchar(35) default NULL,
  `supervisor_name` varchar(150) default NULL,
  `supervisor_fax_number` varchar(35) default NULL,
  `supervisor_email` varchar(100) default NULL,
  `work_term_street_address_1` varchar(75) default NULL,
  `work_term_street_address_2` varchar(75) default NULL,
  `work_term_city` varchar(40) default NULL,
  `work_term_postal_code` varchar(10) default NULL,
  `work_term_province` mediumint(8) unsigned default NULL,
  `work_term_country` mediumint(8) unsigned default NULL,
  `wt_status` tinyint(4) default NULL,
  `options` tinyint(4) NOT NULL default '0',
  `hitech` tinyint(4) default NULL,
  `job_id` mediumint(8) unsigned default NULL,
  `site_visit_date_supervisor` date default NULL,
  `company_province` mediumint(8) unsigned default NULL,
  `work_term_home_phone` varchar(35) default NULL,
  `company_country` mediumint(8) unsigned default NULL,
  `site_visit_type_id_supervisor` tinyint(4) unsigned default NULL,
  `site_visit_date` date default NULL,
  `site_visit_type_id` tinyint(4) unsigned default NULL,
  `report_marker` varchar(150) default NULL,
  `site_visit_by_supervisor` bigint(20) unsigned default NULL,
  `supervisor_position` varchar(100) default NULL,
  `department_id` smallint(5) unsigned default NULL,
  `changes_recorded_1` text,
  `changes_recorded_2` text,
  `changes_recorded_3` text,
  `change_by_1` bigint(20) unsigned default NULL,
  `change_by_2` bigint(20) unsigned default NULL,
  `change_by_3` bigint(20) unsigned default NULL,
  `change_date_1` date default NULL,
  `change_date_2` date default NULL,
  `change_date_3` date default NULL,
  `company_region_id` mediumint(8) unsigned default NULL,
  `eval_stu_wt_completed` char(1) binary default '0',
  `work_term_region_id` mediumint(8) unsigned default NULL,
  `salary_period_id` smallint(6) default NULL,
  `work_term_hours_per_week` decimal(4,2) default NULL,
  `temp_supervisor_contact` bigint(20) unsigned default NULL,
  `temp_job_contact` bigint(20) unsigned default NULL,
  `temp_date_entered` date default NULL,
  `start_date` date default NULL,
  `end_date` date default NULL,
  `entered_on` date default NULL,
  `entered_by` bigint(20) unsigned default NULL,
  `supervisor_cellphone_number` varchar(35) default NULL,
  PRIMARY KEY  (`history_id`),
  KEY `employer_id` (`employer_id`),
  KEY `job_id` (`job_id`),
  KEY `company_department_id` (`company_department_id`),
  KEY `student_number` (`student_number`),
  KEY `term_id` (`term_id`),
  KEY `year` (`year`),
  KEY `department_id` (`department_id`),
  KEY `work_term_number` (`work_term_number`),
  KEY `supervisor_name` (`supervisor_name`),
  KEY `temp_supervisor_contact` (`temp_supervisor_contact`),
  KEY `temp_job_contact` (`temp_job_contact`),
  KEY `temp_date_entered` (`temp_date_entered`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `history`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `history_container`
-- 

DROP TABLE IF EXISTS `history_container`;
CREATE TABLE IF NOT EXISTS `history_container` (
  `history_id` int(11) default NULL,
  `workterm_code` char(3) default NULL,
  `department_id` smallint(5) default NULL,
  `student_number` varchar(10) default NULL,
  KEY `history_id` (`history_id`),
  KEY `department_id` (`department_id`),
  KEY `student_number` (`student_number`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `history_container`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `history_flags`
-- 

DROP TABLE IF EXISTS `history_flags`;
CREATE TABLE IF NOT EXISTS `history_flags` (
  `history_flags_id` int(11) NOT NULL auto_increment,
  `description` varchar(50) default NULL,
  `order_by` smallint(5) unsigned default NULL,
  `comment` tinytext,
  PRIMARY KEY  (`history_flags_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `history_flags`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `history_flags_join`
-- 

DROP TABLE IF EXISTS `history_flags_join`;
CREATE TABLE IF NOT EXISTS `history_flags_join` (
  `history_id` int(11) default NULL,
  `department_id` smallint(5) unsigned default NULL,
  `history_flags_id` int(11) default NULL,
  KEY `history_flags_id` (`history_flags_id`),
  KEY `history_id` (`history_id`),
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `history_flags_join`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `history_notes`
-- 

DROP TABLE IF EXISTS `history_notes`;
CREATE TABLE IF NOT EXISTS `history_notes` (
  `history_notes_id` mediumint(8) unsigned NOT NULL auto_increment,
  `notes` text,
  `history_id` int(11) default NULL,
  `date_entered` datetime default NULL,
  `contact_id` bigint(20) unsigned default NULL,
  PRIMARY KEY  (`history_notes_id`),
  KEY `history_id` (`history_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `history_notes`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `history_options`
-- 

DROP TABLE IF EXISTS `history_options`;
CREATE TABLE IF NOT EXISTS `history_options` (
  `history_options_id` tinyint(4) unsigned NOT NULL auto_increment,
  `history_options_name` varchar(50) default NULL,
  PRIMARY KEY  (`history_options_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `history_options`
-- 

INSERT INTO `history_options` VALUES (1, 'TBC');
INSERT INTO `history_options` VALUES (2, 'Job Description');
INSERT INTO `history_options` VALUES (3, 'Verbal Confirm');
INSERT INTO `history_options` VALUES (4, 'Letter Confirm');

-- --------------------------------------------------------

-- 
-- Table structure for table `history_report_subject`
-- 

DROP TABLE IF EXISTS `history_report_subject`;
CREATE TABLE IF NOT EXISTS `history_report_subject` (
  `report_subject_id` tinyint(4) unsigned NOT NULL auto_increment,
  `report_subject_name` varchar(100) default NULL,
  `department_id` smallint(5) unsigned default NULL,
  `comment` tinytext,
  PRIMARY KEY  (`report_subject_id`),
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `history_report_subject`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `history_status`
-- 

DROP TABLE IF EXISTS `history_status`;
CREATE TABLE IF NOT EXISTS `history_status` (
  `history_status_id` tinyint(4) unsigned NOT NULL auto_increment,
  `history_status_name` varchar(100) default NULL,
  PRIMARY KEY  (`history_status_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `history_status`
-- 

INSERT INTO `history_status` VALUES (1, 'INP');
INSERT INTO `history_status` VALUES (2, 'COM');
INSERT INTO `history_status` VALUES (3, 'DEF');
INSERT INTO `history_status` VALUES (4, 'N');
INSERT INTO `history_status` VALUES (5, 'F');
INSERT INTO `history_status` VALUES (6, 'WNF');
INSERT INTO `history_status` VALUES (7, 'Not Counted');
INSERT INTO `history_status` VALUES (8, 'A+');
INSERT INTO `history_status` VALUES (9, 'A');
INSERT INTO `history_status` VALUES (10, 'A-');
INSERT INTO `history_status` VALUES (11, 'B+');
INSERT INTO `history_status` VALUES (12, 'B');
INSERT INTO `history_status` VALUES (13, 'B-');
INSERT INTO `history_status` VALUES (14, 'C+');
INSERT INTO `history_status` VALUES (15, 'C');
INSERT INTO `history_status` VALUES (16, 'C-');
INSERT INTO `history_status` VALUES (17, 'D');
INSERT INTO `history_status` VALUES (18, 'REV');
INSERT INTO `history_status` VALUES (19, 'SUB');

-- --------------------------------------------------------

-- 
-- Table structure for table `industries`
-- 

DROP TABLE IF EXISTS `industries`;
CREATE TABLE IF NOT EXISTS `industries` (
  `industry_id` mediumint(8) unsigned NOT NULL auto_increment,
  `industry_name` varchar(100) default NULL,
  `order_by` smallint(5) unsigned default '0',
  PRIMARY KEY  (`industry_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `industries`
-- 

INSERT INTO `industries` VALUES (1, 'Manufacturing - Food and Beverage', 10);
INSERT INTO `industries` VALUES (2, 'Manufacturing - Textiles/Clothing', 20);
INSERT INTO `industries` VALUES (3, 'Wood Products (Pulp and Paper/Printing)', 30);
INSERT INTO `industries` VALUES (4, 'Petroleum and Coal products', 40);
INSERT INTO `industries` VALUES (5, 'Chemical/Pharmaceuticals', 50);
INSERT INTO `industries` VALUES (6, 'Manufacturing - Metals (Machine Shops)', 60);
INSERT INTO `industries` VALUES (7, 'Automotive', 80);
INSERT INTO `industries` VALUES (8, 'Aerospace', 90);
INSERT INTO `industries` VALUES (9, 'Marine', 100);
INSERT INTO `industries` VALUES (10, 'Publishing', 110);
INSERT INTO `industries` VALUES (11, 'Audio/Video', 120);
INSERT INTO `industries` VALUES (12, 'Broadcasting/Telecommunications', 130);
INSERT INTO `industries` VALUES (13, 'Information Services and Data Processing', 140);
INSERT INTO `industries` VALUES (14, 'Software', 150);
INSERT INTO `industries` VALUES (15, 'Performing Arts', 160);
INSERT INTO `industries` VALUES (16, 'Sports and Recreation', 170);
INSERT INTO `industries` VALUES (17, 'Heritage Institutions (Museums, Galleries)', 180);
INSERT INTO `industries` VALUES (18, 'Agriculture', 240);
INSERT INTO `industries` VALUES (19, 'Forestry', 250);
INSERT INTO `industries` VALUES (20, 'Oil and Gas', 270);
INSERT INTO `industries` VALUES (21, 'Other Natural Resources', 280);
INSERT INTO `industries` VALUES (22, 'Utilities', 290);
INSERT INTO `industries` VALUES (23, 'Construction', 300);
INSERT INTO `industries` VALUES (24, 'Wholesale Trade', 310);
INSERT INTO `industries` VALUES (25, 'Retail Trade', 320);
INSERT INTO `industries` VALUES (26, 'Transportation Services', 330);
INSERT INTO `industries` VALUES (27, 'Finance and Insurance', 340);
INSERT INTO `industries` VALUES (28, 'Real Estate and Rental/Leasing', 350);
INSERT INTO `industries` VALUES (29, 'Educational Services', 390);
INSERT INTO `industries` VALUES (30, 'Health Care and Social Assistance', 400);
INSERT INTO `industries` VALUES (31, 'Manufacturing - Computers and Electronics', 70);
INSERT INTO `industries` VALUES (32, 'Tourism', 190);
INSERT INTO `industries` VALUES (33, 'Government - Federal', 200);
INSERT INTO `industries` VALUES (34, 'Government - Provincial', 210);
INSERT INTO `industries` VALUES (35, 'Government - Municipal', 220);
INSERT INTO `industries` VALUES (36, 'Non Government Organization (NGO)', 230);
INSERT INTO `industries` VALUES (37, 'Mining', 260);
INSERT INTO `industries` VALUES (38, 'Consulting (Professional, Scientific, and Technical Services)', 360);
INSERT INTO `industries` VALUES (39, 'Management of Companies and Enterprises', 370);
INSERT INTO `industries` VALUES (40, 'Administrative and Support Services', 380);
INSERT INTO `industries` VALUES (41, 'Environment and Remediation Services', 410);
INSERT INTO `industries` VALUES (42, 'Manufacturing - Other', 420);
INSERT INTO `industries` VALUES (43, 'Government - Other', 430);
INSERT INTO `industries` VALUES (44, 'Other Services', 440);
INSERT INTO `industries` VALUES (45, '?', 10000);
INSERT INTO `industries` VALUES (46, 'Accommodation & Food Services', 10);

-- --------------------------------------------------------

-- 
-- Table structure for table `installed_components`
-- 

DROP TABLE IF EXISTS `installed_components`;
CREATE TABLE IF NOT EXISTS `installed_components` (
  `description` varchar(20) default NULL,
  `installed` char(1) default NULL
) ENGINE=MyISAM;

-- 
-- Dumping data for table `installed_components`
-- 

INSERT INTO `installed_components` VALUES ('job_develop_report', '0');
INSERT INTO `installed_components` VALUES ('job_posting_report', '0');
INSERT INTO `installed_components` VALUES ('placement_report', '0');
INSERT INTO `installed_components` VALUES ('student_apps_report', '0');

-- --------------------------------------------------------

-- 
-- Table structure for table `interview`
-- 

DROP TABLE IF EXISTS `interview`;
CREATE TABLE IF NOT EXISTS `interview` (
  `job_id` mediumint(8) unsigned NOT NULL default '0',
  `interview_type_id` tinyint(8) unsigned NOT NULL default '1',
  `interview_place_id` tinyint(8) unsigned NOT NULL default '1',
  `interview_medium_id` tinyint(8) unsigned NOT NULL default '1',
  `notes` blob,
  `presentation` tinyint(3) unsigned NOT NULL default '0',
  `date_created` datetime NOT NULL default '0000-00-00 00:00:00',
  `creator` varchar(8) default NULL,
  `cancelled` tinyint(3) unsigned NOT NULL default '0',
  `contact` bigint(20) unsigned NOT NULL default '0',
  `student_visible` tinyint(3) unsigned NOT NULL default '0',
  `emailed_employer` tinyint(3) unsigned NOT NULL default '0',
  `emailed_newsgroup` tinyint(3) unsigned NOT NULL default '0',
  `emailed_secretary` tinyint(3) unsigned NOT NULL default '0',
  `staff_notes` blob,
  `use_room_phone_number` tinyint(3) unsigned default '0',
  PRIMARY KEY  (`job_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `interview`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `interview_edit`
-- 

DROP TABLE IF EXISTS `interview_edit`;
CREATE TABLE IF NOT EXISTS `interview_edit` (
  `job_id` mediumint(8) unsigned NOT NULL default '0',
  `date_edited` timestamp NOT NULL,
  `editor` varchar(8) NOT NULL default '',
  `menu_edited` enum('Job Info','Date/Time','Students Shortlisted','Location','Button') default NULL,
  KEY `interview_edit_indx` (`job_id`,`menu_edited`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `interview_edit`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `interview_interviewer`
-- 

DROP TABLE IF EXISTS `interview_interviewer`;
CREATE TABLE IF NOT EXISTS `interview_interviewer` (
  `interviewer_id` mediumint(8) unsigned NOT NULL auto_increment,
  `interviewer_name` varchar(100) default NULL,
  PRIMARY KEY  (`interviewer_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `interview_interviewer`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `interview_job_join`
-- 

DROP TABLE IF EXISTS `interview_job_join`;
CREATE TABLE IF NOT EXISTS `interview_job_join` (
  `interview_job_id` mediumint(8) unsigned default NULL,
  `job_id` mediumint(8) unsigned default NULL,
  `term_id` smallint(5) default NULL,
  `year` year(4) default NULL,
  `job_code` varchar(120) default NULL,
  `filled` tinyint(4) default NULL,
  KEY `interview_job_id` (`interview_job_id`),
  KEY `job_id` (`job_id`),
  KEY `term_id` (`term_id`),
  KEY `year` (`year`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `interview_job_join`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `interview_location`
-- 

DROP TABLE IF EXISTS `interview_location`;
CREATE TABLE IF NOT EXISTS `interview_location` (
  `job_id` mediumint(8) unsigned NOT NULL default '0',
  `street_address_1` varchar(75) NOT NULL default '',
  `street_address_2` varchar(75) default NULL,
  `city` varchar(40) NOT NULL default '',
  `province` varchar(40) NOT NULL default '',
  `country` varchar(40) default NULL,
  `directions` text,
  PRIMARY KEY  (`job_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `interview_location`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `interview_medium`
-- 

DROP TABLE IF EXISTS `interview_medium`;
CREATE TABLE IF NOT EXISTS `interview_medium` (
  `interview_medium_id` tinyint(8) unsigned NOT NULL auto_increment,
  `description` varchar(50) NOT NULL default '',
  PRIMARY KEY  (`interview_medium_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `interview_medium`
-- 

INSERT INTO `interview_medium` VALUES (1, 'In-person');
INSERT INTO `interview_medium` VALUES (2, 'Phone');
INSERT INTO `interview_medium` VALUES (3, 'Video Conference');
INSERT INTO `interview_medium` VALUES (4, 'Other');

-- --------------------------------------------------------

-- 
-- Table structure for table `interview_phone`
-- 

DROP TABLE IF EXISTS `interview_phone`;
CREATE TABLE IF NOT EXISTS `interview_phone` (
  `time_id` mediumint(8) unsigned NOT NULL default '0',
  `phone` varchar(25) NOT NULL default '',
  PRIMARY KEY  (`time_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `interview_phone`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `interview_place`
-- 

DROP TABLE IF EXISTS `interview_place`;
CREATE TABLE IF NOT EXISTS `interview_place` (
  `interview_place_id` tinyint(8) unsigned NOT NULL auto_increment,
  `description` varchar(50) NOT NULL default '',
  PRIMARY KEY  (`interview_place_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `interview_place`
-- 

INSERT INTO `interview_place` VALUES (1, 'On Campus');
INSERT INTO `interview_place` VALUES (2, 'Off Campus');

-- --------------------------------------------------------

-- 
-- Table structure for table `interview_time`
-- 

DROP TABLE IF EXISTS `interview_time`;
CREATE TABLE IF NOT EXISTS `interview_time` (
  `time_id` mediumint(8) unsigned NOT NULL auto_increment,
  `job_id` mediumint(8) unsigned NOT NULL default '0',
  `int_date` date NOT NULL default '0000-00-00',
  `int_time` time NOT NULL default '00:00:00',
  `end_time` time NOT NULL default '00:00:00',
  `interviewer_id` mediumint(8) unsigned NOT NULL default '0',
  `switch` tinyint(3) unsigned NOT NULL default '0',
  `student_number` varchar(10) default NULL,
  `int_type` tinyint(3) unsigned NOT NULL default '0',
  `int_room_id` smallint(5) unsigned default NULL,
  `int_wait_room_id` smallint(5) unsigned default NULL,
  `sequential` tinyint(3) unsigned NOT NULL default '0',
  PRIMARY KEY  (`time_id`),
  KEY `int_date` (`int_date`),
  KEY `job_id` (`job_id`),
  KEY `student_number` (`student_number`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `interview_time`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `interview_type`
-- 

DROP TABLE IF EXISTS `interview_type`;
CREATE TABLE IF NOT EXISTS `interview_type` (
  `interview_type_id` tinyint(8) unsigned NOT NULL auto_increment,
  `description` varchar(50) NOT NULL default '',
  PRIMARY KEY  (`interview_type_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `interview_type`
-- 

INSERT INTO `interview_type` VALUES (1, 'Standard');
INSERT INTO `interview_type` VALUES (2, 'Switching');
INSERT INTO `interview_type` VALUES (3, 'Simultaneous');
INSERT INTO `interview_type` VALUES (4, 'Sequential');

-- --------------------------------------------------------

-- 
-- Table structure for table `job_discipline_join`
-- 

DROP TABLE IF EXISTS `job_discipline_join`;
CREATE TABLE IF NOT EXISTS `job_discipline_join` (
  `job_id` mediumint(8) unsigned default NULL,
  `discipline_id` mediumint(8) unsigned default NULL,
  KEY `job_id` (`job_id`),
  KEY `discipline_id` (`discipline_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `job_discipline_join`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `job_hiring_status`
-- 

DROP TABLE IF EXISTS `job_hiring_status`;
CREATE TABLE IF NOT EXISTS `job_hiring_status` (
  `status_id` mediumint(8) unsigned NOT NULL auto_increment,
  `status_name` varchar(75) default NULL,
  PRIMARY KEY  (`status_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `job_hiring_status`
-- 

INSERT INTO `job_hiring_status` VALUES (1, 'Pending');
INSERT INTO `job_hiring_status` VALUES (2, 'UVic');
INSERT INTO `job_hiring_status` VALUES (3, 'Elsewhere');
INSERT INTO `job_hiring_status` VALUES (4, 'Not Hiring');

-- --------------------------------------------------------

-- 
-- Table structure for table `job_info`
-- 

DROP TABLE IF EXISTS `job_info`;
CREATE TABLE IF NOT EXISTS `job_info` (
  `job_id` mediumint(8) unsigned NOT NULL auto_increment,
  `employer_id` mediumint(8) unsigned NOT NULL default '0',
  `department_id` smallint(5) unsigned NOT NULL default '0',
  `job_code` varchar(120) default NULL,
  `term_id` smallint(5) NOT NULL default '0',
  `year` year(4) NOT NULL default '0000',
  `closing_date` date default NULL,
  `position_title` tinytext NOT NULL,
  `status` tinyint(3) unsigned default '7',
  `num_positions` tinytext NOT NULL,
  `apply_method` tinytext NOT NULL,
  `receive_address` tinytext,
  `country_id` mediumint(8) unsigned NOT NULL default '0',
  `provstate_id` mediumint(8) unsigned default NULL,
  `region_id` mediumint(8) unsigned default NULL,
  `city` tinytext NOT NULL,
  `salary_amount1` varchar(25) default NULL,
  `salary_period` tinytext,
  `closing_time` time default NULL,
  `workterm_lengths` tinytext NOT NULL,
  `other_postings` text NOT NULL,
  `job_description` text NOT NULL,
  `contact_id` bigint(20) unsigned NOT NULL default '0',
  `min_academic` tinyint(3) unsigned NOT NULL default '0',
  `min_workterms` tinyint(3) unsigned NOT NULL default '0',
  `special_requirements` text NOT NULL,
  `skills_required` text NOT NULL,
  `comments` text NOT NULL,
  `date_added` date NOT NULL default '0000-00-00',
  `time_added` time NOT NULL default '00:00:00',
  `added_by` tinytext,
  `last_updated_on` date NOT NULL default '0000-00-00',
  `last_updated_time` time NOT NULL default '00:00:00',
  `last_updated_by` tinytext,
  `salary_amount2` varchar(25) default NULL,
  `workterm_hours` varchar(20) default NULL,
  `student_status` tinyint(3) unsigned NOT NULL default '0',
  `admin_comments` text NOT NULL,
  `resumes_pulled` tinyint(3) unsigned default '0',
  `employer_code` varchar(40) default NULL,
  `employer_department_id` mediumint(8) unsigned default NULL,
  `industry` mediumint(8) unsigned default NULL,
  `internal_contact_id` bigint(20) unsigned default NULL,
  `is_rejectable` tinyint(3) unsigned default '0',
  `filled` char(1) NOT NULL default '0',
  `interview_term_id` smallint(6) NOT NULL default '0',
  `interview_year` year(4) NOT NULL default '0000',
  `hiring_status` mediumint(8) unsigned NOT NULL default '1',
  `admin_status` smallint(5) unsigned default NULL,
  `special_instructions` text NOT NULL,
  `special_instr_button` varchar(40) default NULL,
  `show_special_instr_button` tinyint(1) unsigned default '1',
  `employer_status` tinyint(5) unsigned default NULL,
  `interview_notify_sent` tinyint(1) unsigned default '0',
  `hold_begin_datetime` datetime default NULL,
  `hold_end_datetime` datetime default NULL,
  `date_entered` date default NULL,
  `entered_by` bigint(20) unsigned default NULL,
  `date_posted` date default NULL,
  `temp_date_entered` date default NULL,
  `start_date` date default NULL,
  `end_date` date default NULL,
  `displayname_on_coverltr` tinyint(1) unsigned default '0',
  `for_student_comments` text,
  PRIMARY KEY  (`job_id`),
  KEY `employer_id` (`employer_id`),
  KEY `job_code` (`job_code`),
  KEY `department_id` (`department_id`),
  KEY `term_id` (`term_id`),
  KEY `year` (`year`),
  KEY `closing_date` (`closing_date`),
  KEY `employer_department_id` (`employer_department_id`),
  KEY `status` (`status`),
  KEY `country_id` (`country_id`),
  KEY `provstate_id` (`provstate_id`),
  KEY `region_id` (`region_id`),
  KEY `contact_id` (`contact_id`),
  KEY `closing_time` (`closing_time`),
  KEY `industry` (`industry`),
  KEY `min_academic` (`min_academic`),
  KEY `last_updated_on` (`last_updated_on`),
  KEY `date_posted` (`date_posted`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `job_info`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `job_status`
-- 

DROP TABLE IF EXISTS `job_status`;
CREATE TABLE IF NOT EXISTS `job_status` (
  `status_id` tinyint(3) unsigned NOT NULL auto_increment,
  `status_name` varchar(50) default NULL,
  PRIMARY KEY  (`status_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `job_status`
-- 

INSERT INTO `job_status` VALUES (1, 'Not Checked');
INSERT INTO `job_status` VALUES (2, 'Checked');
INSERT INTO `job_status` VALUES (3, 'Posted');
INSERT INTO `job_status` VALUES (4, 'Closed');
INSERT INTO `job_status` VALUES (5, 'Cancelled');
INSERT INTO `job_status` VALUES (6, 'Rejected');
INSERT INTO `job_status` VALUES (7, 'Interview Only');
INSERT INTO `job_status` VALUES (8, 'Pending Posting');

-- --------------------------------------------------------

-- 
-- Table structure for table `login_info`
-- 

DROP TABLE IF EXISTS `login_info`;
CREATE TABLE IF NOT EXISTS `login_info` (
  `login_id` varchar(20) default NULL,
  `contact_id` bigint(20) unsigned default NULL,
  `student_number` varchar(10) default NULL,
  `last_datetime_in` datetime default NULL,
  `num_logins` mediumint(8) unsigned default NULL,
  `last_browser` tinyint(3) unsigned default NULL,
  `last_platform` tinyint(3) unsigned default NULL,
  `last_IP_address` varchar(50) default NULL
) ENGINE=MyISAM;

-- 
-- Dumping data for table `login_info`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `mailed_new_password_log`
-- 

DROP TABLE IF EXISTS `mailed_new_password_log`;
CREATE TABLE IF NOT EXISTS `mailed_new_password_log` (
  `IP_logged` varchar(50) default NULL,
  `datetime_mailed` datetime default NULL,
  `contact_id` bigint(20) unsigned default NULL,
  `employer_id` mediumint(8) unsigned default NULL,
  `email` varchar(100) default NULL
) ENGINE=MyISAM;

-- 
-- Dumping data for table `mailed_new_password_log`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `menu_item`
-- 

DROP TABLE IF EXISTS `menu_item`;
CREATE TABLE IF NOT EXISTS `menu_item` (
  `menu_id` smallint(5) unsigned NOT NULL auto_increment,
  `menu_name` varchar(30) NOT NULL default '',
  `module_id` smallint(5) unsigned NOT NULL default '0',
  `menu_owner` tinyint(3) unsigned NOT NULL default '0',
  `order_by` smallint(5) unsigned default NULL,
  PRIMARY KEY  (`menu_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `menu_item`
-- 

INSERT INTO `menu_item` VALUES (6, 'Employer Info', 3, 9, 30);
INSERT INTO `menu_item` VALUES (7, 'Employer Info', 3, 4, 30);
INSERT INTO `menu_item` VALUES (31, 'Help', 1, 1, 90);
INSERT INTO `menu_item` VALUES (54, 'Preferences', 20, 4, 95);
INSERT INTO `menu_item` VALUES (126, 'Getting Started', 3, 6, 5);
INSERT INTO `menu_item` VALUES (50, 'Logout', 19, 4, 130);
INSERT INTO `menu_item` VALUES (51, 'Logout', 19, 5, 80);
INSERT INTO `menu_item` VALUES (53, 'Logout', 19, 9, 130);
INSERT INTO `menu_item` VALUES (110, 'Employer Info', 3, 88, 10);
INSERT INTO `menu_item` VALUES (113, 'Logout', 19, 88, 100);
INSERT INTO `menu_item` VALUES (100, 'Student Info', 40, 4, 20);
INSERT INTO `menu_item` VALUES (112, 'Interviews', 10, 88, 75);
INSERT INTO `menu_item` VALUES (1, 'Interviews', 10, 4, 75);
INSERT INTO `menu_item` VALUES (2, 'Interviews', 10, 9, 30);
INSERT INTO `menu_item` VALUES (10, 'Job Descriptions', 5, 9, 50);
INSERT INTO `menu_item` VALUES (11, 'Job Descriptions', 5, 4, 50);
INSERT INTO `menu_item` VALUES (33, 'Edit a Job', 5, 111, 20);
INSERT INTO `menu_item` VALUES (34, 'View a Job', 5, 5, 10);
INSERT INTO `menu_item` VALUES (35, 'Add a Job', 5, 5, 25);
INSERT INTO `menu_item` VALUES (111, 'Job Descriptions', 5, 88, 20);
INSERT INTO `menu_item` VALUES (3, 'Sign Up for an Interview', 10, 1, 50);
INSERT INTO `menu_item` VALUES (15, 'Job Applications', 6, 4, 70);
INSERT INTO `menu_item` VALUES (49, 'Logout', 19, 1, 900);
INSERT INTO `menu_item` VALUES (101, 'Resumes', 6, 1, 20);
INSERT INTO `menu_item` VALUES (102, 'Cover Letters', 6, 1, 30);
INSERT INTO `menu_item` VALUES (103, 'View Transcript', 6, 1, 40);
INSERT INTO `menu_item` VALUES (104, 'Job Descriptions', 5, 1, 10);
INSERT INTO `menu_item` VALUES (108, 'Job Applications', 6, 1, 15);
INSERT INTO `menu_item` VALUES (109, 'Other', 41, 4, 100);
INSERT INTO `menu_item` VALUES (16, 'Job Offers', 12, 4, 80);
INSERT INTO `menu_item` VALUES (17, 'Job Offers', 12, 9, 80);
INSERT INTO `menu_item` VALUES (105, 'Student Applications', 6, 6, 35);
INSERT INTO `menu_item` VALUES (106, 'Change Password', 3, 6, 75);
INSERT INTO `menu_item` VALUES (107, 'Logout', 19, 6, 80);
INSERT INTO `menu_item` VALUES (115, 'View a Job', 5, 6, 10);
INSERT INTO `menu_item` VALUES (116, 'Add a Job', 5, 6, 20);
INSERT INTO `menu_item` VALUES (117, 'Edit a Job', 5, 6, 25);
INSERT INTO `menu_item` VALUES (118, 'Other', 41, 88, 93);
INSERT INTO `menu_item` VALUES (125, 'Statistics', 14, 4, 90);
INSERT INTO `menu_item` VALUES (124, 'Preferences', 20, 88, 90);
INSERT INTO `menu_item` VALUES (127, 'Feedback', 15, 4, 120);
INSERT INTO `menu_item` VALUES (128, 'Feedback', 15, 1, 100);
INSERT INTO `menu_item` VALUES (129, 'Feedback', 15, 88, 98);

-- --------------------------------------------------------

-- 
-- Table structure for table `menu_sub_item`
-- 

DROP TABLE IF EXISTS `menu_sub_item`;
CREATE TABLE IF NOT EXISTS `menu_sub_item` (
  `menu_id` smallint(5) unsigned NOT NULL default '0',
  `menu_name` varchar(30) default NULL,
  `select_var` varchar(50) default NULL,
  `sub_item_id` smallint(5) unsigned NOT NULL auto_increment,
  `order_by` smallint(5) unsigned default NULL,
  PRIMARY KEY  (`sub_item_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `menu_sub_item`
-- 

INSERT INTO `menu_sub_item` VALUES (12, 'Preference Form', 'select=pref&page=search', 12, 40);
INSERT INTO `menu_sub_item` VALUES (13, 'Preference Form', 'select=pref&page=search', 32, 40);
INSERT INTO `menu_sub_item` VALUES (31, 'Help', 'select=help', 55, 10);
INSERT INTO `menu_sub_item` VALUES (125, 'Job Summary', 'select=job_summary', 225, 10);
INSERT INTO `menu_sub_item` VALUES (54, 'Personal', 'select=prefer', 81, 10);
INSERT INTO `menu_sub_item` VALUES (54, 'Department', 'select=prefer_dept', 82, 20);
INSERT INTO `menu_sub_item` VALUES (7, 'Set Actions/Flags', 'select=set_contact_flag', 157, 40);
INSERT INTO `menu_sub_item` VALUES (7, 'Edit', 'select=edit_contact', 156, 30);
INSERT INTO `menu_sub_item` VALUES (7, 'Add', 'select=add_contact', 155, 20);
INSERT INTO `menu_sub_item` VALUES (7, 'View/Search', 'select=view_contact', 154, 10);
INSERT INTO `menu_sub_item` VALUES (110, 'Set Actions/Flags', 'select=set_contact_flag', 195, 40);
INSERT INTO `menu_sub_item` VALUES (110, 'Edit', 'select=edit_contact', 196, 30);
INSERT INTO `menu_sub_item` VALUES (110, 'Add', 'select=add_contact', 197, 20);
INSERT INTO `menu_sub_item` VALUES (110, 'View/Search', 'select=view_contact', 198, 10);
INSERT INTO `menu_sub_item` VALUES (100, 'View/Search', 'select=view_student', 148, 10);
INSERT INTO `menu_sub_item` VALUES (100, 'Add', 'select=add_student', 149, 20);
INSERT INTO `menu_sub_item` VALUES (100, 'Edit', 'select=edit_student', 150, 30);
INSERT INTO `menu_sub_item` VALUES (100, 'Student Flags', 'select=set_student_flags', 215, 35);
INSERT INTO `menu_sub_item` VALUES (1, 'View Interviews', 'select=view', 1, 10);
INSERT INTO `menu_sub_item` VALUES (1, 'Add an Interview', 'select=add', 2, 20);
INSERT INTO `menu_sub_item` VALUES (1, 'Edit an Interview', 'select=edit', 3, 30);
INSERT INTO `menu_sub_item` VALUES (1, 'Modify Sign-up', 'select=sign_up', 4, 40);
INSERT INTO `menu_sub_item` VALUES (1, 'Lunch Schedule', 'select=lunchall', 80, 50);
INSERT INTO `menu_sub_item` VALUES (2, 'View Interviews', 'select=view', 21, 10);
INSERT INTO `menu_sub_item` VALUES (2, 'Add an Interview', 'select=add', 22, 20);
INSERT INTO `menu_sub_item` VALUES (2, 'Edit an Interview', 'select=edit', 23, 30);
INSERT INTO `menu_sub_item` VALUES (2, 'Modify Sign-up', 'select=sign_up', 24, 40);
INSERT INTO `menu_sub_item` VALUES (112, 'Lunch Schedule', 'select=lunchall', 190, 20);
INSERT INTO `menu_sub_item` VALUES (112, 'View Interviews', 'select=view', 189, 10);
INSERT INTO `menu_sub_item` VALUES (100, 'Placement History', 'select=history_choose', 151, 40);
INSERT INTO `menu_sub_item` VALUES (7, 'Hiring History', 'select=employer_history', 185, 35);
INSERT INTO `menu_sub_item` VALUES (11, 'Search', 'select=search_job', 162, 40);
INSERT INTO `menu_sub_item` VALUES (11, 'Edit', 'select=edit_job', 161, 30);
INSERT INTO `menu_sub_item` VALUES (11, 'Add', 'select=add_job', 160, 20);
INSERT INTO `menu_sub_item` VALUES (11, 'View', 'select=view_job', 159, 10);
INSERT INTO `menu_sub_item` VALUES (10, 'View Job Descriptions', 'select=view_job', 25, 10);
INSERT INTO `menu_sub_item` VALUES (10, 'Add Job Description', 'select=add_job', 26, 20);
INSERT INTO `menu_sub_item` VALUES (10, 'Edit Job Description', 'select=edit_job', 27, 30);
INSERT INTO `menu_sub_item` VALUES (10, 'Search Jobs', 'select=search_job', 28, 40);
INSERT INTO `menu_sub_item` VALUES (33, 'Edit a Job', 'select=edit_job', 56, 20);
INSERT INTO `menu_sub_item` VALUES (32, 'Add a Job', 'select=add_job', 57, 10);
INSERT INTO `menu_sub_item` VALUES (34, 'View a Job', 'select=view_job', 58, 10);
INSERT INTO `menu_sub_item` VALUES (35, 'Add a Job', 'select=add_job', 59, 10);
INSERT INTO `menu_sub_item` VALUES (111, 'Search', 'select=search_job', 191, 40);
INSERT INTO `menu_sub_item` VALUES (111, 'Edit', 'select=edit_job', 192, 30);
INSERT INTO `menu_sub_item` VALUES (111, 'Add', 'select=add_job', 193, 20);
INSERT INTO `menu_sub_item` VALUES (111, 'View', 'select=view_job', 194, 10);
INSERT INTO `menu_sub_item` VALUES (111, 'Incoming Jobs', 'select=pending_jobs', 200, 50);
INSERT INTO `menu_sub_item` VALUES (3, 'Sign-up for an Interview', 'select=student_sign_up', 41, 10);
INSERT INTO `menu_sub_item` VALUES (100, 'Resumes', 'select=resume&page=view', 152, 60);
INSERT INTO `menu_sub_item` VALUES (100, 'Transcripts', 'select=transcript&page=view', 153, 70);
INSERT INTO `menu_sub_item` VALUES (100, 'Cover Letters', 'select=coverletter&page=view', 179, 65);
INSERT INTO `menu_sub_item` VALUES (101, 'View', 'select=resume&page=view', 168, 10);
INSERT INTO `menu_sub_item` VALUES (101, 'Add', 'select=resume&page=add', 169, 20);
INSERT INTO `menu_sub_item` VALUES (101, 'Edit', 'select=resume&page=edit', 170, 30);
INSERT INTO `menu_sub_item` VALUES (101, 'Delete', 'select=resume&page=delete', 171, 40);
INSERT INTO `menu_sub_item` VALUES (102, 'View/Edit', 'select=coverletter&page=view', 172, 10);
INSERT INTO `menu_sub_item` VALUES (103, 'View Transcript', 'select=transcript', 176, 10);
INSERT INTO `menu_sub_item` VALUES (104, 'View', 'select=view_job', 177, 10);
INSERT INTO `menu_sub_item` VALUES (104, 'Search', 'select=search_job', 178, 20);
INSERT INTO `menu_sub_item` VALUES (108, 'View', 'select=applications&option=view', 184, 10);
INSERT INTO `menu_sub_item` VALUES (108, 'Edit/Delete', 'select=applications&option=edit_delete', 210, 20);
INSERT INTO `menu_sub_item` VALUES (102, 'Add', 'select=coverletter&page=add', 212, 20);
INSERT INTO `menu_sub_item` VALUES (109, 'Student Login', 'select=student_login', 187, 10);
INSERT INTO `menu_sub_item` VALUES (109, 'Room Editing', 'select=room_booking', 186, 10);
INSERT INTO `menu_sub_item` VALUES (19, 'Placement Summary', 'select=placement_summary', 224, 20);
INSERT INTO `menu_sub_item` VALUES (15, 'View/Send', 'select=applications&amp;option=view', 164, 10);
INSERT INTO `menu_sub_item` VALUES (16, 'Place Students', 'select=placement_by_term', 167, 10);
INSERT INTO `menu_sub_item` VALUES (17, 'Shortlisting', '', 36, 10);
INSERT INTO `menu_sub_item` VALUES (17, 'Rankings', '', 37, 20);
INSERT INTO `menu_sub_item` VALUES (105, 'View Applicants', 'select=applications', 181, 10);
INSERT INTO `menu_sub_item` VALUES (16, 'Own Job Placement', 'select=add_history', 180, 20);
INSERT INTO `menu_sub_item` VALUES (106, 'Change Password', 'select=change_login_password', 182, 10);
INSERT INTO `menu_sub_item` VALUES (107, 'Logout', 'select=logout', 183, 10);
INSERT INTO `menu_sub_item` VALUES (109, 'Employer Login', 'select=employer_login', 188, 10);
INSERT INTO `menu_sub_item` VALUES (115, 'View a Job', 'select=view_job', 204, 10);
INSERT INTO `menu_sub_item` VALUES (116, 'Add a Job', 'select=add_job', 205, 10);
INSERT INTO `menu_sub_item` VALUES (117, 'Edit a Job', 'select=edit_job', 206, 10);
INSERT INTO `menu_sub_item` VALUES (118, 'Employer Login', 'select=employer_login', 209, 10);
INSERT INTO `menu_sub_item` VALUES (19, 'Job Summary', 'select=job_summary', 223, 10);
INSERT INTO `menu_sub_item` VALUES (16, 'Returning Placement', 'select=returning_placement', 216, 20);
INSERT INTO `menu_sub_item` VALUES (7, 'Send/Create Login Info', 'select=make_login_info', 158, 50);
INSERT INTO `menu_sub_item` VALUES (110, 'Send/Create Login Info', 'select=make_login_info', 199, 50);
INSERT INTO `menu_sub_item` VALUES (111, 'Holding Jobs', 'select=holding_jobs', 201, 60);
INSERT INTO `menu_sub_item` VALUES (11, 'Holding Jobs', 'select=holding_jobs', 202, 50);
INSERT INTO `menu_sub_item` VALUES (118, 'Room Editing', 'select=room_booking', 207, 10);
INSERT INTO `menu_sub_item` VALUES (118, 'Student Login', 'select=student_login', 208, 10);
INSERT INTO `menu_sub_item` VALUES (124, 'Personal', 'select=prefer', 220, 10);
INSERT INTO `menu_sub_item` VALUES (54, 'Search', 'select=manage_searches', 221, 30);
INSERT INTO `menu_sub_item` VALUES (124, 'Search', 'select=manage_searches', 222, 30);
INSERT INTO `menu_sub_item` VALUES (125, 'Placement Summary', 'select=placement_summary', 226, 20);
INSERT INTO `menu_sub_item` VALUES (126, 'Getting Started', 'select=getting_started', 227, 5);

-- --------------------------------------------------------

-- 
-- Table structure for table `menus_being_used`
-- 

DROP TABLE IF EXISTS `menus_being_used`;
CREATE TABLE IF NOT EXISTS `menus_being_used` (
  `menus_being_used_id` mediumint(8) unsigned NOT NULL auto_increment,
  `description` tinytext,
  PRIMARY KEY  (`menus_being_used_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `menus_being_used`
-- 

INSERT INTO `menus_being_used` VALUES (1, 'Full Menus');
INSERT INTO `menus_being_used` VALUES (2, 'Interview Menus Only');
INSERT INTO `menus_being_used` VALUES (3, 'Co-op Japan Menus');
INSERT INTO `menus_being_used` VALUES (4, 'Director''s Office Menus');
INSERT INTO `menus_being_used` VALUES (5, 'Employer Service (Traffic Director) Menus');

-- --------------------------------------------------------

-- 
-- Table structure for table `module`
-- 

DROP TABLE IF EXISTS `module`;
CREATE TABLE IF NOT EXISTS `module` (
  `module_id` smallint(5) unsigned NOT NULL auto_increment,
  `module_name` varchar(40) default NULL,
  `order_by` smallint(5) unsigned default '0',
  PRIMARY KEY  (`module_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `module`
-- 

INSERT INTO `module` VALUES (1, 'Other', 90);
INSERT INTO `module` VALUES (2, 'Student Info', 20);
INSERT INTO `module` VALUES (3, 'Employer Info', 30);
INSERT INTO `module` VALUES (4, 'New Contact Info', 40);
INSERT INTO `module` VALUES (5, 'Job Descriptions', 50);
INSERT INTO `module` VALUES (6, 'Resumes', 60);
INSERT INTO `module` VALUES (10, 'Interviews', 10);
INSERT INTO `module` VALUES (12, 'Placement', 70);
INSERT INTO `module` VALUES (14, 'Reporting/stats', 80);
INSERT INTO `module` VALUES (15, 'Feedback', 110);
INSERT INTO `module` VALUES (18, 'Help', 100);
INSERT INTO `module` VALUES (19, 'Logout', 600);
INSERT INTO `module` VALUES (20, 'Preferences', 85);
INSERT INTO `module` VALUES (40, 'Student Info', 20);
INSERT INTO `module` VALUES (41, 'Other: CSc & ENGR', 90);
INSERT INTO `module` VALUES (42, 'Admin Reporting', 80);
INSERT INTO `module` VALUES (43, 'Evaluations/Surveys', 95);

-- --------------------------------------------------------

-- 
-- Table structure for table `pending_employer_company`
-- 

DROP TABLE IF EXISTS `pending_employer_company`;
CREATE TABLE IF NOT EXISTS `pending_employer_company` (
  `employer_id` mediumint(8) unsigned NOT NULL auto_increment,
  `company_name` varchar(75) default NULL,
  `company_website` varchar(60) default NULL,
  `company_description` text,
  `street_address_1` varchar(75) default NULL,
  `street_address_2` varchar(75) default NULL,
  `street_address_3` varchar(75) default NULL,
  `city` varchar(40) default NULL,
  `provstate_id` mediumint(8) unsigned default NULL,
  `country_id` mediumint(8) unsigned default NULL,
  `postal_code` varchar(10) default NULL,
  `phone` varchar(25) default NULL,
  `fax` varchar(25) default NULL,
  `email` varchar(100) default NULL,
  `department_name` varchar(75) default NULL,
  `industry_id` mediumint(8) unsigned default NULL,
  `size_id` mediumint(8) unsigned default NULL,
  `entered_on` datetime default NULL,
  `region_id` mediumint(8) unsigned NOT NULL default '0',
  `company_type_id` mediumint(8) unsigned default NULL,
  PRIMARY KEY  (`employer_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `pending_employer_company`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `pending_employer_contact`
-- 

DROP TABLE IF EXISTS `pending_employer_contact`;
CREATE TABLE IF NOT EXISTS `pending_employer_contact` (
  `employer_id` mediumint(8) unsigned NOT NULL default '0',
  `contact_id` bigint(20) unsigned NOT NULL auto_increment,
  `title` varchar(10) default NULL,
  `first_name` varchar(40) default NULL,
  `last_name` varchar(40) default NULL,
  `called_name` varchar(40) default NULL,
  `middle_name` varchar(40) default NULL,
  `email` varchar(100) default NULL,
  `phone` varchar(25) default NULL,
  `pager` varchar(25) default NULL,
  `cellphone` varchar(25) default NULL,
  `fax` varchar(25) default NULL,
  `street_address_1` varchar(75) default NULL,
  `street_address_2` varchar(75) default NULL,
  `street_address_3` varchar(75) default NULL,
  `country_id` mediumint(8) unsigned default NULL,
  `provstate_id` mediumint(8) unsigned default NULL,
  `region_id` mediumint(8) unsigned default NULL,
  `city` varchar(40) default NULL,
  `postal_code` varchar(10) default NULL,
  `entered_on` datetime default NULL,
  `department_name` varchar(150) default NULL,
  `ip_added_with` varchar(50) default NULL,
  `module` enum('job','eval') default 'job',
  PRIMARY KEY  (`contact_id`),
  KEY `employer_id` (`employer_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `pending_employer_contact`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `pending_job_description`
-- 

DROP TABLE IF EXISTS `pending_job_description`;
CREATE TABLE IF NOT EXISTS `pending_job_description` (
  `job_id` mediumint(8) unsigned NOT NULL auto_increment,
  `employer_id` mediumint(8) unsigned default '0',
  `contact_id` bigint(20) unsigned default '0',
  `term_id` smallint(6) default NULL,
  `year` year(4) default NULL,
  `position_title` tinytext,
  `employer_code` varchar(14) default NULL,
  `num_positions` smallint(5) unsigned default NULL,
  `apply_method` tinytext,
  `receive_address` varchar(100) default NULL,
  `country_id` mediumint(8) unsigned default NULL,
  `provstate_id` mediumint(8) unsigned default NULL,
  `region_id` mediumint(8) unsigned default NULL,
  `city` tinytext,
  `salary_amount1` varchar(25) default NULL,
  `salary_amount2` varchar(25) default NULL,
  `salary_period` tinytext,
  `workterm_hours` varchar(20) default NULL,
  `workterm_lengths` tinytext,
  `other_postings` text,
  `job_description` text,
  `min_academic` tinyint(3) unsigned default NULL,
  `min_workterms` tinyint(3) unsigned default NULL,
  `special_requirements` text,
  `skills_required` text,
  `comments` text,
  `date_added` datetime default NULL,
  `industry` mediumint(8) unsigned default NULL,
  `ip_added_with` varchar(50) default NULL,
  `host_department` smallint(5) unsigned default NULL,
  `apply_departments` tinytext,
  `disciplines` tinytext,
  `start_date` date default NULL,
  `end_date` date default NULL,
  `displayname_on_coverltr` tinyint(1) unsigned default '0',
  PRIMARY KEY  (`job_id`),
  KEY `employer_id` (`employer_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `pending_job_description`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `plugins`
-- 

DROP TABLE IF EXISTS `plugins`;
CREATE TABLE IF NOT EXISTS `plugins` (
  `plugin_id` int(11) unsigned NOT NULL auto_increment,
  `plugin_name` varchar(250) NOT NULL default '',
  `plugin_path` varchar(250) NOT NULL default '',
  `plugin_type` varchar(255) NOT NULL default '0',
  PRIMARY KEY  (`plugin_id`),
  UNIQUE KEY `plugin_name` (`plugin_name`,`plugin_path`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `plugins`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `preferences`
-- 

DROP TABLE IF EXISTS `preferences`;
CREATE TABLE IF NOT EXISTS `preferences` (
  `contact_id` bigint(20) unsigned default NULL,
  `student_number` varchar(10) default NULL,
  `colorscheme` varchar(10) default 'uvic',
  UNIQUE KEY `student_number` (`student_number`),
  UNIQUE KEY `contact_id` (`contact_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `preferences`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `presentation`
-- 

DROP TABLE IF EXISTS `presentation`;
CREATE TABLE IF NOT EXISTS `presentation` (
  `job_id` mediumint(8) unsigned NOT NULL default '0',
  `pres_date` date NOT NULL default '0000-00-00',
  `pres_time` time NOT NULL default '00:00:00',
  `pres_loc` varchar(60) NOT NULL default '',
  `pres_notes` blob,
  `end_time` time default NULL,
  `viewable_by_all` tinyint(3) unsigned default '1',
  PRIMARY KEY  (`job_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `presentation`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `provstate_list`
-- 

DROP TABLE IF EXISTS `provstate_list`;
CREATE TABLE IF NOT EXISTS `provstate_list` (
  `provstate_id` mediumint(8) unsigned NOT NULL auto_increment,
  `provstate_name` varchar(50) default NULL,
  `country_id` mediumint(8) unsigned NOT NULL default '0',
  `abbreviation` varchar(10) default NULL,
  PRIMARY KEY  (`provstate_id`),
  KEY `country_id` (`country_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `provstate_list`
-- 

INSERT INTO `provstate_list` VALUES (1, 'British Columbia', 1, 'BC');
INSERT INTO `provstate_list` VALUES (2, 'Alberta', 1, 'AB');
INSERT INTO `provstate_list` VALUES (3, 'Manitoba', 1, 'MB');
INSERT INTO `provstate_list` VALUES (4, 'New Brunswick', 1, 'NB');
INSERT INTO `provstate_list` VALUES (5, 'Newfoundland', 1, 'NL');
INSERT INTO `provstate_list` VALUES (6, 'Northwest Territories', 1, 'NWT');
INSERT INTO `provstate_list` VALUES (7, 'Nova Scotia', 1, 'NS');
INSERT INTO `provstate_list` VALUES (8, 'Nunavut', 1, 'NU');
INSERT INTO `provstate_list` VALUES (9, 'Ontario', 1, 'ON');
INSERT INTO `provstate_list` VALUES (10, 'Prince Edward Island', 1, 'PEI');
INSERT INTO `provstate_list` VALUES (11, 'Quebec', 1, 'PQ');
INSERT INTO `provstate_list` VALUES (12, 'Saskatchewan', 1, 'SK');
INSERT INTO `provstate_list` VALUES (13, 'Yukon Territory', 1, 'YK');
INSERT INTO `provstate_list` VALUES (14, 'Alabama', 2, 'AL');
INSERT INTO `provstate_list` VALUES (15, 'Alaska', 2, 'AK');
INSERT INTO `provstate_list` VALUES (16, 'Arizona', 2, 'AZ');
INSERT INTO `provstate_list` VALUES (17, 'Arkansas', 2, 'AR');
INSERT INTO `provstate_list` VALUES (18, 'California', 2, 'CA');
INSERT INTO `provstate_list` VALUES (19, 'Colorado', 2, 'CO');
INSERT INTO `provstate_list` VALUES (20, 'Connecticut', 2, 'CT');
INSERT INTO `provstate_list` VALUES (21, 'Delaware', 2, 'DE');
INSERT INTO `provstate_list` VALUES (22, 'District of Columbia', 2, 'DC');
INSERT INTO `provstate_list` VALUES (23, 'Florida', 2, 'FL');
INSERT INTO `provstate_list` VALUES (24, 'Georgia', 2, 'GA');
INSERT INTO `provstate_list` VALUES (25, 'Hawaii', 2, 'HI');
INSERT INTO `provstate_list` VALUES (26, 'Idaho', 2, 'ID');
INSERT INTO `provstate_list` VALUES (27, 'Illinois', 2, 'IL');
INSERT INTO `provstate_list` VALUES (28, 'Indiana', 2, 'IN');
INSERT INTO `provstate_list` VALUES (29, 'Iowa', 2, 'IA');
INSERT INTO `provstate_list` VALUES (30, 'Kansas', 2, 'KS');
INSERT INTO `provstate_list` VALUES (31, 'Kentucky', 2, 'KY');
INSERT INTO `provstate_list` VALUES (32, 'Louisiana', 2, 'LA');
INSERT INTO `provstate_list` VALUES (33, 'Maine', 2, 'ME');
INSERT INTO `provstate_list` VALUES (34, 'Maryland', 2, 'MD');
INSERT INTO `provstate_list` VALUES (35, 'Massachusetts', 2, 'MA');
INSERT INTO `provstate_list` VALUES (36, 'Michigan', 2, 'MI');
INSERT INTO `provstate_list` VALUES (37, 'Minnesota', 2, 'MN');
INSERT INTO `provstate_list` VALUES (38, 'Mississippi', 2, 'MS');
INSERT INTO `provstate_list` VALUES (39, 'Missouri', 2, 'MO');
INSERT INTO `provstate_list` VALUES (40, 'Montana', 2, 'MT');
INSERT INTO `provstate_list` VALUES (41, 'Nebraska', 2, 'NE');
INSERT INTO `provstate_list` VALUES (42, 'Nevada', 2, 'NV');
INSERT INTO `provstate_list` VALUES (43, 'New Hampshire', 2, 'NH');
INSERT INTO `provstate_list` VALUES (44, 'New Jersey', 2, 'NJ');
INSERT INTO `provstate_list` VALUES (45, 'New Mexico', 2, 'NM');
INSERT INTO `provstate_list` VALUES (46, 'New York', 2, 'NY');
INSERT INTO `provstate_list` VALUES (47, 'North Carolina', 2, 'NC');
INSERT INTO `provstate_list` VALUES (48, 'North Dakota', 2, 'ND');
INSERT INTO `provstate_list` VALUES (49, 'Ohio', 2, 'OH');
INSERT INTO `provstate_list` VALUES (50, 'Oklahoma', 2, 'OK');
INSERT INTO `provstate_list` VALUES (51, 'Oregon', 2, 'OR');
INSERT INTO `provstate_list` VALUES (52, 'Pennsylvania', 2, 'PA');
INSERT INTO `provstate_list` VALUES (53, 'Rhode Island', 2, 'RI');
INSERT INTO `provstate_list` VALUES (54, 'South Carolina', 2, 'SC');
INSERT INTO `provstate_list` VALUES (55, 'South Dakota', 2, 'SD');
INSERT INTO `provstate_list` VALUES (56, 'Tennessee', 2, 'TN');
INSERT INTO `provstate_list` VALUES (57, 'Texas', 2, 'TX');
INSERT INTO `provstate_list` VALUES (58, 'Utah', 2, 'UT');
INSERT INTO `provstate_list` VALUES (59, 'Vermont', 2, 'VT');
INSERT INTO `provstate_list` VALUES (60, 'Virginia', 2, 'VA');
INSERT INTO `provstate_list` VALUES (61, 'Washington', 2, 'WA');
INSERT INTO `provstate_list` VALUES (62, 'West Virginia', 2, 'WV');
INSERT INTO `provstate_list` VALUES (63, 'Wisconsin', 2, 'WI');
INSERT INTO `provstate_list` VALUES (64, 'Wyoming', 2, 'WY');
INSERT INTO `provstate_list` VALUES (65, 'Tasmania', 14, 'TAS');
INSERT INTO `provstate_list` VALUES (66, 'Victoria', 14, 'VIC');
INSERT INTO `provstate_list` VALUES (67, 'Queensland', 14, 'QLD');

-- --------------------------------------------------------

-- 
-- Table structure for table `region_list`
-- 

DROP TABLE IF EXISTS `region_list`;
CREATE TABLE IF NOT EXISTS `region_list` (
  `region_id` mediumint(8) unsigned NOT NULL auto_increment,
  `region_name` varchar(50) default NULL,
  `provstate_id` mediumint(8) unsigned NOT NULL default '0',
  `city_name` varchar(40) default NULL,
  PRIMARY KEY  (`region_id`),
  KEY `provstate_id` (`provstate_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `region_list`
-- 

INSERT INTO `region_list` VALUES (1, 'Victoria (Capital Region)', 1, 'Victoria');
INSERT INTO `region_list` VALUES (2, 'Cowichan Valley', 1, NULL);
INSERT INTO `region_list` VALUES (3, 'Nanaimo', 1, 'Nanaimo');
INSERT INTO `region_list` VALUES (4, 'Other Vancouver Island', 1, NULL);
INSERT INTO `region_list` VALUES (5, 'Lower Mainland (outside Greater Vancouver)', 1, NULL);
INSERT INTO `region_list` VALUES (6, 'Thompson-Okanagan', 1, NULL);
INSERT INTO `region_list` VALUES (7, 'Northern BC', 1, NULL);
INSERT INTO `region_list` VALUES (8, 'Greater Vancouver', 1, 'Vancouver');
INSERT INTO `region_list` VALUES (9, 'Calgary', 2, 'Calgary');
INSERT INTO `region_list` VALUES (10, 'Edmonton', 2, 'Edmonton');
INSERT INTO `region_list` VALUES (11, 'Northern Alberta', 2, NULL);
INSERT INTO `region_list` VALUES (12, 'Southern Alberta', 2, NULL);
INSERT INTO `region_list` VALUES (13, 'Fredericton', 4, 'Fredericton');
INSERT INTO `region_list` VALUES (14, 'Moncton', 4, 'Moncton');
INSERT INTO `region_list` VALUES (15, 'Saint John', 4, 'Saint John');
INSERT INTO `region_list` VALUES (16, 'Hamilton', 9, 'Hamilton');
INSERT INTO `region_list` VALUES (17, 'Kitchener', 9, 'Kitchener');
INSERT INTO `region_list` VALUES (18, 'London', 9, 'London');
INSERT INTO `region_list` VALUES (19, 'Mississauga', 9, 'Mississauga');
INSERT INTO `region_list` VALUES (20, 'Northern Ontario', 9, NULL);
INSERT INTO `region_list` VALUES (21, 'Ottawa', 9, 'Ottawa');
INSERT INTO `region_list` VALUES (22, 'South Western Ontario', 9, NULL);
INSERT INTO `region_list` VALUES (23, 'Toronto', 9, 'Toronto');
INSERT INTO `region_list` VALUES (24, 'Montreal', 11, 'Montreal');
INSERT INTO `region_list` VALUES (25, 'Northen Quebec', 11, NULL);
INSERT INTO `region_list` VALUES (26, 'Quebec City', 11, 'Quebec');
INSERT INTO `region_list` VALUES (27, 'Regina', 12, 'Regina');
INSERT INTO `region_list` VALUES (28, 'Saskatoon', 12, 'Saskatoon');
INSERT INTO `region_list` VALUES (29, 'Northern Saskatchewan', 12, NULL);
INSERT INTO `region_list` VALUES (30, 'Other BC', 1, NULL);
INSERT INTO `region_list` VALUES (31, 'Other New Brunswick', 4, NULL);
INSERT INTO `region_list` VALUES (32, 'Other Ontario', 9, NULL);
INSERT INTO `region_list` VALUES (33, 'Other Saskatchewan', 12, NULL);
INSERT INTO `region_list` VALUES (34, 'Other Quebec', 11, NULL);
INSERT INTO `region_list` VALUES (35, 'Kootenay', 1, NULL);
INSERT INTO `region_list` VALUES (36, 'Cariboo', 1, NULL);

-- --------------------------------------------------------

-- 
-- Table structure for table `rejected_employer_ips`
-- 

DROP TABLE IF EXISTS `rejected_employer_ips`;
CREATE TABLE IF NOT EXISTS `rejected_employer_ips` (
  `date_rejected` date default NULL,
  `IP_address` varchar(50) default NULL,
  `position_title` tinytext,
  `company_name` varchar(75) default NULL,
  `contact_first_name` varchar(40) default NULL,
  `contact_last_name` varchar(40) default NULL
) ENGINE=MyISAM;

-- 
-- Dumping data for table `rejected_employer_ips`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `resume`
-- 

DROP TABLE IF EXISTS `resume`;
CREATE TABLE IF NOT EXISTS `resume` (
  `resume_id` mediumint(8) unsigned NOT NULL auto_increment,
  `student_number` varchar(10) NOT NULL default '',
  `resume` mediumtext,
  `last_modified` datetime default NULL,
  `type_id` smallint(5) unsigned NOT NULL default '0',
  `name` varchar(50) default NULL,
  `builder` char(3) default 'no',
  `builder_class` mediumtext,
  `builder_id` mediumint(5) default NULL,
  PRIMARY KEY  (`resume_id`,`student_number`),
  KEY `student_number` (`student_number`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `resume`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `room`
-- 

DROP TABLE IF EXISTS `room`;
CREATE TABLE IF NOT EXISTS `room` (
  `room_id` smallint(5) unsigned NOT NULL auto_increment,
  `building_id` smallint(5) unsigned NOT NULL default '0',
  `room_number` varchar(10) NOT NULL default '',
  `phone` varchar(25) default NULL,
  `email` varchar(50) default NULL,
  `wait_room_id` smallint(5) unsigned default NULL,
  PRIMARY KEY  (`room_id`),
  UNIQUE KEY `room_index` (`building_id`,`room_number`(4))
) ENGINE=MyISAM;

-- 
-- Dumping data for table `room`
-- 

INSERT INTO `room` VALUES (1, 2, '225', '', 'email@email.ca', NULL);
INSERT INTO `room` VALUES (21, 1, 'Lobby', '', '', NULL);
INSERT INTO `room` VALUES (3, 1, 'A210', '', '', NULL);
INSERT INTO `room` VALUES (4, 1, 'A204', '', '', NULL);

-- --------------------------------------------------------

-- 
-- Table structure for table `salary_period`
-- 

DROP TABLE IF EXISTS `salary_period`;
CREATE TABLE IF NOT EXISTS `salary_period` (
  `salary_period_id` smallint(6) NOT NULL auto_increment,
  `salary_period_name` varchar(80) default NULL,
  `order_by` smallint(6) default NULL,
  PRIMARY KEY  (`salary_period_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `salary_period`
-- 

INSERT INTO `salary_period` VALUES (1, 'hour', 1);
INSERT INTO `salary_period` VALUES (2, 'week', 2);
INSERT INTO `salary_period` VALUES (3, '2 weeks', 3);
INSERT INTO `salary_period` VALUES (4, 'month', 4);
INSERT INTO `salary_period` VALUES (5, 'year', 5);

-- --------------------------------------------------------

-- 
-- Table structure for table `search`
-- 

DROP TABLE IF EXISTS `search`;
CREATE TABLE IF NOT EXISTS `search` (
  `search_id` int(11) NOT NULL auto_increment,
  `search` mediumtext,
  `contact_id` int(11) default NULL,
  `search_name` varchar(100) default NULL,
  `search_module_id` int(11) default NULL,
  `search_description` varchar(255) default NULL,
  PRIMARY KEY  (`search_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `search`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `search_module`
-- 

DROP TABLE IF EXISTS `search_module`;
CREATE TABLE IF NOT EXISTS `search_module` (
  `module_id` int(11) NOT NULL auto_increment,
  `module_name` varchar(100) default NULL,
  PRIMARY KEY  (`module_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `search_module`
-- 

INSERT INTO `search_module` VALUES (1, 'Student Advanced Search');
INSERT INTO `search_module` VALUES (2, 'Placement History Advanced Search');
INSERT INTO `search_module` VALUES (3, 'Employer Information - Companies/Divisions Search');
INSERT INTO `search_module` VALUES (4, 'Employer Information - Contacts Search');

-- --------------------------------------------------------

-- 
-- Table structure for table `semesters`
-- 

DROP TABLE IF EXISTS `semesters`;
CREATE TABLE IF NOT EXISTS `semesters` (
  `semesters_id` smallint(6) NOT NULL auto_increment,
  `description` varchar(100) default NULL,
  `display` varchar(20) default NULL,
  `description_type` smallint(6) default NULL,
  PRIMARY KEY  (`semesters_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `semesters`
-- 

INSERT INTO `semesters` VALUES (1, 'Work Term 1', 'WT1', 1);
INSERT INTO `semesters` VALUES (2, 'Work Term 2', 'WT2', 1);
INSERT INTO `semesters` VALUES (3, 'Work Term 3', 'WT3', 1);
INSERT INTO `semesters` VALUES (4, 'Work Term 4', 'WT4', 1);
INSERT INTO `semesters` VALUES (5, 'Work Term 5', 'WT5', 1);
INSERT INTO `semesters` VALUES (6, 'Work Term 6', 'WT6', 1);
INSERT INTO `semesters` VALUES (7, 'Work Term 7', 'WT7', 1);
INSERT INTO `semesters` VALUES (8, 'Work Term 8', 'WT8', 1);
INSERT INTO `semesters` VALUES (9, 'Academic Term 1A', '1A', 2);
INSERT INTO `semesters` VALUES (10, 'Academic Term 1B', '1B', 2);
INSERT INTO `semesters` VALUES (11, 'Academic Term 2A', '2A', 2);
INSERT INTO `semesters` VALUES (12, 'Academic Term 2B', '2B', 2);
INSERT INTO `semesters` VALUES (13, 'Academic Term 3A', '3A', 2);
INSERT INTO `semesters` VALUES (14, 'Academic Term 3B', '3B', 2);
INSERT INTO `semesters` VALUES (15, 'Academic Term 4A', '4A', 2);
INSERT INTO `semesters` VALUES (16, 'Academic Term 4B', '4B', 2);
INSERT INTO `semesters` VALUES (29, 'Graduate Campus Term', 'GC*', 2);
INSERT INTO `semesters` VALUES (22, 'Modified Year 1', '1*', 2);
INSERT INTO `semesters` VALUES (23, 'Modified Year 2', '2*', 2);
INSERT INTO `semesters` VALUES (24, 'Modified Year 3', '3*', 2);
INSERT INTO `semesters` VALUES (25, 'Modified Year 4', '4*', 2);
INSERT INTO `semesters` VALUES (20, 'Campus Term', 'CT', 2);
INSERT INTO `semesters` VALUES (26, 'Excused from Term', 'EXC', 3);
INSERT INTO `semesters` VALUES (27, 'Part-time Work Term', 'PWT', 1);
INSERT INTO `semesters` VALUES (28, 'Business Management Option', 'BMO', 2);
INSERT INTO `semesters` VALUES (21, 'Unknown', '??', 3);
INSERT INTO `semesters` VALUES (30, 'Time-off', 'OFF', 4);
INSERT INTO `semesters` VALUES (31, 'Withdrawn No Fault', 'WNF', 3);
INSERT INTO `semesters` VALUES (32, 'Work Term ?', 'WT?', 1);
INSERT INTO `semesters` VALUES (33, 'Academic Term 5A', '5A', 2);
INSERT INTO `semesters` VALUES (34, 'Academic Term 5B', '5B', 2);
INSERT INTO `semesters` VALUES (35, 'Academic Term 6A', '6A', 2);
INSERT INTO `semesters` VALUES (36, 'Academic Term 6B', '6B', 2);
INSERT INTO `semesters` VALUES (37, 'Academic Term 7A', '7A', 2);
INSERT INTO `semesters` VALUES (38, 'Academic Term 7B', '7B', 2);
INSERT INTO `semesters` VALUES (39, 'Academic Term 8A', '8A', 2);
INSERT INTO `semesters` VALUES (40, 'Academic Term 8B', '8B', 2);
INSERT INTO `semesters` VALUES (41, 'Academic Term 9A', '9A', 2);
INSERT INTO `semesters` VALUES (42, 'Academic Term 9B', '9B', 2);
INSERT INTO `semesters` VALUES (43, 'Modified Year 5', '5*', 2);
INSERT INTO `semesters` VALUES (44, 'Modified Year 6', '6*', 2);
INSERT INTO `semesters` VALUES (45, 'Modified Year 7', '7*', 2);
INSERT INTO `semesters` VALUES (46, 'Modified Year 8', '8*', 2);
INSERT INTO `semesters` VALUES (47, 'Modified Year 9', '9*', 2);
INSERT INTO `semesters` VALUES (48, 'No Response', 'NR', 3);
INSERT INTO `semesters` VALUES (49, 'Graduated', 'GRD', 3);
INSERT INTO `semesters` VALUES (50, 'Extend', 'EXT', 2);
INSERT INTO `semesters` VALUES (51, 'Work Term', 'W', 1);
INSERT INTO `semesters` VALUES (52, 'Work Term 1', 'W1', 1);
INSERT INTO `semesters` VALUES (53, 'Work Term 2', 'W2', 1);
INSERT INTO `semesters` VALUES (54, 'Work Term 3', 'W3', 1);
INSERT INTO `semesters` VALUES (55, 'Graduating on a Work Term', 'WG', 1);
INSERT INTO `semesters` VALUES (56, 'Graduating on a Campus Term', 'CG', 2);
INSERT INTO `semesters` VALUES (57, 'Work Term Transfer', 'WT1', 1);
INSERT INTO `semesters` VALUES (58, 'Work Term Preadmit', 'WP1', 1);
INSERT INTO `semesters` VALUES (59, 'Work Term Challenge 1', 'WC1', 1);
INSERT INTO `semesters` VALUES (60, 'Work Term Challenge 2', 'WC2', 1);
INSERT INTO `semesters` VALUES (61, 'Work Term Challenge 3', 'WC3', 1);
INSERT INTO `semesters` VALUES (62, 'Work Term Optional', 'WO', 1);
INSERT INTO `semesters` VALUES (63, 'International Business Exchange', 'IBX', 3);
INSERT INTO `semesters` VALUES (64, 'MBA', 'MBA', 2);
INSERT INTO `semesters` VALUES (65, 'MPA', 'MPA', 2);
INSERT INTO `semesters` VALUES (66, 'Optional Work Term', 'OPT', 1);
INSERT INTO `semesters` VALUES (67, 'Stop Out', 'STO', 3);
INSERT INTO `semesters` VALUES (68, 'Transfer', 'TRA', 3);
INSERT INTO `semesters` VALUES (69, 'UBC Academic Exchange', 'UBC', 3);
INSERT INTO `semesters` VALUES (70, 'Academic Term 1C', '1C', 2);
INSERT INTO `semesters` VALUES (71, 'Academic Term 2C', '2C', 2);
INSERT INTO `semesters` VALUES (72, 'Academic Term 3C', '3C', 2);
INSERT INTO `semesters` VALUES (73, 'Academic Term 4C', '4C', 2);
INSERT INTO `semesters` VALUES (74, 'On Probation', 'PRO', 3);
INSERT INTO `semesters` VALUES (75, 'Academic Term 1D', '1D', 2);
INSERT INTO `semesters` VALUES (76, 'Academic Term 2D', '2D', 2);
INSERT INTO `semesters` VALUES (77, 'Academic Term 3D', '3D', 2);
INSERT INTO `semesters` VALUES (78, 'Academic Term 4D', '4D', 2);
INSERT INTO `semesters` VALUES (79, 'Work Term 9', 'WT9', 1);
INSERT INTO `semesters` VALUES (80, 'Bamfield Marine Station', 'BMS', 3);
INSERT INTO `semesters` VALUES (81, 'Transfer Academic Term 1', 'T1', 2);
INSERT INTO `semesters` VALUES (82, 'Transfer Academic Term 1A', 'T1A', 2);
INSERT INTO `semesters` VALUES (83, 'Transfer Academic Term 1B', 'T1B', 2);
INSERT INTO `semesters` VALUES (84, 'Transfer Academic Term 2', 'T2', 2);
INSERT INTO `semesters` VALUES (85, 'Transfer Academic Term 2A', 'T2A', 2);
INSERT INTO `semesters` VALUES (86, 'Transfer Academic Term 2B', 'T2B', 2);
INSERT INTO `semesters` VALUES (87, 'Transfer Academic Term 3', 'T3', 2);
INSERT INTO `semesters` VALUES (88, 'Work Term Done in Another Program 1', 'XT1', 1);
INSERT INTO `semesters` VALUES (89, 'Work Term Done in Another Program 2', 'XT2', 1);
INSERT INTO `semesters` VALUES (90, 'Work Term Done in Another Program 3', 'XT3', 1);
INSERT INTO `semesters` VALUES (91, 'Work Term Done in Another Program 4', 'XT4', 1);

-- --------------------------------------------------------

-- 
-- Table structure for table `semesters_description_type`
-- 

DROP TABLE IF EXISTS `semesters_description_type`;
CREATE TABLE IF NOT EXISTS `semesters_description_type` (
  `description_type` smallint(6) NOT NULL auto_increment,
  `description` varchar(100) default NULL,
  PRIMARY KEY  (`description_type`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `semesters_description_type`
-- 

INSERT INTO `semesters_description_type` VALUES (1, 'Work Term');
INSERT INTO `semesters_description_type` VALUES (2, 'Academic Term');
INSERT INTO `semesters_description_type` VALUES (3, 'Miscellaneous Term');
INSERT INTO `semesters_description_type` VALUES (4, 'Off Term');

-- --------------------------------------------------------

-- 
-- Table structure for table `semesters_table`
-- 

DROP TABLE IF EXISTS `semesters_table`;
CREATE TABLE IF NOT EXISTS `semesters_table` (
  `term_id` tinyint(3) unsigned default NULL,
  `year` year(4) default NULL,
  `semesters_id` smallint(6) default NULL,
  `record_id` int(11) default NULL,
  KEY `year` (`year`),
  KEY `record_id` (`record_id`),
  KEY `semesters_id` (`semesters_id`),
  KEY `term_id` (`term_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `semesters_table`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `sequential_job_code`
-- 

DROP TABLE IF EXISTS `sequential_job_code`;
CREATE TABLE IF NOT EXISTS `sequential_job_code` (
  `next_sequential_code` mediumint(8) unsigned default '1',
  `term_id` tinyint(3) unsigned default NULL,
  `year` year(4) default NULL
) ENGINE=MyISAM;

-- 
-- Dumping data for table `sequential_job_code`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `signup_edit`
-- 

DROP TABLE IF EXISTS `signup_edit`;
CREATE TABLE IF NOT EXISTS `signup_edit` (
  `job_id` mediumint(8) unsigned default NULL,
  `last_edited` timestamp NOT NULL,
  `type` enum('Student','Admin') default NULL,
  KEY `signup_edit_indx` (`job_id`,`type`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `signup_edit`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `site_visit_type`
-- 

DROP TABLE IF EXISTS `site_visit_type`;
CREATE TABLE IF NOT EXISTS `site_visit_type` (
  `site_visit_type_id` tinyint(4) unsigned NOT NULL auto_increment,
  `description` varchar(50) default NULL,
  PRIMARY KEY  (`site_visit_type_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `site_visit_type`
-- 

INSERT INTO `site_visit_type` VALUES (1, 'On Site Visit');
INSERT INTO `site_visit_type` VALUES (2, 'Phone');
INSERT INTO `site_visit_type` VALUES (3, 'E-mail');
INSERT INTO `site_visit_type` VALUES (4, 'On Campus Debriefing');
INSERT INTO `site_visit_type` VALUES (5, 'Other');

-- --------------------------------------------------------

-- 
-- Table structure for table `special_flags`
-- 

DROP TABLE IF EXISTS `special_flags`;
CREATE TABLE IF NOT EXISTS `special_flags` (
  `table_name` tinytext,
  `flag_type_id` int(11) NOT NULL auto_increment,
  `description` text,
  PRIMARY KEY  (`flag_type_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `special_flags`
-- 

INSERT INTO `special_flags` VALUES ('NONE', 1, 'General');
INSERT INTO `special_flags` VALUES ('NONE', 2, 'Profile Flags');
INSERT INTO `special_flags` VALUES ('eligible_placed', 4, 'Term Flags');

-- --------------------------------------------------------

-- 
-- Table structure for table `status`
-- 

DROP TABLE IF EXISTS `status`;
CREATE TABLE IF NOT EXISTS `status` (
  `status_id` smallint(5) unsigned NOT NULL auto_increment,
  `status_name` varchar(40) NOT NULL default '',
  `order_by` tinyint(3) unsigned default NULL,
  PRIMARY KEY  (`status_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `status`
-- 

INSERT INTO `status` VALUES (1, 'Not to be implemented', 5);
INSERT INTO `status` VALUES (2, 'Long term plan', 3);
INSERT INTO `status` VALUES (3, 'Short term plan', 2);
INSERT INTO `status` VALUES (4, 'In progress', 1);
INSERT INTO `status` VALUES (5, 'Complete', 4);

-- --------------------------------------------------------

-- 
-- Table structure for table `student`
-- 

DROP TABLE IF EXISTS `student`;
CREATE TABLE IF NOT EXISTS `student` (
  `login_id` varchar(8) NOT NULL default '*',
  `student_number` varchar(10) NOT NULL default '',
  `first_name` varchar(40) NOT NULL default '',
  `last_name` varchar(40) NOT NULL default '',
  `netlink_id` varchar(10) default '*',
  `current` tinyint(4) default NULL,
  `student_status` smallint(5) unsigned default NULL,
  `street_address_current` varchar(75) default NULL,
  `birth` date default NULL,
  `street_address_permanent` varchar(75) default NULL,
  `postal_code_current` varchar(10) default NULL,
  `postal_code_permanent` varchar(10) default NULL,
  `city_current` varchar(40) default NULL,
  `city_permanent` varchar(40) default NULL,
  `province_current` mediumint(8) unsigned default NULL,
  `province_permanent` mediumint(8) unsigned default NULL,
  `email` varchar(100) default NULL,
  `high_school` varchar(100) default NULL,
  `phone_current` varchar(35) default NULL,
  `phone_permanent` varchar(35) default NULL,
  `photo` tinytext,
  `citizen` varchar(50) default NULL,
  `street_address_current2` varchar(75) default NULL,
  `street_address_permanent2` varchar(75) default NULL,
  `country_current` mediumint(8) unsigned default NULL,
  `country_permanent` mediumint(8) unsigned default NULL,
  `gender` char(1) default NULL,
  `middle_name` varchar(40) default NULL,
  `fax_current` varchar(35) default NULL,
  `fax_permanent` varchar(35) default NULL,
  `preferred_name` varchar(40) default NULL,
  `emergency_name` varchar(100) default NULL,
  `emergency_relationship` varchar(100) default NULL,
  `emergency_home_phone` varchar(35) default NULL,
  `emergency_home_fax` varchar(35) default NULL,
  `emergency_work_phone` varchar(35) default NULL,
  `emergency_work_fax` varchar(35) default NULL,
  `emergency_email` varchar(100) default NULL,
  `street_address_current3` varchar(75) default NULL,
  `street_address_permanent3` varchar(75) default NULL,
  `which_address` varchar(40) default NULL,
  `view_job_last` date default NULL,
  `view_job_last_time` time default NULL,
  `view_job_recent` date default NULL,
  `view_job_recent_time` time default NULL,
  `email2` varchar(100) default NULL,
  `current_region_id` mediumint(8) unsigned default NULL,
  `permanent_region_id` mediumint(8) unsigned default NULL,
  PRIMARY KEY  (`student_number`),
  KEY `last_name` (`last_name`),
  KEY `first_name` (`first_name`),
  KEY `netlink_id` (`netlink_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `student`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `student_action_types`
-- 

DROP TABLE IF EXISTS `student_action_types`;
CREATE TABLE IF NOT EXISTS `student_action_types` (
  `action_id` mediumint(9) NOT NULL auto_increment,
  `action_name` char(100) default NULL,
  PRIMARY KEY  (`action_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `student_action_types`
-- 

INSERT INTO `student_action_types` VALUES (1, 'Contacted Via E-Mail');
INSERT INTO `student_action_types` VALUES (2, 'Contacted Via Phone');
INSERT INTO `student_action_types` VALUES (3, 'Met in Person');
INSERT INTO `student_action_types` VALUES (5, 'Sent Work Term Package');
INSERT INTO `student_action_types` VALUES (6, 'Work Site Visit');

-- --------------------------------------------------------

-- 
-- Table structure for table `student_actions`
-- 

DROP TABLE IF EXISTS `student_actions`;
CREATE TABLE IF NOT EXISTS `student_actions` (
  `student_number` int(11) default NULL,
  `action_by` mediumint(9) default NULL,
  `action_on` date default NULL,
  `action_id` int(11) default NULL,
  `record_id` int(11) default NULL,
  `student_actions_id` int(11) NOT NULL auto_increment,
  PRIMARY KEY  (`student_actions_id`),
  KEY `record_id` (`record_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `student_actions`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `student_department`
-- 

DROP TABLE IF EXISTS `student_department`;
CREATE TABLE IF NOT EXISTS `student_department` (
  `department_id` smallint(5) unsigned default NULL,
  `student_number` varchar(10) default NULL,
  `comment` text,
  `advisor` varchar(100) default NULL,
  `academic_year` smallint(5) default NULL,
  `grad_gpa` double default NULL,
  `admit` date default NULL,
  `grad` date default NULL,
  `withdraw` date default NULL,
  `discipline_id` smallint(5) default NULL,
  `start_year` tinytext,
  `grad_year` smallint(6) default NULL,
  `record_id` int(11) NOT NULL auto_increment,
  `MAC_COOP_modified` varchar(16) default NULL,
  `MAC_COOP_URL` varchar(24) default NULL,
  `changes_recorded_1` text,
  `changes_recorded_2` text,
  `changes_recorded_3` text,
  `change_by_1` text,
  `change_by_2` text,
  `change_by_3` text,
  `change_date_1` date default NULL,
  `change_date_2` date default NULL,
  `change_date_3` date default NULL,
  `coop_advisor` bigint(20) unsigned default NULL,
  `convocation_month` smallint(2) default NULL,
  `convocation_year` year(4) default NULL,
  PRIMARY KEY  (`record_id`),
  KEY `student_number_index` (`student_number`),
  KEY `department_id` (`department_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `student_department`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `student_flags`
-- 

DROP TABLE IF EXISTS `student_flags`;
CREATE TABLE IF NOT EXISTS `student_flags` (
  `student_flags_id` int(11) NOT NULL auto_increment,
  `description` tinytext,
  `order_by` smallint(6) default NULL,
  `flag_type_id` int(11) default NULL,
  `comment` tinytext,
  PRIMARY KEY  (`student_flags_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `student_flags`
-- 

INSERT INTO `student_flags` VALUES (4, 'Eligible', NULL, 4, 'required');
INSERT INTO `student_flags` VALUES (5, 'Placed', NULL, 4, 'required');
INSERT INTO `student_flags` VALUES (7, 'Current', NULL, 1, 'required');

-- --------------------------------------------------------

-- 
-- Table structure for table `student_flags_join`
-- 

DROP TABLE IF EXISTS `student_flags_join`;
CREATE TABLE IF NOT EXISTS `student_flags_join` (
  `student_flags_id` int(11) default NULL,
  `record_id` int(11) default NULL,
  KEY `student_flags_id` (`student_flags_id`),
  KEY `record_id` (`record_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `student_flags_join`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `student_job_folder`
-- 

DROP TABLE IF EXISTS `student_job_folder`;
CREATE TABLE IF NOT EXISTS `student_job_folder` (
  `folder_id` mediumint(9) NOT NULL auto_increment,
  `default_name` varchar(100) default NULL,
  PRIMARY KEY  (`folder_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `student_job_folder`
-- 

INSERT INTO `student_job_folder` VALUES (1, 'Not Interested');
INSERT INTO `student_job_folder` VALUES (2, 'Potential');
INSERT INTO `student_job_folder` VALUES (3, 'High Interest');
INSERT INTO `student_job_folder` VALUES (4, 'Folder 1');
INSERT INTO `student_job_folder` VALUES (5, 'Folder 2');

-- --------------------------------------------------------

-- 
-- Table structure for table `student_job_folder_join`
-- 

DROP TABLE IF EXISTS `student_job_folder_join`;
CREATE TABLE IF NOT EXISTS `student_job_folder_join` (
  `folder_id` mediumint(9) default NULL,
  `job_id` mediumint(8) unsigned default NULL,
  `student_number` varchar(10) default NULL,
  KEY `job_id` (`job_id`),
  KEY `student_number` (`student_number`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `student_job_folder_join`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `student_job_folder_name`
-- 

DROP TABLE IF EXISTS `student_job_folder_name`;
CREATE TABLE IF NOT EXISTS `student_job_folder_name` (
  `folder_id` mediumint(9) default NULL,
  `student_number` varchar(10) default NULL,
  `folder_name` varchar(100) default NULL,
  KEY `student_number` (`student_number`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `student_job_folder_name`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `student_jobs_flagged`
-- 

DROP TABLE IF EXISTS `student_jobs_flagged`;
CREATE TABLE IF NOT EXISTS `student_jobs_flagged` (
  `student_number` varchar(10) default NULL,
  `job_id` mediumint(8) unsigned default NULL,
  KEY `student_number` (`student_number`),
  KEY `job_id` (`job_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `student_jobs_flagged`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `student_notes`
-- 

DROP TABLE IF EXISTS `student_notes`;
CREATE TABLE IF NOT EXISTS `student_notes` (
  `student_number` varchar(10) default NULL,
  `notes` text,
  `record_id` int(11) default NULL,
  `date_entered` datetime default NULL,
  `student_notes_id` mediumint(8) unsigned NOT NULL auto_increment,
  `contact_id` bigint(20) unsigned default NULL,
  `form_id` smallint(5) unsigned default NULL,
  PRIMARY KEY  (`student_notes_id`),
  KEY `record_id` (`record_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `student_notes`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `student_status`
-- 

DROP TABLE IF EXISTS `student_status`;
CREATE TABLE IF NOT EXISTS `student_status` (
  `status_id` tinyint(3) unsigned NOT NULL auto_increment,
  `description` varchar(50) default NULL,
  PRIMARY KEY  (`status_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `student_status`
-- 

INSERT INTO `student_status` VALUES (1, 'Accepting applications');
INSERT INTO `student_status` VALUES (2, 'No longer accepting applications');
INSERT INTO `student_status` VALUES (3, 'Students shortlisted');
INSERT INTO `student_status` VALUES (4, 'Waiting for ranking');
INSERT INTO `student_status` VALUES (5, 'Student hired from here');
INSERT INTO `student_status` VALUES (6, 'Student hired from elsewhere');
INSERT INTO `student_status` VALUES (7, 'Employer not hiring');

-- --------------------------------------------------------

-- 
-- Table structure for table `students_shortlisted`
-- 

DROP TABLE IF EXISTS `students_shortlisted`;
CREATE TABLE IF NOT EXISTS `students_shortlisted` (
  `job_id` mediumint(8) unsigned NOT NULL default '0',
  `student_number` varchar(10) NOT NULL default '',
  `last_visited` datetime default NULL,
  PRIMARY KEY  (`job_id`,`student_number`(8))
) ENGINE=MyISAM;

-- 
-- Dumping data for table `students_shortlisted`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `term`
-- 

DROP TABLE IF EXISTS `term`;
CREATE TABLE IF NOT EXISTS `term` (
  `term_id` tinyint(3) unsigned NOT NULL auto_increment,
  `term_code` varchar(10) NOT NULL default '',
  `term_name` varchar(50) default NULL,
  `start_date` tinyint(3) unsigned NOT NULL default '0',
  `end_date` tinyint(3) unsigned NOT NULL default '0',
  `order_by` smallint(6) default NULL,
  `year_order` tinyint(4) default NULL,
  PRIMARY KEY  (`term_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `term`
-- 

INSERT INTO `term` VALUES (1, 'F', 'Fall', 9, 12, 1, 3);
INSERT INTO `term` VALUES (2, 'S', 'Spring', 1, 4, 2, 1);
INSERT INTO `term` VALUES (3, 'K', 'Summer', 5, 8, 3, 2);

-- --------------------------------------------------------

-- 
-- Table structure for table `title`
-- 

DROP TABLE IF EXISTS `title`;
CREATE TABLE IF NOT EXISTS `title` (
  `title_id` smallint(5) unsigned NOT NULL auto_increment,
  `title_name` varchar(50) default NULL,
  PRIMARY KEY  (`title_id`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `title`
-- 

INSERT INTO `title` VALUES (1, 'Mr');
INSERT INTO `title` VALUES (2, 'Ms');
INSERT INTO `title` VALUES (3, 'Miss');
INSERT INTO `title` VALUES (4, 'Mrs');
INSERT INTO `title` VALUES (5, 'Dr');
INSERT INTO `title` VALUES (6, 'Prof');
INSERT INTO `title` VALUES (7, 'Capt');
INSERT INTO `title` VALUES (8, 'Sgt');
INSERT INTO `title` VALUES (9, 'Lt');
INSERT INTO `title` VALUES (10, 'Mme');
INSERT INTO `title` VALUES (11, 'Mlle');
INSERT INTO `title` VALUES (12, 'M');
INSERT INTO `title` VALUES (13, 'Hon');

-- --------------------------------------------------------

-- 
-- Table structure for table `transcript`
-- 

DROP TABLE IF EXISTS `transcript`;
CREATE TABLE IF NOT EXISTS `transcript` (
  `student_number` varchar(10) NOT NULL default '',
  `transcript` mediumtext,
  `last_modified` datetime default NULL,
  PRIMARY KEY  (`student_number`)
) ENGINE=MyISAM;

-- 
-- Dumping data for table `transcript`
-- 


-- --------------------------------------------------------

-- 
-- Table structure for table `user_types`
-- 

DROP TABLE IF EXISTS `user_types`;
CREATE TABLE IF NOT EXISTS `user_types` (
  `user_type` varchar(80) default NULL,
  `user_type_id` smallint(6) default NULL
) ENGINE=MyISAM;

-- 
-- Dumping data for table `user_types`
-- 

INSERT INTO `user_types` VALUES ('Student', 1);
INSERT INTO `user_types` VALUES ('Office', 4);
INSERT INTO `user_types` VALUES ('Employer', 6);
INSERT INTO `user_types` VALUES ('Traffic Director', 88);
