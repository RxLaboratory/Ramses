#!/bin/bash

destination=ramses_data.sql

mysqldump --user=admin -p --column-statistics=FALSE --protocol=socket --default-character-set=utf8 --no-create-info=TRUE --skip-triggers "ramses" > $destination