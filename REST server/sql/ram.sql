-- phpMyAdmin SQL Dump
-- version 4.6.4
-- https://www.phpmyadmin.net/
--
-- Client :  127.0.0.1
-- Généré le :  Ven 07 Juillet 2017 à 16:09
-- Version du serveur :  5.7.14
-- Version de PHP :  5.6.25

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données :  `ram`
--

-- --------------------------------------------------------

--
-- Structure de la table `assets`
--

CREATE TABLE `assets` (
  `id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `shortName` varchar(15) NOT NULL,
  `stageId` int(11) NOT NULL,
  `statusId` int(11) DEFAULT NULL,
  `projectId` int(11) DEFAULT NULL,
  `comment` text NOT NULL,
  `latestUpdate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `assetstatuses`
--

CREATE TABLE `assetstatuses` (
  `id` int(11) NOT NULL,
  `assetId` int(11) NOT NULL,
  `shotId` int(11) NOT NULL,
  `latestUpdate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `projects`
--

CREATE TABLE `projects` (
  `id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `shortName` varchar(15) NOT NULL,
  `latestUpdate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `projectshot`
--

CREATE TABLE `projectshot` (
  `id` int(11) NOT NULL,
  `shotId` int(11) NOT NULL,
  `projectId` int(11) NOT NULL,
  `shotOrder` int(11) NOT NULL,
  `latestUpdate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `projectstage`
--

CREATE TABLE `projectstage` (
  `id` int(11) NOT NULL,
  `stageId` int(11) NOT NULL,
  `projectId` int(11) NOT NULL,
  `stageOrder` int(11) NOT NULL DEFAULT '0',
  `latestUpdate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `shots`
--

CREATE TABLE `shots` (
  `id` int(11) NOT NULL,
  `name` varchar(50) NOT NULL,
  `duration` float NOT NULL DEFAULT '0',
  `latestUpdate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `stages`
--

CREATE TABLE `stages` (
  `id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `shortName` varchar(15) NOT NULL,
  `autoCreateAssets` tinyint(1) NOT NULL DEFAULT '0',
  `latestUpdate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- --------------------------------------------------------

--
-- Structure de la table `status`
--

CREATE TABLE `status` (
  `id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `shortName` varchar(15) NOT NULL,
  `color` varchar(7) NOT NULL,
  `description` text NOT NULL,
  `latestUpdate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Contenu de la table `status`
--

INSERT INTO `status` (`id`, `name`, `shortName`, `color`, `description`, `latestUpdate`) VALUES
(1, 'Stand by', 'STB', '#6d6d6d', '', '2017-06-27 15:39:30'),
(2, 'To do', 'TODO', '#00aaff', '', '2017-06-27 15:39:42'),
(3, 'Retake', 'RTK', '#aa0000', '', '2017-06-27 15:39:52'),
(4, 'OK', 'OK', '#00aa00', '', '2017-06-27 15:40:00'),
(6, 'Check', 'CHK', '#ffaa00', '', '2017-06-27 15:41:49'),
(7, 'Work in progress', 'WIP', '#d1bf61', '', '2017-07-04 14:12:50');

-- --------------------------------------------------------

--
-- Structure de la table `users`
--

CREATE TABLE `users` (
  `id` int(11) NOT NULL,
  `firstName` varchar(255) NOT NULL,
  `lastName` varchar(255) NOT NULL,
  `userName` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL,
  `latestUpdate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Contenu de la table `users`
--

INSERT INTO `users` (`id`, `firstName`, `lastName`, `userName`, `password`, `latestUpdate`) VALUES
(1, '', '', 'Admin', 'd7e6961661419ce9e5650b51e3fd01ae8a97316ec840e465ec2254faf8f963390d928cb6dc9035a2fb1fbd8caa65def5c11fce4007c8d4814a14ec5f8b479f6e', '2017-07-07 15:55:57');

--
-- Index pour les tables exportées
--

--
-- Index pour la table `assets`
--
ALTER TABLE `assets`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `assetName` (`name`,`shortName`,`projectId`),
  ADD KEY `statusId` (`statusId`),
  ADD KEY `stageId` (`stageId`),
  ADD KEY `projectId` (`projectId`);

--
-- Index pour la table `assetstatuses`
--
ALTER TABLE `assetstatuses`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `assetId` (`assetId`,`shotId`) USING BTREE,
  ADD KEY `shotId` (`shotId`);

--
-- Index pour la table `projects`
--
ALTER TABLE `projects`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`name`,`shortName`);

--
-- Index pour la table `projectshot`
--
ALTER TABLE `projectshot`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `shotId` (`shotId`,`projectId`),
  ADD KEY `projectId` (`projectId`);

--
-- Index pour la table `projectstage`
--
ALTER TABLE `projectstage`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `projectStage` (`stageId`,`projectId`),
  ADD KEY `projectId` (`projectId`);

--
-- Index pour la table `shots`
--
ALTER TABLE `shots`
  ADD PRIMARY KEY (`id`);

--
-- Index pour la table `stages`
--
ALTER TABLE `stages`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`name`,`shortName`),
  ADD UNIQUE KEY `name_2` (`name`,`shortName`);

--
-- Index pour la table `status`
--
ALTER TABLE `status`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`name`,`shortName`);

--
-- Index pour la table `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `userName` (`userName`);

--
-- AUTO_INCREMENT pour les tables exportées
--

--
-- AUTO_INCREMENT pour la table `assets`
--
ALTER TABLE `assets`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT pour la table `assetstatuses`
--
ALTER TABLE `assetstatuses`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT pour la table `projects`
--
ALTER TABLE `projects`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT pour la table `projectshot`
--
ALTER TABLE `projectshot`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT pour la table `projectstage`
--
ALTER TABLE `projectstage`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT pour la table `shots`
--
ALTER TABLE `shots`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT pour la table `stages`
--
ALTER TABLE `stages`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT pour la table `status`
--
ALTER TABLE `status`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- AUTO_INCREMENT pour la table `users`
--
ALTER TABLE `users`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
--
-- Contraintes pour les tables exportées
--

--
-- Contraintes pour la table `assets`
--
ALTER TABLE `assets`
  ADD CONSTRAINT `assets_ibfk_1` FOREIGN KEY (`statusId`) REFERENCES `status` (`id`) ON DELETE SET NULL ON UPDATE CASCADE,
  ADD CONSTRAINT `assets_ibfk_2` FOREIGN KEY (`stageId`) REFERENCES `stages` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `assets_ibfk_3` FOREIGN KEY (`projectId`) REFERENCES `projects` (`id`) ON DELETE SET NULL ON UPDATE CASCADE;

--
-- Contraintes pour la table `assetstatuses`
--
ALTER TABLE `assetstatuses`
  ADD CONSTRAINT `assetstatuses_ibfk_2` FOREIGN KEY (`assetId`) REFERENCES `assets` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `assetstatuses_ibfk_3` FOREIGN KEY (`shotId`) REFERENCES `shots` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Contraintes pour la table `projectshot`
--
ALTER TABLE `projectshot`
  ADD CONSTRAINT `projectshot_ibfk_1` FOREIGN KEY (`shotId`) REFERENCES `shots` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `projectshot_ibfk_2` FOREIGN KEY (`projectId`) REFERENCES `projects` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Contraintes pour la table `projectstage`
--
ALTER TABLE `projectstage`
  ADD CONSTRAINT `projectstage_ibfk_1` FOREIGN KEY (`stageId`) REFERENCES `stages` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `projectstage_ibfk_2` FOREIGN KEY (`projectId`) REFERENCES `projects` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
