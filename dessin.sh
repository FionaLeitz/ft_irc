#!/bin/bash

# This is a minimal set of ANSI/VT100 color codes
_END=$'\x1b[0m'
_BOLD=$'\x1b[1m'
_UNDER=$'\x1b[4m'
_REV=$'\x1b[7m'

# Colors
_GREY=$'\x1b[30m'
_RED=$'\x1b[31m'
_GREEN=$'\x1b[32m'
_YELLOW=$'\x1b[33m'
_BLUE=$'\x1b[34m'
_PURPLE=$'\x1b[35m'
_CYAN=$'\x1b[36m'
_WHITE=$'\x1b[37m'

# Inverted, i.e. colored backgrounds
_IGREY=$'\x1b[40m'
_IRED=$'\x1b[41m'
_IGREEN=$'\x1b[42m'
_IYELLOW=$'\x1b[43m'
_IBLUE=$'\x1b[44m'
_IPURPLE=$'\x1b[45m'
_ICYAN=$'\x1b[46m'
_IWHITE=$'\x1b[47m'

i=0
tput cup 0 0
while IFS= read -r line
do
	prefix=$(echo "$line" | cut -c 1-20)
	suffix=$(echo "$line" | cut -c 42-)
	echo -n "$prefix"
	tput cup $i 41
	echo -e "$suffix"
	i=$((i + 1))
	tput cup $i 0
done < dessin
sleep 2;

line=".****..****..*****.*****..*********..*******..*****..***..*."
y=21; 
x=0;

# Découper la ligne en segments entre les points
IFS='.' read -ra segments <<< "$line"

for segment in "${segments[@]}"; do
    # Afficher le segment à la position actuelle (x)
    tput cup "$x" "$y"
    echo "$segment"
	sleep 1;
	x=$((x + 2))
	y=$((y + 1))
done
# while IFS= read -r line
# do
# 	prefix=$(echo "$line" | cut -c 1-20)
# 	red=$(echo "$line" | cut -c 21-25)
# 	yellow=$(echo "$line" | cut -c 26-28)
# 	green=$(echo "$line" | cut -c 29-31)
# 	blue=$(echo "$line" | cut -c 32-34)
# 	purple=$(echo "$line" | cut -c 35-41)
# 	suffix=$(echo "$line" | cut -c 42-)
# 	echo -n "$prefix"
# 	echo -n "${_RED}$red${_END}";
# 	echo -n "                 ";
# 	echo -e "$suffix"
# done < dessin
# sleep 0.02;
# clear;
# while IFS= read -r line
# do
# 	prefix=$(echo "$line" | cut -c 1-20)
# 	red=$(echo "$line" | cut -c 21-25)
# 	yellow=$(echo "$line" | cut -c 26-28)
# 	green=$(echo "$line" | cut -c 29-31)
# 	blue=$(echo "$line" | cut -c 32-34)
# 	purple=$(echo "$line" | cut -c 35-41)
# 	suffix=$(echo "$line" | cut -c 42-)
# 	echo -n "$prefix"
# 	echo -n "${_RED}$red${_END}";
# 	echo -n "${_YELLOW}$yellow${_END}";
# 	echo -n "               ";
# 	echo -e "$suffix"
# done < dessin
# sleep 0.02;
# clear;
# while IFS= read -r line
# do
# 	prefix=$(echo "$line" | cut -c 1-20)
# 	red=$(echo "$line" | cut -c 21-25)
# 	yellow=$(echo "$line" | cut -c 26-28)
# 	green=$(echo "$line" | cut -c 29-31)
# 	blue=$(echo "$line" | cut -c 32-34)
# 	purple=$(echo "$line" | cut -c 35-41)
# 	suffix=$(echo "$line" | cut -c 42-)
# 	echo -n "$prefix"
# 	echo -n "${_RED}$red${_END}";
# 	echo -n "${_YELLOW}$yellow${_END}";
# 	echo -n "${_GREEN}$green${_END}";
# 	echo -n "           ";
# 	echo -e "$suffix"
# done < dessin
# sleep 0.02;
# clear;
# while IFS= read -r line
# do
# 	prefix=$(echo "$line" | cut -c 1-20)
# 	red=$(echo "$line" | cut -c 21-25)
# 	yellow=$(echo "$line" | cut -c 26-28)
# 	green=$(echo "$line" | cut -c 29-31)
# 	blue=$(echo "$line" | cut -c 32-34)
# 	purple=$(echo "$line" | cut -c 35-41)
# 	suffix=$(echo "$line" | cut -c 42-)
# 	echo -n "$prefix"
# 	echo -n "${_RED}$red${_END}";
# 	echo -n "${_YELLOW}$yellow${_END}";
# 	echo -n "${_GREEN}$green${_END}";
# 	echo -n "${_BLUE}$blue${_END}";
# 	echo -n "         ";
# 	echo -e "$suffix"
# done < dessin
# sleep 0.02;
# clear;
# while IFS= read -r line
# do
# 	prefix=$(echo "$line" | cut -c 1-20)
# 	red=$(echo "$line" | cut -c 21-25)
# 	yellow=$(echo "$line" | cut -c 26-28)
# 	green=$(echo "$line" | cut -c 29-31)
# 	blue=$(echo "$line" | cut -c 32-34)
# 	purple=$(echo "$line" | cut -c 35-41)
# 	suffix=$(echo "$line" | cut -c 42-)
# 	echo -n "$prefix"
# 	echo -n "${_RED}$red${_END}";
# 	echo -n "${_YELLOW}$yellow${_END}";
# 	echo -n "${_GREEN}$green${_END}";
# 	echo -n "${_BLUE}$blue${_END}";
# 	echo -n "${_PURPLE}$purple${_END}";
# 	echo -e "$suffix"
# done < dessin
