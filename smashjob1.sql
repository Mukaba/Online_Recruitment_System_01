-- phpMyAdmin SQL Dump
-- version 4.1.14
-- http://www.phpmyadmin.net
--
-- Host: 127.0.0.1
-- Generation Time: Nov 04, 2016 at 11:07 PM
-- Server version: 5.6.17
-- PHP Version: 5.5.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `smashjob`
--

-- --------------------------------------------------------

--
-- Table structure for table `application`
--

CREATE TABLE IF NOT EXISTS `application` (
  `id` int(255) NOT NULL AUTO_INCREMENT,
  `jobseeker_id` int(255) NOT NULL,
  `job_id` int(255) NOT NULL,
  `cv` varchar(255) NOT NULL,
  `additional_info` blob NOT NULL,
  `date_applied` varchar(255) NOT NULL,
  `reviewed` varchar(255) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  KEY `jobseeker_id` (`jobseeker_id`),
  KEY `job_id` (`job_id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=7 ;

--
-- Dumping data for table `application`
--

INSERT INTO `application` (`id`, `jobseeker_id`, `job_id`, `cv`, `additional_info`, `date_applied`, `reviewed`) VALUES
(5, 53, 3, 'img/cv/589381ee8e.docx', '', '03/11/2016', '1'),
(6, 53, 11, 'img/cv/589381ee8e.docx', '', '03/11/2016', '0');

-- --------------------------------------------------------

--
-- Table structure for table `certification`
--

CREATE TABLE IF NOT EXISTS `certification` (
  `id` int(225) NOT NULL AUTO_INCREMENT,
  `jobseeker_username` varchar(225) NOT NULL,
  `title` varchar(225) NOT NULL,
  `institution` varchar(225) NOT NULL,
  `year` varchar(225) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `jobseeker_certification` (`jobseeker_username`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=18 ;

--
-- Dumping data for table `certification`
--

INSERT INTO `certification` (`id`, `jobseeker_username`, `title`, `institution`, `year`) VALUES
(16, 'profchydon', '', '', ''),
(17, 'profchydon', 'Cert. in Word processing', 'Microsoft Institute', '1999');

-- --------------------------------------------------------

--
-- Table structure for table `education`
--

CREATE TABLE IF NOT EXISTS `education` (
  `id` int(225) NOT NULL AUTO_INCREMENT,
  `jobseeker_username` varchar(225) NOT NULL,
  `type_of_institution` varchar(225) NOT NULL,
  `institution` varchar(255) NOT NULL,
  `degree` varchar(225) NOT NULL,
  `field_of_study` varchar(225) NOT NULL,
  `year_of_graduation` varchar(225) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `jobseeker_username` (`jobseeker_username`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=26 ;

--
-- Dumping data for table `education`
--

INSERT INTO `education` (`id`, `jobseeker_username`, `type_of_institution`, `institution`, `degree`, `field_of_study`, `year_of_graduation`) VALUES
(24, 'profchydon', '', '', '', '', ''),
(25, 'profchydon', 'University', 'University of Lagos', 'B.Sc', 'Food Science', '2013');

-- --------------------------------------------------------

--
-- Table structure for table `employer`
--

CREATE TABLE IF NOT EXISTS `employer` (
  `id` int(255) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  `email` varchar(255) NOT NULL,
  `website` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  `mobile` varchar(255) NOT NULL,
  `address` varchar(255) NOT NULL,
  `size` varchar(255) NOT NULL,
  `industry_type` varchar(255) NOT NULL,
  `about` longblob NOT NULL,
  `logo` varchar(255) NOT NULL,
  `password_recovered` int(255) NOT NULL DEFAULT '0',
  `email_code` varchar(255) NOT NULL,
  `active` int(11) NOT NULL DEFAULT '0',
  `deactivated` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`,`website`),
  UNIQUE KEY `email` (`email`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=15 ;

--
-- Dumping data for table `employer`
--

INSERT INTO `employer` (`id`, `name`, `email`, `website`, `password`, `mobile`, `address`, `size`, `industry_type`, `about`, `logo`, `password_recovered`, `email_code`, `active`, `deactivated`) VALUES
(2, 'Anakle Nigeria Limited', 'info@anakle.com', 'www.anakle.com', '$2y$10$kp234ze3OpOEX9yZfaD7u.luqKVfcbRlahsKH6xiOfKfOhq1FtMVi', '08050596959', '', 'option', 'Marketing / Advertising / Communications', 0x57652061726520736d6172742068617070792070656f706c65, '', 0, '8690e80729431a6bf453cf1eedb3a97b', 1, 0),
(3, 'iamchidi webdev concept', 'info@iamchidi.com', 'www.iamchidi.com', '$2y$10$0Wjqdhp97km/w3qsInlUMe3qeo0cTC2LsiH8X5bmvhrdAyMkgvgtW', '07038696899', '#162 adetola street aguda surulere Lagos', 'option', 'Information Technology', 0x69616d636869646920697320612077656220646576656c6f706d656e74206167656e6379207468617420697320666f6375736564206f6e206265696e6720746865206e756d626572206f6e6520636c69656e7427732063686f69636520666f72207765627369746520646576656c6f706d656e74, '', 0, 'bb56d1443470f1372b92d77bf96bd6e0', 1, 0),
(4, 'Fruits&amp;Veg', 'customer@fruitsandveg.com', 'www.fruitsandveg.com', '$2y$10$GwLkq5bdtomaxO5G.4GYO.jzjzsKYOF.7.YCBRUmx2Cew8f35PWHe', '07030203050', '12A clement bassey way uyo', 'option', 'Vocational Trade and Services', 0x46727569747320616e6420566567, '', 0, 'd9dc986f5948cd562a99c206bb3ad84d', 1, 0),
(14, 'Medina Book Club Lagos', 'profchydon@gmail.com', 'www.medinalagos.com', '$2y$10$ufpkLJ36566P2munKY94c.FxilqFKWUByT./c5pZCtuO.H45lREPO', '07038696899', '4 Ochiri street elekahia port harcourt', 'option', 'Legal', 0x6b, '', 0, 'df7f739a563b7ec1efe1574fb6c0a741', 0, 0);

-- --------------------------------------------------------

--
-- Table structure for table `jobs`
--

CREATE TABLE IF NOT EXISTS `jobs` (
  `id` int(255) NOT NULL AUTO_INCREMENT,
  `status` varchar(255) NOT NULL DEFAULT 'Open',
  `employer` varchar(255) NOT NULL,
  `employer_email` varchar(255) NOT NULL,
  `description` longblob NOT NULL,
  `type` varchar(255) NOT NULL,
  `experience` varchar(255) NOT NULL,
  `location` varchar(255) NOT NULL,
  `date_posted` varchar(255) NOT NULL,
  `deadline` varchar(255) NOT NULL,
  `title` varchar(255) NOT NULL,
  `qualification` varchar(255) NOT NULL,
  `gender` varchar(225) NOT NULL,
  `category` varchar(255) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `employer_email` (`employer_email`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=18 ;

--
-- Dumping data for table `jobs`
--

INSERT INTO `jobs` (`id`, `status`, `employer`, `employer_email`, `description`, `type`, `experience`, `location`, `date_posted`, `deadline`, `title`, `qualification`, `gender`, `category`) VALUES
(3, 'Open', 'Anakle Nigeria Limited', 'info@anakle.com', 0x4170706c6963616e74732073686f756c642062652061626c6520746f206b6565702070656f706c6520636f6d6d697474656420616e64206861707079, 'Full-time', 'Nil', 'Lagos', '16 Jun 2016', '23rd July 2016', 'Human Resource Manager', 'Second Class upper and above', 'All', 'Human Resources'),
(4, 'Open', 'iamchidi webdev concept', 'info@iamchidi.com', 0x4170706c6963616e74206d75737420626520617665726167656c7920676f6f6420776974682070686f746f73686f702c20696c6c7573747261746f7220616e64206172746966696369616c2064657369676e73, '', '', '', '16 Jun 2016', 'None', 'Interface Designer', 'None', 'All', 'Creatives(Arts, Design, Fashion)'),
(7, 'Open', 'Anakle Nigeria Limited', 'info@anakle.com', '', '', '', '', '17 Jun 2016', '3rd August 2016', 'Client Service', 'Second Class Upper and above', 'Female', 'Marketing / Advertising / Communications'),
(8, 'Open', 'Anakle Nigeria Limited', 'info@anakle.com', '', '', '', '', '17 Jun 2016', '3rd August 2016', 'Client Service', 'Second Class Upper and above', 'Female', 'Marketing / Advertising / Communications'),
(9, 'Open', 'iamchidi webdev concept', 'info@iamchidi.com', 0x4170706c6963616e74206d757374206861766520676f6f64206b6e6f776c6564676520696e202048746d6c2c206373732c204a6176615363726970742c204a517565727920616e6420616e79206f746865722066726f6e742d656e64206c616e6775616765206f72206672616d65776f726b2e, 'Full-time', '1yr in Front-End development', '', '16 Jun 2016', '1st September 2016', 'Front-End Designer', 'None', 'All', 'Creatives(Arts, Design, Fashion)'),
(10, 'Open', 'iamchidi webdev concept', 'info@iamchidi.com', 0x4e696c, 'Internship', 'Nil', '', '16 Jun 2016', 'Nil', 'Human Resource Manager', 'Nil', 'Female', 'Human Resources'),
(11, 'Open', 'iamchidi webdev concept', 'info@iamchidi.com', 0x626c61, 'Part-time', 'Nil', '', '16 Jun 2016', 'Nil', 'Graphics Designer', 'Nil', 'Male', 'Creatives(Arts, Design, Fashion)'),
(12, 'Open', 'iamchidi webdev concept', 'info@iamchidi.com', 0x426c6120626c61, 'Full-time', 'Nil', '', '16 Jun 2016', 'Nil', 'Accountant', 'Nil', 'All', 'Accounting / Audit / Tax'),
(13, 'Open', 'iamchidi webdev concept', 'info@iamchidi.com', 0x4170706c6963616e74206d7573742068617665207665727920676f6f64206b6e6f776c65646765206f662068746d6c352c20637373332c206a6176615363726970742c206a51756572792c20416e67756c61724a732c20576562736572766963652c20524553542e, 'Part-time', '1yr in web development', '', '17 Jun 2016', '10th December 2016', 'Web developer', 'None', 'Male', 'Information Technology'),
(14, 'Open', 'iamchidi webdev concept', 'info@iamchidi.com', '', 'Full-time', '5yrs in project Management', '', '17 Jun 2016', 'None', 'Project Supervisor', 'Second Class Upper Divison and above', 'Male', 'Project / Programme Management'),
(15, 'Open', 'iamchidi webdev concept', 'info@iamchidi.com', 0x426c6120626c61, 'Full-time', '2yrs in  Construction and Design', '', '18 Jun 2016', 'None', 'Construction Supervisor', 'First Class Division', 'Male', 'Engineering'),
(16, 'Open', 'iamchidi webdev concept', 'info@iamchidi.com', 0x426c6120426c61, 'Fresh Graduate', 'None', '', '18 Jun 2016', '12th September 2016', 'Customer care attendant', 'Second Class lower and above', 'Female', 'Banking / Finance / Insurance'),
(17, 'Open', 'Fruits&amp;Veg', 'customer@fruitsandveg.com', 0x426c6120626c61, 'Full-time', 'None', '', '18 Jun 2016', 'None', 'Finance Manager', 'None', 'All', 'Accounting / Audit / Tax');

-- --------------------------------------------------------

--
-- Table structure for table `jobseeker`
--

CREATE TABLE IF NOT EXISTS `jobseeker` (
  `id` int(225) NOT NULL AUTO_INCREMENT,
  `username` varchar(225) NOT NULL,
  `password` varchar(225) NOT NULL,
  `first_name` varchar(225) NOT NULL,
  `middle_name` varchar(225) NOT NULL,
  `last_name` varchar(225) NOT NULL,
  `email` varchar(225) NOT NULL,
  `mobile` varchar(225) NOT NULL,
  `sex` varchar(225) NOT NULL,
  `age` varchar(225) NOT NULL,
  `address` varchar(225) NOT NULL,
  `state_of_origin` varchar(225) NOT NULL,
  `lga` varchar(225) NOT NULL,
  `state_of_residence` varchar(225) NOT NULL,
  `specialization` varchar(225) NOT NULL,
  `password_recovered` int(1) NOT NULL DEFAULT '0',
  `profile_image` varchar(225) NOT NULL,
  `about` longblob NOT NULL,
  `cv` varchar(225) NOT NULL,
  `deactivated` int(1) NOT NULL DEFAULT '0',
  `email_code` varchar(225) NOT NULL,
  `active` int(11) NOT NULL DEFAULT '0',
  `updated_profile` varchar(255) NOT NULL DEFAULT 'no',
  `allow_email` varchar(255) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  UNIQUE KEY `email` (`email`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=54 ;

--
-- Dumping data for table `jobseeker`
--

INSERT INTO `jobseeker` (`id`, `username`, `password`, `first_name`, `middle_name`, `last_name`, `email`, `mobile`, `sex`, `age`, `address`, `state_of_origin`, `lga`, `state_of_residence`, `specialization`, `password_recovered`, `profile_image`, `about`, `cv`, `deactivated`, `email_code`, `active`, `updated_profile`, `allow_email`) VALUES
(53, 'profchydon', '$2y$10$n5BXGAJPG/SpwKP2WmHqmOoOYFqkqQPUrcM19wvxQ0dzHbJDib2Ce', 'Chidi', 'Callistus', 'Nkwocha', 'profchydon@gmail.com', '07038696899', 'Male', '07/11/2016', '4 Ochiri street elekahia port harcourt', 'Akwa Ibom', 'Ibiono Ibom', 'Rivers', 'Information Technology', 0, 'img/profile/8914f03539.jpg', '', 'img/cv/589381ee8e.docx', 0, 'a99bff9aac4c9475bdb23e17b05cb382', 1, 'yes', '1');

--
-- Constraints for dumped tables
--

--
-- Constraints for table `application`
--
ALTER TABLE `application`
  ADD CONSTRAINT `jobseeker_application` FOREIGN KEY (`jobseeker_id`) REFERENCES `jobseeker` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `job_application` FOREIGN KEY (`job_id`) REFERENCES `jobs` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `certification`
--
ALTER TABLE `certification`
  ADD CONSTRAINT `jobseeker_certification` FOREIGN KEY (`jobseeker_username`) REFERENCES `jobseeker` (`username`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `education`
--
ALTER TABLE `education`
  ADD CONSTRAINT `jobseeker_education` FOREIGN KEY (`jobseeker_username`) REFERENCES `jobseeker` (`username`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `jobs`
--
ALTER TABLE `jobs`
  ADD CONSTRAINT `employer_job` FOREIGN KEY (`employer_email`) REFERENCES `employer` (`email`) ON DELETE CASCADE ON UPDATE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
