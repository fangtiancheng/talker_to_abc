#!/bin/bash

project_root="$(dirname ${BASH_SOURCE[0]})/.."
build_dir="${project_root}/build"

mkdir -p ${build_dir}
cd ${build_dir}
cmake ${project_root} -DCMAKE_BUILD_TYPE=Release && make -j
