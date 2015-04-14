#!/bin/sh
exec tshark -f 'port 15986' -i lo0 -w ${1}
