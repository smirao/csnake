#!/bin/bash

SCRIPT=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$SCRIPT")
ROOTPATH=$(dirname "$SCRIPTPATH")

cd $ROOTPATH/lib/
git clone https://github.com/ThrowTheSwitch/Unity.git