-- phpMyAdmin SQL Dump
-- version 5.0.2
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Oct 05, 2020 at 08:49 AM
-- Server version: 10.4.14-MariaDB
-- PHP Version: 7.2.33

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `realtime_quiz_db`
--

-- --------------------------------------------------------

--
-- Table structure for table `answers`
--

CREATE TABLE `answers` (
  `id` int(30) NOT NULL,
  `user_id` int(30) NOT NULL,
  `quiz_id` int(30) NOT NULL,
  `question_id` int(30) NOT NULL,
  `option_id` int(30) NOT NULL,
  `is_right` tinyint(1) NOT NULL COMMENT ' 1 = right, 0 = wrong',
  `date_updated` datetime NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `answers`
--

INSERT INTO `answers` (`id`, `user_id`, `quiz_id`, `question_id`, `option_id`, `is_right`, `date_updated`) VALUES
(1, 8, 2, 6, 41, 1, '2020-10-05 09:41:38'),
(2, 9, 2, 6, 44, 0, '2020-10-05 09:31:16'),
(3, 8, 2, 7, 51, 0, '2020-10-05 11:20:01'),
(4, 9, 2, 7, 50, 1, '2020-10-05 11:20:06');

-- --------------------------------------------------------

--
-- Table structure for table `participants`
--

CREATE TABLE `participants` (
  `id` int(30) NOT NULL,
  `quiz_id` int(30) NOT NULL,
  `user_id` int(30) NOT NULL,
  `status` int(1) NOT NULL COMMENT '0 = discoonected, 1= connected'
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `participants`
--

INSERT INTO `participants` (`id`, `quiz_id`, `user_id`, `status`) VALUES
(2, 2, 8, 1),
(3, 2, 9, 1),
(4, 4, 8, 0);

-- --------------------------------------------------------

--
-- Table structure for table `questions`
--

CREATE TABLE `questions` (
  `id` int(30) NOT NULL,
  `question` text NOT NULL,
  `points` float NOT NULL,
  `qid` int(30) NOT NULL,
  `order_by` int(11) NOT NULL,
  `status` tinyint(1) NOT NULL DEFAULT 0,
  `date_updated` datetime NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `questions`
--

INSERT INTO `questions` (`id`, `question`, `points`, `qid`, `order_by`, `status`, `date_updated`) VALUES
(2, 'Sample', 5, 0, 0, 0, '2020-10-03 10:35:55'),
(3, 'Sample', 5, 0, 0, 0, '2020-10-03 10:41:50'),
(4, 'Sample', 5, 1, 0, 0, '2020-10-03 10:43:27'),
(5, 'Question 2', 5, 1, 0, 0, '2020-10-03 10:46:50'),
(6, 'Sample', 10, 2, 0, 1, '2020-10-05 09:58:30'),
(7, 'Second Question', 10, 2, 0, 0, '2020-10-03 13:12:43'),
(8, 'Sample 1', 20, 4, 0, 0, '2020-10-05 14:47:03'),
(9, 'Sample 2', 15, 4, 0, 0, '2020-10-05 14:47:22'),
(10, 'Sample 3', 15, 4, 0, 0, '2020-10-05 14:47:39');

-- --------------------------------------------------------

--
-- Table structure for table `question_opt`
--

CREATE TABLE `question_opt` (
  `id` int(30) NOT NULL,
  `option_txt` text NOT NULL,
  `question_id` int(30) NOT NULL,
  `is_right` tinyint(4) NOT NULL DEFAULT 0 COMMENT '1= right answer',
  `date_updated` datetime NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `question_opt`
--

INSERT INTO `question_opt` (`id`, `option_txt`, `question_id`, `is_right`, `date_updated`) VALUES
(17, 'Right', 2, 0, '2020-10-03 10:37:29'),
(18, 'Wrong', 2, 0, '2020-10-03 10:37:29'),
(19, 'Wrong', 2, 1, '2020-10-03 10:37:29'),
(20, 'Wrong', 2, 0, '2020-10-03 10:37:29'),
(21, 'Right', 3, 1, '2020-10-03 10:41:50'),
(22, 'Wrong', 3, 0, '2020-10-03 10:41:50'),
(23, 'Wrong', 3, 0, '2020-10-03 10:41:50'),
(24, 'Wrong', 3, 0, '2020-10-03 10:41:50'),
(33, 'Right', 4, 1, '2020-10-03 10:43:38'),
(34, 'Wrong', 4, 0, '2020-10-03 10:43:38'),
(35, 'Wrong', 4, 0, '2020-10-03 10:43:38'),
(36, 'Wrong', 4, 0, '2020-10-03 10:43:38'),
(37, 'Wrong', 5, 0, '2020-10-03 10:46:50'),
(38, 'Wrong', 5, 0, '2020-10-03 10:46:50'),
(39, 'Right', 5, 0, '2020-10-03 10:46:50'),
(40, 'Wrong', 5, 0, '2020-10-03 10:46:50'),
(41, 'Right', 6, 1, '2020-10-03 13:12:15'),
(42, 'Wrong', 6, 0, '2020-10-03 13:12:15'),
(43, 'Wrong', 6, 0, '2020-10-03 13:12:15'),
(44, 'Wrong', 6, 0, '2020-10-03 13:12:15'),
(49, 'Wrong', 7, 0, '2020-10-03 13:12:50'),
(50, 'Right', 7, 1, '2020-10-03 13:12:50'),
(51, 'Wrong', 7, 0, '2020-10-03 13:12:50'),
(52, 'Wrong', 7, 0, '2020-10-03 13:12:50'),
(53, 'Right', 8, 1, '2020-10-05 14:47:03'),
(54, 'Wrong', 8, 0, '2020-10-05 14:47:03'),
(55, 'Wrong', 8, 0, '2020-10-05 14:47:03'),
(56, 'Wrong', 8, 0, '2020-10-05 14:47:03'),
(57, 'Wrong', 9, 0, '2020-10-05 14:47:22'),
(58, 'Wrong', 9, 0, '2020-10-05 14:47:22'),
(59, 'Right', 9, 0, '2020-10-05 14:47:22'),
(60, 'Wrong', 9, 0, '2020-10-05 14:47:22'),
(61, 'Wrong', 10, 0, '2020-10-05 14:47:39'),
(62, 'Wrong', 10, 0, '2020-10-05 14:47:39'),
(63, 'Wrong', 10, 0, '2020-10-05 14:47:39'),
(64, 'Right', 10, 0, '2020-10-05 14:47:39');

-- --------------------------------------------------------

--
-- Table structure for table `quiz_list`
--

CREATE TABLE `quiz_list` (
  `id` int(30) NOT NULL,
  `title` varchar(200) NOT NULL,
  `user_id` int(20) NOT NULL,
  `status` tinyint(1) DEFAULT 0 COMMENT '0=pending,1=on-going,2=done',
  `date_updated` datetime NOT NULL DEFAULT current_timestamp() ON UPDATE current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `quiz_list`
--

INSERT INTO `quiz_list` (`id`, `title`, `user_id`, `status`, `date_updated`) VALUES
(2, 'Quizz 1', 7, 3, '2020-10-05 13:10:34'),
(4, 'Quiz 2', 7, 1, '2020-10-05 14:47:53');

-- --------------------------------------------------------

--
-- Table structure for table `users`
--

CREATE TABLE `users` (
  `id` int(30) NOT NULL,
  `name` text NOT NULL,
  `type` tinyint(4) NOT NULL DEFAULT 2 COMMENT '1 = Admin, 2= Teacher, 3 = student',
  `username` varchar(100) NOT NULL,
  `password` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Dumping data for table `users`
--

INSERT INTO `users` (`id`, `name`, `type`, `username`, `password`) VALUES
(6, 'Administrator', 1, 'admin', '0192023a7bbd73250516f069df18b500'),
(7, 'John Smith', 2, 'jsmith', '1254737c076cf867dc53d60a0364f38e'),
(8, 'Sample Student 1', 3, 'student', 'cd73502828457d15655bbd7a63fb0bc8'),
(9, 'student 2', 3, 'student2', '213ee683360d88249109c2f92789dbc3');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `answers`
--
ALTER TABLE `answers`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `participants`
--
ALTER TABLE `participants`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `questions`
--
ALTER TABLE `questions`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `question_opt`
--
ALTER TABLE `question_opt`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `quiz_list`
--
ALTER TABLE `quiz_list`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `answers`
--
ALTER TABLE `answers`
  MODIFY `id` int(30) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT for table `participants`
--
ALTER TABLE `participants`
  MODIFY `id` int(30) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT for table `questions`
--
ALTER TABLE `questions`
  MODIFY `id` int(30) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;

--
-- AUTO_INCREMENT for table `question_opt`
--
ALTER TABLE `question_opt`
  MODIFY `id` int(30) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=65;

--
-- AUTO_INCREMENT for table `quiz_list`
--
ALTER TABLE `quiz_list`
  MODIFY `id` int(30) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT for table `users`
--
ALTER TABLE `users`
  MODIFY `id` int(30) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=10;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
