#!/usr/local/bin/bash
midi2abc -gk -gu -i test.mid -o test.abc
abc2midi test.abc -o testabc.mid
