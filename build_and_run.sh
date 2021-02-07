#! /usr/bin/env bash
parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )


[ -d "$parent_path/build/" ] || cmake -DCMAKE_BUILD_TYPE=Debug -S $parent_path/src/ -B $parent_path/build/

cmake --build $parent_path/build/ && $parent_path/build/dork
