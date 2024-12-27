#!/bin/bash

project_root="$(dirname ${BASH_SOURCE[0]})/.."
build_dir="${project_root}/build"
exe_path="${build_dir}/talker_to_abc"

if [ ! -f "$exe_path" ] || [ ! -x "$exe_path" ]; then
    echo "please run build.sh first"
    exit 1
fi

npy_path="${project_root}/resources/cet4.npy"
flame_path="${project_root}/resources/FLAME_sample.npy"

$exe_path $npy_path $flame_path
