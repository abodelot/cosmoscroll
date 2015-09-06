#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Run this script to check language files define exactly the same
# string ids than en.lang (the reference language file)

import os

# Return the set of string ids defined in 'filename'
def get_strings(filename):
    strings = set()
    for line in open(filename):
        separator = line.find("=")
        if separator != -1:
            entry = line[:separator].strip()
            strings.add(entry)
    return strings

lang_dir = "resources/lang/"
reference = get_strings(lang_dir + "en.lang")

for f in os.listdir(lang_dir):
    if f != "en.lang":
        print "* Checking " + f
        strings = get_strings(lang_dir + f)
        if strings == reference:
            print "  OK (%d strings)" % len(strings)
        else:
            print "  missing strings: " + ", ".join(reference.difference(strings))
            print "  extra strings: " + ", ".join(strings.difference(reference))
