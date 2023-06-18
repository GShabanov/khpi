#! /bin/bash
curl -fsSL https://get.docker.com -o get-docker.sh
sudo sh get-docker.sh

sudo chmod 777 /var/run/docker.sock

docker run -d -p 9100:9100 --name node-exporter prom/node-exporter:latest
docker run -d -p 8080:8080 --name cadvisor-exporter google/cadvisor:latest