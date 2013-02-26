#!/usr/bin/env python
#-*- coding: utf-8 -*-

##
# Makefile generator
# usage: ./make_generator.py [makefile_name]
# @author Alexandre Bodelot <alexandre.bodelot@gmail.com>
#

import sys
import os
import time

SRC_DIR = "src"
MAKEFILE = "Makefile"
TARGET = "bin/cosmoscroll"
CFLAGS = "-Wall -Wextra -Wwrite-strings -std=c++11 -pedantic"
LIBS = ("sfml-graphics", "sfml-window", "sfml-system", "sfml-audio", "sfml-network")


class MakeGenerator:

	def __init__(self, target, makefile_name):
		self.sources = []
		self.target = target
		self.makefile_name = makefile_name
		self.libs = ()
		self.cflags = ""
		self.searchpath = []

	def set_cflags(self, cflags):
		self.cflags = cflags


	##
	# Indiquer les bibliothèques à utiliser lors de l'édition des liens
	#
	def set_libs(self, libs):
		self.libs = libs


	##
	# Add a source directory in the search path
	#
	def add_src_dir(self, directory):
		self.searchpath.append(directory)
		self.add_sources(directory)


	##
	# Récupérer toutes les unités de compilation dans parent et ses sous-dossiers
	#
	def add_sources(self, parent):
		for item in os.listdir(parent):
			path = os.path.join(parent, item)
			if os.path.isfile(path) and path.endswith(".cpp"):
				self.sources.append(os.path.splitext(path)[0])
			elif os.path.isdir(path):
				self.add_sources(path)

	##
	# Générer le fichier makefile
	#
	def write_makefile(self):
		f = open(self.makefile_name, "w")
		f.write(
		"# Makefile generated on " + time.ctime() + "\n\n"
		"CC=g++\n"
		"CFLAGS= " + self.cflags + "".join(" -I" + d for d in self.searchpath) + "\n")

		# libs
		libs_linux = " ".join("-l" + lib for lib in self.libs)
		libs_mac = " ".join("-framework " + lib for lib in self.libs)

		f.write(
		"MAC := $(shell uname)\n"
		'ifeq (MAC, "Darwin")\n'
		"\tLDFLAGS= " + libs_mac + " -ldumb\n"
		"else\n"
		"\tLDFLAGS= " + libs_linux + " -ldumb\n"
		"endif\n"
		"EXEC=" + TARGET + "\n"
		"\n"
		"# debug/release mode\n"
		"DEBUG=yes\n"
		"ifeq ($(DEBUG), yes)\n"
		"	CFLAGS += -g -DDEBUG\n"
		"else\n"
		"	CFLAGS += -O2 -march=native -fomit-frame-pointer\n"
		"endif\n"
		"\n"
		"$(EXEC): " + " ".join(s + ".o" for s in self.sources) + "\n"
		"	@echo linking $(EXEC)...\n"
		"	@$(CC) $^ -o $(EXEC) $(LDFLAGS)\n\n")

		# .o
		for source in self.sources:
			f.write(
			source + ".o:" + source + ".cpp\n"
			"	@echo '*' $<\n"
			"	@$(CC) -o $@ $< -c $(CFLAGS)\n\n")

		# clean
		f.write(
		"clean:\n")
		for source in self.sources:
			f.write(
			"	@rm " + source + ".o\n")

		f.write(
		"\n"
		"mrproper: clean\n"
		"	@rm $(EXEC)\n"
		"\n"
		"all: mrproper $(EXEC)\n")

		f.close();


if __name__ == "__main__":
	makefile_name = sys.argv[1] if len(sys.argv) > 1 else MAKEFILE

	m = MakeGenerator(TARGET, makefile_name)
	m.add_src_dir(SRC_DIR)
	m.set_libs(LIBS)
	m.set_cflags(CFLAGS)
	m.write_makefile()
	print '"%s" generated. Run make to compile.' % makefile_name
