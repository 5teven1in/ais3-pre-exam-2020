#!/bin/bash

exec 2> /dev/null
timeout 60 LD_PRELOAD=/home/portal_gun/hook.so /home/portal_gun/portal_gun
