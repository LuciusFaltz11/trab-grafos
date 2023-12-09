#!/bin/bash

rm -f execGrupo18


g++ *.cpp -o execGrupo18 -g

while IFS=$'\n' read -r line
do
    args=($line)

    mkdir "./out/${args[5]}"
    echo "Running test case: ${args[@]}"
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        ./execGrupo18 "${args[@]}"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        ./execGrupo18 "${args[@]}"
    elif [[ "$OSTYPE" == "cygwin" || "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
        ./execGrupo18.exe "${args[@]}"
    else
        echo "Unsupported operating system"
    fi
done < <(tr -d '\r' < args.txt)