-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Sep 15, 2022 at 05:33 PM
-- Server version: 10.4.24-MariaDB
-- PHP Version: 8.1.6

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `smashjob`
--
CREATE DATABASE IF NOT EXISTS `smashjob` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
USE `smashjob`;

-- --------------------------------------------------------

--
-- Table structure for table `application`
--

CREATE TABLE `application` (
  `id` int(255) NOT NULL,
  `jobseeker_id` int(255) NOT NULL,
  `job_id` int(255) NOT NULL,
  `cv` varchar(255) NOT NULL,
  `additional_info` blob NOT NULL,
  `date_applied` varchar(255) NOT NULL,
  `reviewed` varchar(255) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `application`
--

INSERT INTO `application` (`id`, `jobseeker_id`, `job_id`, `cv`, `additional_info`, `date_applied`, `reviewed`) VALUES
(7, 54, 18, '', 0x49207265616c6c79206e6565642074686973204a6f622e, '03/09/2022', '0'),
(8, 55, 18, '', 0x49206e6565642074686973204a6f62206173662e, '03/09/2022', '0'),
(9, 55, 18, '', 0x49206e6565642074686973204a6f622061736621, '03/09/2022', '0'),
(10, 56, 18, '', 0x49206e6565642074686973206a6f62, '06/09/2022', '0'),
(11, 57, 18, '', 0x6e616d653a20656e6963650d0a656475636174696f6e3a20626163616c6f7220646567726565, '07/09/2022', '0'),
(12, 57, 19, '', 0x7465616368696e67, '07/09/2022', '0'),
(13, 57, 20, '', '', '07/09/2022', '0'),
(14, 58, 23, '', 0x49206e6565642074686973206a6f62, '08/09/2022', '0'),
(15, 58, 18, 'img/cv/58fd50c860.txt', 0x7468697320697320612074657374, '10/09/2022', '0'),
(16, 58, 18, 'img/cv/58fd50c860.txt', '', '10/09/2022', '0'),
(17, 58, 18, 'img/cv/58fd50c860.txt', '', '10/09/2022', '0'),
(18, 58, 24, 'img/cv/58fd50c860.txt', '', '10/09/2022', '0'),
(19, 58, 18, 'img/cv/58fd50c860.txt', '', '10/09/2022', '0');

-- --------------------------------------------------------

--
-- Table structure for table `certification`
--

CREATE TABLE `certification` (
  `id` int(225) NOT NULL,
  `jobseeker_username` varchar(225) NOT NULL,
  `title` varchar(225) NOT NULL,
  `institution` varchar(225) NOT NULL,
  `year` varchar(225) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `certification`
--

INSERT INTO `certification` (`id`, `jobseeker_username`, `title`, `institution`, `year`) VALUES
(16, 'profchydon', '', '', ''),
(17, 'profchydon', 'Cert. in Word processing', 'Microsoft Institute', '1999'),
(18, 'Jean-louis', '', '', ''),
(19, 'Daniel', '', '', ''),
(20, 'Daniel', '', '', ''),
(21, 'Test', '', '', ''),
(22, 'Test', 'PhD', 'ULK', '2022'),
(23, 'eunice', '', '', ''),
(24, 'eunice', 'programming', 'klab', '2020'),
(25, 'Raoul', '', '', ''),
(26, 'Raoul', 'Developer', 'K-Lab', '2022'),
(27, 'Raoul', 'Savings', 'japon wealth', '2022-09-10'),
(28, 'Raoul', 'business', 'self', '2020-01-11');

-- --------------------------------------------------------

--
-- Table structure for table `education`
--

CREATE TABLE `education` (
  `id` int(225) NOT NULL,
  `jobseeker_username` varchar(225) NOT NULL,
  `type_of_institution` varchar(225) NOT NULL,
  `institution` varchar(255) NOT NULL,
  `degree` varchar(225) NOT NULL,
  `field_of_study` varchar(225) NOT NULL,
  `year_of_graduation` varchar(225) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `education`
--

INSERT INTO `education` (`id`, `jobseeker_username`, `type_of_institution`, `institution`, `degree`, `field_of_study`, `year_of_graduation`) VALUES
(24, 'profchydon', '', '', '', '', ''),
(25, 'profchydon', 'University', 'University of Lagos', 'B.Sc', 'Food Science', '2013'),
(26, 'Jean-louis', '', '', '', '', ''),
(27, 'Daniel', '', '', '', '', ''),
(28, 'Daniel', '', '', '', '', ''),
(29, 'Test', '', '', '', '', ''),
(30, 'Test', 'University', 'Salem University', 'B.Sc', 'Computer Science', '2020'),
(31, 'eunice', '', '', '', '', ''),
(32, 'eunice', 'University', 'University of Education', 'B.Sc', 'computer science', '2022'),
(33, 'Raoul', '', '', '', '', ''),
(34, 'Raoul', 'University', 'University of Education', 'B.Sc', 'Computer Science', '2022'),
(36, 'Raoul', '', 'ST francois xavier de bruxel', 'High School (S.S.C.E)', 'social', '2019-09-19');

-- --------------------------------------------------------

--
-- Table structure for table `employer`
--

CREATE TABLE `employer` (
  `id` int(255) NOT NULL,
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
  `password_recovered` int(255) NOT NULL DEFAULT 0,
  `email_code` varchar(255) NOT NULL,
  `active` int(11) NOT NULL DEFAULT 1,
  `deactivated` int(11) NOT NULL DEFAULT 0
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `employer`
--

INSERT INTO `employer` (`id`, `name`, `email`, `website`, `password`, `mobile`, `address`, `size`, `industry_type`, `about`, `logo`, `password_recovered`, `email_code`, `active`, `deactivated`) VALUES
(14, 'Medina Book Club Lagos', 'profchydon@gmail.com', 'www.medinalagos.com', '$2y$10$ufpkLJ36566P2munKY94c.FxilqFKWUByT./c5pZCtuO.H45lREPO', '07038696899', '4 Ochiri street elekahia port harcourt', 'option', 'Legal', 0x6b, '', 0, 'df7f739a563b7ec1efe1574fb6c0a741', 1, 0),
(17, 'SneakerPlug', 'sneakerplug@yahoo.com', 'sneakerplug.com', '$2y$10$jE5v92xnUYBHk356C9a1DemoGWg2gtl17lrYmq/8n5myaD7aSfNtK', '+250786200013', 'Kigali', 'option', 'Legal', 0x747474, '', 0, '42d2a75c1bd951476825869a395636a8', 1, 0),
(18, 'Kingakati', 'kingakati@exa.com', 'kingakati.com', '$2y$10$kDVh9pIu5VkKEhK1kyV5m.npVyXqlXNp7caWTmUif1oaz0yTOeIMu', '+250786200013', 'Kigali/Rwanda', 'big', 'Manufacturing / Production', 0x54686973206973206120626172, '', 0, '852ecc50ec70dd99b63c21553a1e6f54', 0, 0),
(19, 'Aly E-Store', 'alyestore@gmail.com', 'www.alyestore.com', '$2y$10$EKGPChjGGed2GUJL8nuqku.vTypX5nlsDKcCeTaBH.yWO3hJVzk0K', '+250739438471', 'DRC/North-Kivu/Goma/Katindo', 'medium', 'Sales/Business Development', 0x57652064656c697665722073686f657320776f726c6420776964652e, '', 0, 'ac5f0bcfd0ef4e43502066c6561cc5b8', 1, 0);

-- --------------------------------------------------------

--
-- Table structure for table `jobs`
--

CREATE TABLE `jobs` (
  `id` int(255) NOT NULL,
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
  `category` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `jobs`
--

INSERT INTO `jobs` (`id`, `status`, `employer`, `employer_email`, `description`, `type`, `experience`, `location`, `date_posted`, `deadline`, `title`, `qualification`, `gender`, `category`) VALUES
(18, 'Open', 'SneakerPlug', 'sneakerplug@yahoo.com', 0x536f6d656f6e652077686f2077696c6c2062652061626c6520746f206d616e616765206120636f6d70616e7926233033393b732073746f636b2e, 'Part-time', '0 Year', '', '03 Sep 2022', '01/December/2022', 'Manager', 'Accounting', 'All', 'Customer Service'),
(19, 'Open', 'SneakerPlug', 'sneakerplug@yahoo.com', 0x6b6e6f7720656e676c697368, 'Part-time', '2 years', '', '07 Sep 2022', '01/December/2022', 'Teacher', 'teacher', 'All', 'Education/Teaching/Training'),
(20, 'Open', 'SneakerPlug', 'sneakerplug@yahoo.com', 0x6e656564, 'Fresh Graduate', '32 years', '', '07 Sep 2022', '20/01/2023', 'Manager', 'English speaker', 'Female', 'Education/Teaching/Training'),
(21, 'Open', 'SneakerPlug', 'sneakerplug@yahoo.com', 0x746f2061646d696e69737472617465206120646174612063656e746572, 'Full-time', '0', '', '07 Sep 2022', '12/11/2022', 'Web Administrator', 'Web Developer', 'All', 'Administration &amp; Office Support'),
(22, 'Open', 'SneakerPlug', 'sneakerplug@yahoo.com', 0x4353532c204a732c204d7953514c, 'Full-time', '2 years', '', '07 Sep 2022', '06/September/2022', 'Programmer', 'Full stack', 'All', 'Engineering'),
(23, 'Open', 'Aly E-Store', 'alyestore@gmail.com', 0x5468652063616e646964617465206d75737420686176652061206d61737465722064656772656520696e20636c65616e696e67, 'Part-time', '1 year experience', '', '08 Sep 2022', '20th September', 'Cleaner', 'Professional cleaner', 'Female', 'Creatives(Arts, Design, Fashion)'),
(24, 'Open', 'SneakerPlug', 'sneakerplug@yahoo.com', 0x54657374, 'Full-time', 'None', '', '10 Sep 2022', '12/12/2022', 'Test', 'None', 'Female', 'Customer Service');

-- --------------------------------------------------------

--
-- Table structure for table `jobseeker`
--

CREATE TABLE `jobseeker` (
  `id` int(225) NOT NULL,
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
  `password_recovered` int(1) NOT NULL DEFAULT 0,
  `profile_image` varchar(225) NOT NULL,
  `about` longblob NOT NULL,
  `cv` varchar(225) NOT NULL,
  `deactivated` int(1) NOT NULL DEFAULT 0,
  `email_code` varchar(225) NOT NULL,
  `active` int(11) NOT NULL DEFAULT 1,
  `updated_profile` varchar(255) NOT NULL DEFAULT 'no',
  `allow_email` varchar(255) NOT NULL DEFAULT '1'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `jobseeker`
--

INSERT INTO `jobseeker` (`id`, `username`, `password`, `first_name`, `middle_name`, `last_name`, `email`, `mobile`, `sex`, `age`, `address`, `state_of_origin`, `lga`, `state_of_residence`, `specialization`, `password_recovered`, `profile_image`, `about`, `cv`, `deactivated`, `email_code`, `active`, `updated_profile`, `allow_email`) VALUES
(53, 'profchydon', '$2y$10$n5BXGAJPG/SpwKP2WmHqmOoOYFqkqQPUrcM19wvxQ0dzHbJDib2Ce', 'Chidi', 'Callistus', 'Nkwocha', 'profchydon@gmail.com', '07038696899', 'Male', '07/11/2016', '4 Ochiri street elekahia port harcourt', 'Akwa Ibom', 'Ibiono Ibom', 'Rivers', 'Information Technology', 0, 'img/profile/8914f03539.jpg', '', 'img/cv/589381ee8e.docx', 0, 'a99bff9aac4c9475bdb23e17b05cb382', 1, 'yes', '1'),
(54, 'Jean-louis', '$2y$10$jddmQ6HtVhmv4gC2L.9dD.PbSQWlMn2xKG1i//GBhgxcrSbck/32y', 'Jean-louis', 'Raoul', 'Mukaba', 'jeanlouismukaba01@yahoo.com', '+250786200013', 'Male', '', '', '', '', '', '', 0, '', '', '', 0, '08a40b6fe32bdc7bb47231a292d37f0f', 1, 'yes', '1'),
(55, 'Daniel', '$2y$10$uf8EzAC3yuop6k3F1q6vV.OqDItc/xrVBFDKjSWcMQiGmUxsYatpy', 'Daniel', '', 'Munoka', 'danielmunoka@exa.com', '', '', '', '', '', '', '', '', 0, '', '', '', 0, 'd23e10183b99543379ee8fa9afd56f8e', 1, 'yes', '1'),
(56, 'Test', '$2y$10$ioXE.qgx8xKI46Ujp6JXx.JiSnfdFIyWLnQybsTdHU14VP.JmxChS', 'Test', 'Sijuwe', 'Test', 'test@test.test', '+250786200013', 'Male', '06/09/2022', 'Kingakati', 'Ekiti', 'Ado', 'Gombe', 'Engineering', 0, '', 0x48657920616d2070617373696f6e6174652062792077656220646576656c6f706d656e742021, '', 0, '125d46c8d0e5efe21a160db7aecdb92f', 1, 'yes', '1'),
(57, 'eunice', '$2y$10$t6W76UQoql92Y1fysEncZOi8AkVr7B3zEneq5sfKGMzQ1q4vqMO6u', 'KAYIBANDA', 'fille', 'Eunice', 'eunicekayibanda@gmail,com', '0791240799', 'Female', '06/02/1997', 'gisozi kigali', 'Yobe', 'Karawa', 'Gombe', 'Customer Service', 0, '', 0x6920776f756c64206c696b6520746f20626520636f6e6669726d6564, '', 0, '1e8eac3fbae2de95567fe729a55c8a34', 1, 'yes', '1'),
(58, 'Raoul', '$2y$10$p/N0PTCqzwWCtoyMWSf9Yuhbkr46D7wdzlc6QHrVshnTT4TSIeBJ2', 'Raoul', 'k.', 'Mukaba', 'raoul@gmail.com', '0786200013', 'Male', '06/09/1940', 'Kigali-Rwanda', 'Abia', '', 'Abia', 'Engineering', 0, 'img/profile/58042646b9.jpg', 0x66756c6c20737461636b20646576656c6f7065722c20656e6a6f7920636f64696e6720746f206368616e67652074686520776f726c642e, 'img/cv/58fd50c860.txt', 0, '397419d0d880deceea8de185671f98ba', 1, 'yes', '1');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `application`
--
ALTER TABLE `application`
  ADD PRIMARY KEY (`id`),
  ADD KEY `jobseeker_id` (`jobseeker_id`),
  ADD KEY `job_id` (`job_id`);

--
-- Indexes for table `certification`
--
ALTER TABLE `certification`
  ADD PRIMARY KEY (`id`),
  ADD KEY `jobseeker_certification` (`jobseeker_username`);

--
-- Indexes for table `education`
--
ALTER TABLE `education`
  ADD PRIMARY KEY (`id`),
  ADD KEY `jobseeker_username` (`jobseeker_username`);

--
-- Indexes for table `employer`
--
ALTER TABLE `employer`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`name`,`website`),
  ADD UNIQUE KEY `email` (`email`);

--
-- Indexes for table `jobs`
--
ALTER TABLE `jobs`
  ADD PRIMARY KEY (`id`),
  ADD KEY `employer_email` (`employer_email`);

--
-- Indexes for table `jobseeker`
--
ALTER TABLE `jobseeker`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `email` (`email`),
  ADD UNIQUE KEY `username` (`username`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `application`
--
ALTER TABLE `application`
  MODIFY `id` int(255) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=20;

--
-- AUTO_INCREMENT for table `certification`
--
ALTER TABLE `certification`
  MODIFY `id` int(225) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=29;

--
-- AUTO_INCREMENT for table `education`
--
ALTER TABLE `education`
  MODIFY `id` int(225) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=37;

--
-- AUTO_INCREMENT for table `employer`
--
ALTER TABLE `employer`
  MODIFY `id` int(255) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=20;

--
-- AUTO_INCREMENT for table `jobs`
--
ALTER TABLE `jobs`
  MODIFY `id` int(255) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=25;

--
-- AUTO_INCREMENT for table `jobseeker`
--
ALTER TABLE `jobseeker`
  MODIFY `id` int(225) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=59;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `application`
--
ALTER TABLE `application`
  ADD CONSTRAINT `job_application` FOREIGN KEY (`job_id`) REFERENCES `jobs` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `jobseeker_application` FOREIGN KEY (`jobseeker_id`) REFERENCES `jobseeker` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

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
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
