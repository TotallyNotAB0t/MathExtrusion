#!/bin/bash
cd ./glfw/build/
cmake -DCMAKE_GENERATOR_PLATFORM=x64 ..
cd ../../openal-soft/build
cmake -DCMAKE_GENERATOR_PLATFORM=x64 ..
cd ../../bullet3/build3
cmake -DCMAKE_GENERATOR_PLATFORM=x64 ..