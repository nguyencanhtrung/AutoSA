#!/bin/sh
git submodule init
git submodule update
(cd ppcg_src/isl; git submodule init imath; git submodule update imath)
