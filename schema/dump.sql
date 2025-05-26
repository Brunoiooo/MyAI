-- MySQL dump 10.13  Distrib 8.0.34, for Win64 (x86_64)
--
-- Host: localhost    Database: dump_neuron_network
-- ------------------------------------------------------
-- Server version	8.0.35

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `layers`
--

DROP TABLE IF EXISTS `layers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `layers` (
  `id` int NOT NULL AUTO_INCREMENT,
  `network_id` int NOT NULL,
  PRIMARY KEY (`id`),
  KEY `network_idx` (`network_id`),
  CONSTRAINT `network` FOREIGN KEY (`network_id`) REFERENCES `networks` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=40 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `layers`
--

LOCK TABLES `layers` WRITE;
/*!40000 ALTER TABLE `layers` DISABLE KEYS */;
/*!40000 ALTER TABLE `layers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `networks`
--

DROP TABLE IF EXISTS `networks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `networks` (
  `id` int NOT NULL AUTO_INCREMENT,
  `name` varchar(45) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=31 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `networks`
--

LOCK TABLES `networks` WRITE;
/*!40000 ALTER TABLE `networks` DISABLE KEYS */;
/*!40000 ALTER TABLE `networks` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `neuron_types`
--

DROP TABLE IF EXISTS `neuron_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `neuron_types` (
  `id` varchar(50) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `neuron_types`
--

LOCK TABLES `neuron_types` WRITE;
/*!40000 ALTER TABLE `neuron_types` DISABLE KEYS */;
INSERT INTO `neuron_types` VALUES ('NeuronReLU'),('NeuronSigmoidalUnipolar'),('NeuronSoftMax');
/*!40000 ALTER TABLE `neuron_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `neurons`
--

DROP TABLE IF EXISTS `neurons`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `neurons` (
  `id` int NOT NULL AUTO_INCREMENT,
  `type_id` varchar(50) NOT NULL,
  `bias` double NOT NULL,
  `layer_id` int NOT NULL,
  PRIMARY KEY (`id`),
  KEY `1_idx` (`type_id`),
  KEY `1_idx1` (`layer_id`),
  CONSTRAINT `layer` FOREIGN KEY (`layer_id`) REFERENCES `layers` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `neuron_type` FOREIGN KEY (`type_id`) REFERENCES `neuron_types` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=67 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `neurons`
--

LOCK TABLES `neurons` WRITE;
/*!40000 ALTER TABLE `neurons` DISABLE KEYS */;
/*!40000 ALTER TABLE `neurons` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `weights`
--

DROP TABLE IF EXISTS `weights`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `weights` (
  `id` int NOT NULL AUTO_INCREMENT,
  `weight` double NOT NULL,
  `neuron_id` int NOT NULL,
  PRIMARY KEY (`id`),
  KEY `d_idx` (`neuron_id`),
  CONSTRAINT `neuron` FOREIGN KEY (`neuron_id`) REFERENCES `neurons` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=103 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `weights`
--

LOCK TABLES `weights` WRITE;
/*!40000 ALTER TABLE `weights` DISABLE KEYS */;
/*!40000 ALTER TABLE `weights` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-05-26 20:17:28
