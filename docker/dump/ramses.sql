ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY 'admin';
CREATE USER 'admin'@'localhost' IDENTIFIED BY 'admin';
GRANT ALL PRIVILEGES ON *.* TO 'admin'@'localhost' WITH GRANT OPTION;
CREATE USER 'ramses'@'localhost' IDENTIFIED WITH mysql_native_password BY 'ramses';GRANT ALL PRIVILEGES ON *.* TO 'ramses'@'localhost';ALTER USER 'ramses'@'localhost' REQUIRE NONE WITH MAX_QUERIES_PER_HOUR 0 MAX_CONNECTIONS_PER_HOUR 0 MAX_UPDATES_PER_HOUR 0 MAX_USER_CONNECTIONS 0;GRANT ALL PRIVILEGES ON `ramses`.* TO 'ramses'@'localhost';