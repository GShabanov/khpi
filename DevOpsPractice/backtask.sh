#!/bin/bash
echo backup time!
test ! -d /home/gennady/backup/ && { mkdir -p /home/gennady/backup/ ; }
cp /home/gennady/txt/*.txt /home/gennady/backup/

