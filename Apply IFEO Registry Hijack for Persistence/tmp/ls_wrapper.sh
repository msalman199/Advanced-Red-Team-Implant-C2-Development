#!/bin/bash
echo "$(date): ls command hijacked" >> /tmp/system_hijack_log.txt
/bin/ls_original "$@"
