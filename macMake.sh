#!/bin/sh

# macMake.sh
# Part of the lhendraw project.
# This replaces the Makefile on xcode
# Using the actual Makefile on a mac was not tested long ago, you'd need the commandline tools.
# which were not tested for long on this project
# Created by Lennard Wasserthal on 29.12.15.
# Copyright 2015 Dr. Lennard Wasserthal. License: AGPLv3+

mkdir ./generated 
rm ./generated/initialization_lhendraw.hxx
rm ./generated/propertylist_lhendraw.hxx 
rm ./generated/propertydirectory_lhendraw.hxx
rm ./generated/multilistlistfile.hxx
rm ./generated/reflection.hxx
./build/Debug/tools/filestructure -l0 internalstructure.draft ./generated/structure.hxx ./generated/initialization_lhendraw.hxx '' ./generated/propertylist_lhendraw.hxx ./generated/propertydirectory_lhendraw.hxx
./build/Debug/tools/filestructure -m1 filestructure.draft ./generated/cambridgestructure.hxx ./generated/initialization_lhendraw.hxx CAMBRIDGE
./build/Debug/tools/filestructure -m1 bkchem.draft ./generated/bkchemstructure.hxx ./generated/initialization_lhendraw.hxx BKCHEM
./build/Debug/tools/filestructure -m2 configfile.draft ./generated/configfilestructure.hxx ./generated/initialization_lhendraw.hxx CONFIGBRIDGE
./build/Debug/tools/reflection_enums
./build/Debug/tools/reflection edit.hxx sdlctrl.hxx filedlg.hxx pivot.hxx

rm ./generated/pullout_stringfile.hxx
rm ./generated/pullout_structfile.hxx
rm ./generated/pullout_reflectfile.hxx
./build/Debug/tools/pullout
./build/Debug/tools/option ./edit.hxx ./parsecdxml_variables.hxx


