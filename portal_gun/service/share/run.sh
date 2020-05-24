#!/bin/bash

exec 2> /dev/null
LD_PRELOAD=/home/portal_gun/hook.so timeout 60 /home/portal_gun/portal_gun
