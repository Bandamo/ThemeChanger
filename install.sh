#! /bin/bash

initial=$(pwd)
echo -e "\e[1;31m Compilation du projet \e[1;37m"
DIR=$(dirname $0)
echo "Chemin relatif du projet :"$DIR
cd $(DIR)/build
