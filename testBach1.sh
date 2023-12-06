#!/bin/bash

# Compile all .cpp files
g++ *.cpp -o execGrupo18

# Read the file line by line
while IFS=$'\n' read -r line
do
    # Split the line into an array of arguments
    args=($line)

    mkdir "./out/${args[5]}"
    mkdir "./out/${args[5]}/images"
    echo "Running test case: ${args[@]}"
    # Detect the output file type based on the user's operating system
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        # Linux
        ./execGrupo18 "${args[@]}"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        # macOS
        ./execGrupo18 "${args[@]}"
    elif [[ "$OSTYPE" == "cygwin" || "$OSTYPE" == "msys" || "$OSTYPE" == "win32" ]]; then
        # Windows
        ./execGrupo18.exe "${args[@]}"
    else
        echo "Unsupported operating system"
    fi
    # python gerar-graphviz-image.py "./out/${args[5]}/graphviz.txt" "${args[5]}"
done < <(tr -d '\r' < args.txt)