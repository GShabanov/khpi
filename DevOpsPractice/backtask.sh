#!/bin/bash
echo backup time!
test ! -d /home/gennady/backup/ && { mkdir -p /home/gennady/backup/ ; }
cp /home/gennady/txt/*.dat /home/gennady/backup/

