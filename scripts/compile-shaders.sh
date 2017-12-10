#!/bin/sh
glslangValidator -V shaders/shader.vert -o spv/vert.spv
glslangValidator -V shaders/shader.frag -o spv/frag.spv
