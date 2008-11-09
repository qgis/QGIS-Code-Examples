#!/bin/bash

#
# Simple bash script to generate tutorial documentation
#

txt2tags -t html -o index.html index.t2t
txt2tags -t html -o tutorial1.html 1_hello_world_qgis_style/tutorial1.t2t
txt2tags -t html -o tutorial2.html 2_basic_main_window/tutorial2.t2t
txt2tags -t html -o tutorial3.html 3_basic_labelling/tutorial3.t2

