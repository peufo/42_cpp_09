#!/bin/bash

LEAKS_CHECK=true

if [ $(uname) = "Linux" ]; then
	LEAKS_CMD="valgrind"
	LEAKS_CMD+=" --leak-check=full"
	LEAKS_CMD+=" --track-fds=yes"
	LEAKS_CMD+=" --show-leak-kinds=all"
	LEAKS_CMD+=" --track-origins=yes"
	LEAKS_CMD+=" --log-file=leaks.log"
	#LEAKS_CMD+=" --trace-children=yes"
else
	LEAKS_CMD="leaks -quiet --atExit --"
	LEAKS_CMD=""
fi

watch() {
	STATE_A=""
	PROG_PID=""
	PROG_NAME="./$(cat Makefile | grep "^NAME	" | cut -d'=' -f2 | xargs)"

	while [[ true ]]
	do
		STATE_B=$(get_state)
		if [[ $STATE_A != $STATE_B ]]; then
			STATE_A=$STATE_B
			clear
			info "───────── $(date) ─────────\n"
			sync_sources
			rm -f "$PROG_NAME"
			make
			if [ ! -f "$PROG_NAME" ]; then
				error "COMPILATION ERROR"
			else
				success "COMPILATION OK\n"
				info "───────────────────────────────────────────────────\n"
				$LEAKS_CMD $PROG_NAME $@
			fi
		fi
		sleep 0.1
	done
}

get_state() {
	if [[ $(uname) == "Linux" ]];  then
		MD5="md5sum"
	else
		MD5="md5"
	fi
	SRC_STATE=$(find -L ./ -type f -name "*.cpp" -exec $MD5 {} \;)
	TXT_STATE=$(find -L ./ -type f -name "*.txt" -exec $MD5 {} \;)
	echo "$SRC_STATE" "$TXT_STATE"
}

sync_sources() {
	SOURCES=$(ls *.cpp | tr '\n' ' ')
	SED_COMMAND="s;^SOURCES	.*;SOURCES			=	$SOURCES;"
	if [ $(uname) = "Linux" ];  then
		sed -i -e "$SED_COMMAND" "./Makefile"
	else
		sed -i "" "$SED_COMMAND" "./Makefile"
	fi
}

check_leaks() {
	LEAKS_DETECTED=$(cat ./leaks.log | grep "ERROR SUMMARY" | awk '{printf "%s", $4}' | tr -d "0")
	if [[ $LEAKS_DETECTED == "" ]] ; then
		success "LEAKS\t\tOK"
	else
		error "LEAKS\t\tERROR\t./log/leaks.log"
	fi
}

info() {
	echo -e "\033[36m$1\033[0m"
}
error() {
	echo -e "\033[31m$1\033[0m"
}
success() {
	echo -e "\033[32m$1\033[0m"
}
warning() {
	echo -e "\033[33m$1\033[0m"
}

main() {
	if [[ $1 == "" ]] ; then
		error "PLEASE, PROVIDE EXERCICE NUMBER"
		return 
	fi
	cd "ex0$1"
	if [[ $? != 0 ]] ; then
		error "EXERCISE NOT DEFINED"
		return
	fi
	sync_sources
	make re
	shift
	watch "$@"
}

main $@