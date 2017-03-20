SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;


CREATE TABLE `assets` (
  `id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `shortName` varchar(15) NOT NULL,
  `latestUpdate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `assetstatuses` (
  `id` int(11) NOT NULL,
  `stageId` int(11) NOT NULL,
  `assetId` int(11) NOT NULL,
  `statusId` int(11) DEFAULT NULL,
  `comment` text NOT NULL,
  `latestUpdate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `projects` (
  `id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `shortName` varchar(15) NOT NULL,
  `latestUpdate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `projectstage` (
  `id` int(11) NOT NULL,
  `stageId` int(11) NOT NULL,
  `projectId` int(11) NOT NULL,
  `stageOrder` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `shotassets` (
  `id` int(11) NOT NULL,
  `shotId` int(11) NOT NULL,
  `assetId` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `shots` (
  `id` int(11) NOT NULL,
  `name` varchar(50) NOT NULL,
  `projectId` int(11) NOT NULL,
  `duration` float NOT NULL DEFAULT '0',
  `shotOrder` int(11) NOT NULL DEFAULT '0',
  `latestUpdate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `shotstatuses` (
  `id` int(11) NOT NULL,
  `statusId` int(11) DEFAULT NULL,
  `shotId` int(11) NOT NULL,
  `stageId` int(11) NOT NULL,
  `comment` text
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `stages` (
  `id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `shortName` varchar(15) NOT NULL,
  `type` varchar(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `status` (
  `id` int(11) NOT NULL,
  `name` varchar(255) NOT NULL,
  `shortName` varchar(15) NOT NULL,
  `color` varchar(6) NOT NULL,
  `description` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `users` (
  `id` int(11) NOT NULL,
  `firstName` varchar(255) NOT NULL,
  `lastName` varchar(255) NOT NULL,
  `userName` varchar(255) NOT NULL,
  `password` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;


ALTER TABLE `assets`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`shortName`,`name`);

ALTER TABLE `assetstatuses`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `stage` (`stageId`,`assetId`),
  ADD KEY `assetId` (`assetId`),
  ADD KEY `statusId` (`statusId`);

ALTER TABLE `projects`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`name`,`shortName`);

ALTER TABLE `projectstage`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `projectStage` (`stageId`,`projectId`),
  ADD KEY `projectId` (`projectId`);

ALTER TABLE `shotassets`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `asset` (`shotId`,`assetId`),
  ADD KEY `assetId` (`assetId`);

ALTER TABLE `shots`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`name`,`projectId`),
  ADD KEY `projectId` (`projectId`);

ALTER TABLE `shotstatuses`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `status` (`statusId`,`shotId`,`stageId`),
  ADD KEY `stageId` (`stageId`),
  ADD KEY `shotId` (`shotId`);

ALTER TABLE `stages`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`name`,`shortName`),
  ADD UNIQUE KEY `name_2` (`name`,`shortName`);

ALTER TABLE `status`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `name` (`name`,`shortName`);

ALTER TABLE `users`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `userName` (`userName`);


ALTER TABLE `assets`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
ALTER TABLE `assetstatuses`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
ALTER TABLE `projects`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
ALTER TABLE `projectstage`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
ALTER TABLE `shotassets`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
ALTER TABLE `shots`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
ALTER TABLE `shotstatuses`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
ALTER TABLE `stages`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
ALTER TABLE `status`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
ALTER TABLE `users`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

ALTER TABLE `assetstatuses`
  ADD CONSTRAINT `assetstatuses_ibfk_1` FOREIGN KEY (`stageId`) REFERENCES `stages` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `assetstatuses_ibfk_2` FOREIGN KEY (`assetId`) REFERENCES `assets` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `assetstatuses_ibfk_3` FOREIGN KEY (`statusId`) REFERENCES `status` (`id`) ON DELETE SET NULL ON UPDATE SET NULL;

ALTER TABLE `projectstage`
  ADD CONSTRAINT `projectstage_ibfk_1` FOREIGN KEY (`stageId`) REFERENCES `stages` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `projectstage_ibfk_2` FOREIGN KEY (`projectId`) REFERENCES `projects` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

ALTER TABLE `shotassets`
  ADD CONSTRAINT `shotassets_ibfk_1` FOREIGN KEY (`assetId`) REFERENCES `assets` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `shotassets_ibfk_2` FOREIGN KEY (`shotId`) REFERENCES `shots` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

ALTER TABLE `shots`
  ADD CONSTRAINT `shots_ibfk_1` FOREIGN KEY (`projectId`) REFERENCES `projects` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;

ALTER TABLE `shotstatuses`
  ADD CONSTRAINT `shotstatuses_ibfk_1` FOREIGN KEY (`stageId`) REFERENCES `stages` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `shotstatuses_ibfk_2` FOREIGN KEY (`shotId`) REFERENCES `shots` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `shotstatuses_ibfk_3` FOREIGN KEY (`statusId`) REFERENCES `status` (`id`) ON DELETE SET NULL ON UPDATE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
