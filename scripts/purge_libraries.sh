#!/bin/bash

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
ROOTPATH=$(dirname "$SCRIPTPATH")

rm -rf $ROOTPATH/lib/*