#!/bin/bash

destination=../src/install/ramses_scheme.sql

mysqldump --user=ramses -p --column-statistics=FALSE --protocol=socket --default-character-set=utf8 --routines --events --no-data "ramses" > $destination