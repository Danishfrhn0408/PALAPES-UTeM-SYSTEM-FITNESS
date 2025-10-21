-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jan 23, 2025 at 12:34 PM
-- Server version: 10.4.32-MariaDB
-- PHP Version: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `fit`
--

-- --------------------------------------------------------

--
-- Table structure for table `admin`
--

CREATE TABLE `admin` (
  `AdminID` varchar(10) NOT NULL,
  `name` varchar(20) NOT NULL,
  `password` varchar(10) NOT NULL,
  `numPhone` varchar(11) NOT NULL,
  `age` varchar(2) NOT NULL,
  `gender` varchar(6) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- Dumping data for table `admin`
--

INSERT INTO `admin` (`AdminID`, `name`, `password`, `numPhone`, `age`, `gender`) VALUES
('dnsh', 'ali', '011', '012', '20', 'male');

-- --------------------------------------------------------

--
-- Table structure for table `cadet`
--

CREATE TABLE `cadet` (
  `cadetid` varchar(10) NOT NULL,
  `name` varchar(20) NOT NULL,
  `numPhone` varchar(11) NOT NULL,
  `batch` varchar(10) NOT NULL,
  `password` varchar(10) NOT NULL,
  `gender` varchar(6) NOT NULL,
  `age` varchar(2) NOT NULL,
  `squad` varchar(20) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- Dumping data for table `cadet`
--

INSERT INTO `cadet` (`cadetid`, `name`, `numPhone`, `batch`, `password`, `gender`, `age`, `squad`) VALUES
('D002', 'MOHD FAUZI', '01812345512', '2025', 'ABC123', 'M', '20', 'Charlie'),
('D020', 'NUR HAFIZAH', '019-0067890', '2025', 'NUR030', 'F', '21', 'Charlie'),
('D022', 'IZZAT FAKHRUL', '019-2667711', '2024', 'IJAT01', 'M', '23', 'Bravo'),
('D024', 'MOHD NAIM ZUHAIR', '01877772890', '2023', 'Abc123', 'M', '20', 'Alpha'),
('D026', 'AHMAD AFWAN', '01113333333', '2024', 'Abc123', 'M', '20', 'Bravo'),
('D028', 'LIYANA NAJWA', '01733356789', '2023', 'Abc123', 'F', '20', 'Alpha'),
('D035', 'FATIN NAJWA', '01114444444', '2025', 'ABC123', 'F', '21', 'Charlie'),
('D040', 'DANIAL AMIN', '11111111111', '2023', 'Abc123', 'M', '20', 'Alpha'),
('D043', 'SITI ALIYAH', '01877736789', '2023', 'SITI00', 'F', '20', 'Alpha'),
('D060', 'AHMAD AMSYAR', '01222222222', '2024', 'Abc123', 'M', '20', 'Bravo'),
('Izz', 'NUR IRFAN', '11111111111', '2024', '1q2w3e4r5t', 'M', '12', 'Bravo');

-- --------------------------------------------------------

--
-- Table structure for table `coach`
--

CREATE TABLE `coach` (
  `coachid` varchar(10) NOT NULL,
  `name` varchar(20) DEFAULT NULL,
  `password` varchar(10) NOT NULL,
  `batch` varchar(10) NOT NULL,
  `squad` varchar(20) DEFAULT NULL,
  `numPhone` varchar(11) NOT NULL,
  `age` int(11) NOT NULL,
  `gender` varchar(6) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- Dumping data for table `coach`
--

INSERT INTO `coach` (`coachid`, `name`, `password`, `batch`, `squad`, `numPhone`, `age`, `gender`) VALUES
('C004', 'MOHD AZHAR ', 'ABC123', '2025', 'Charlie', '01233344789', 35, 'M'),
('C005', 'NUR NAJWA AINA', 'Abc123', '2023', 'Alpha', '01812345679', 30, 'F'),
('C006', 'ENGKU ARIF', 'ABC123', '2023', 'Alpha', '01812345678', 30, 'M'),
('C009', 'MOHD NORALIFF', 'abc', '2024', 'Bravo', '018', 20, 'M'),
('C030', 'IMANINA ANA', 'ZARA02', '2025', 'Charlie', '01812345678', 35, 'F'),
('C040', 'SITI ZAINUN', 'SITI00', '2024', 'Bravo', '01899234567', 37, 'F');

-- --------------------------------------------------------

--
-- Table structure for table `fitness_details`
--

CREATE TABLE `fitness_details` (
  `Date` date NOT NULL,
  `pushup` int(11) DEFAULT NULL,
  `pullup` int(11) DEFAULT NULL,
  `situp` int(11) DEFAULT NULL,
  `height` float(10,2) DEFAULT NULL,
  `weight` float(10,0) NOT NULL,
  `BMI` float(10,0) NOT NULL,
  `lunges` int(11) DEFAULT NULL,
  `squat` int(11) DEFAULT NULL,
  `beepTest` double DEFAULT NULL,
  `CadetID` varchar(10) NOT NULL,
  `distance` double DEFAULT NULL,
  `RecordID` int(11) NOT NULL,
  `squad` varchar(20) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- Dumping data for table `fitness_details`
--

INSERT INTO `fitness_details` (`Date`, `pushup`, `pullup`, `situp`, `height`, `weight`, `BMI`, `lunges`, `squat`, `beepTest`, `CadetID`, `distance`, `RecordID`, `squad`) VALUES
('2025-01-16', 30, 30, 30, 2.00, 65, 23, 30, 30, 10, 'D020', 10, 4, NULL),
('2025-01-16', 30, 30, 30, 1.65, 65, 24, 30, 30, 10, 'D020', 10, 5, NULL),
('2025-01-16', 30, 30, 39, 1.80, 70, 22, 39, 39, 20, 'Izz', 12, 6, NULL),
('2025-01-20', 40, 40, 40, 1.70, 66, 23, 40, 40, 9, 'D024', 11, 7, NULL),
('2025-01-20', 30, 30, 30, 1.60, 56, 22, 30, 30, 8, 'D020', 12, 8, NULL),
('2025-01-20', 30, 30, 30, 1.70, 65, 22, 30, 30, 10, 'D024', 10, 9, NULL);

-- --------------------------------------------------------

--
-- Table structure for table `unfitness`
--

CREATE TABLE `unfitness` (
  `Date` date NOT NULL,
  `pushup` int(11) DEFAULT NULL,
  `pullup` int(11) DEFAULT NULL,
  `situp` int(11) DEFAULT NULL,
  `height` float(10,2) DEFAULT NULL,
  `weight` float(10,0) NOT NULL,
  `BMI` float(10,0) NOT NULL,
  `lunges` int(11) DEFAULT NULL,
  `squat` int(11) DEFAULT NULL,
  `beepTest` double DEFAULT NULL,
  `CadetID` varchar(10) NOT NULL,
  `distance` double DEFAULT NULL,
  `RecordID` int(11) NOT NULL,
  `squad` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_bin;

--
-- Dumping data for table `unfitness`
--

INSERT INTO `unfitness` (`Date`, `pushup`, `pullup`, `situp`, `height`, `weight`, `BMI`, `lunges`, `squat`, `beepTest`, `CadetID`, `distance`, `RecordID`, `squad`) VALUES
('2025-01-15', 3, 3, 3, 2.00, 80, 25, 3, 3, 3, 'Izz', 3, 5, NULL),
('2025-01-15', 5, 5, 5, 2.00, 32, 13, 5, 5, 5, 'Izz', 5, 6, NULL),
('2025-01-15', 30, 30, 30, 2.00, 50, 17, 30, 30, 30, 'Izz', 20, 7, NULL),
('2025-01-20', 30, 30, 30, 1.68, 50, 18, 30, 30, 8, 'D024', 12, 8, NULL),
('2025-01-20', 30, 30, 30, 1.60, 65, 25, 30, 30, 8, 'D060', 13, 9, NULL);

--
-- Indexes for dumped tables
--

--
-- Indexes for table `cadet`
--
ALTER TABLE `cadet`
  ADD PRIMARY KEY (`cadetid`),
  ADD KEY `fk_squad` (`squad`);

--
-- Indexes for table `coach`
--
ALTER TABLE `coach`
  ADD PRIMARY KEY (`coachid`);

--
-- Indexes for table `fitness_details`
--
ALTER TABLE `fitness_details`
  ADD PRIMARY KEY (`RecordID`),
  ADD KEY `idx_cadetID` (`CadetID`);

--
-- Indexes for table `unfitness`
--
ALTER TABLE `unfitness`
  ADD PRIMARY KEY (`RecordID`),
  ADD KEY `fk_cadet_id` (`CadetID`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `fitness_details`
--
ALTER TABLE `fitness_details`
  MODIFY `RecordID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=10;

--
-- AUTO_INCREMENT for table `unfitness`
--
ALTER TABLE `unfitness`
  MODIFY `RecordID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=10;

--
-- Constraints for dumped tables
--

--
-- Constraints for table `unfitness`
--
ALTER TABLE `unfitness`
  ADD CONSTRAINT `fk_cadet_id` FOREIGN KEY (`CadetID`) REFERENCES `cadet` (`cadetid`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
