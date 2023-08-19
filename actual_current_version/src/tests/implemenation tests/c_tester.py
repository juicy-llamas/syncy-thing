#!/usr/bin/python3

# this is what my other project should have been .__.

# this script uses a config dir, which is set by default to ~/.config/c_tester
# obviously, you can change this to anything you wish by setting the
# C_TEST_CONFDIR env var, or just changing the variable you see below
# On windows, you HAVE to change this. Rest of the code is cross platform.

import os
import sys
import argparse

# directories
config_dir = os.environ.get( 'C_TEST_CONFDIR', '~/.config/c_tester' )
template_dir = os.path.join( config_dir, 'templates' )
includes_dir = os.path.join( config_dir, 'includes' )

# default config
default_config = """
# This is supposed to be a config file!

"""
# default templates
default_templates = {
	'default.c': '\n/**\n * Default Template\n * By Daniel Moylan\n */\n\n#include <stdio.h>\n#include <string.h>\n#include <stdlib.h>\n#include <unistd.h>\n\nint main ( int argc, char** argv ) {\n\tputs( "We be doin stuff" );\n\n\treturn 0;\n}',
	'benchmark.c': '// TODO'
}

# template helpers: headers and source files for convenience fns to use while testing.
# If you include them, they all come as one package
template_helpers = {
	# nothing for now
}

def init ():
	# setting up
	os.makedirs( config_dir, exist_ok = True )
	os.makedirs( template_dir, exist_ok = True )
	os.makedirs( includes_dir, exist_ok = True )
	config_file = open( os.path.join( config_dir, 'config.conf' ), 'w+' )
	# also save templates if applicable
	if len( os.listdir( template_dir ) ) == 0:
		for nm, txt in default_templates:
			with open( os.path.join( template_dir, nm ), 'w' ):
				fp.write( txt )

	# parse (or write) the config file
	st = config_file.tell()
	end = config_file.seek( -1 )
	if end == st:
		config_file.write( default_config )
		config_file.close()
	else:
		# read config (not there yet)

	# stuff that I'm supposed to set with config
	config.editor = 'nano'
	config.truncate = True
	config.preserve_code = False
	config.preserve_exc = False
	config.template_name = 'default.c'
	config.gcc_args = f'-Wall -I{includes_dir} -Wextra -g -o ./output ./{template_name}'

	# sanity: make sure we're not working in the config directory
	if os.getcwd().find( config_dir ) != -1:
		print( "Must run program outside of the config directory" )
		sys.exit( 1 )

	return config

def main ( win ):
	config = init()

	# first step: copy the template to the cur dir & open the copy with the editor
	template_path = os.path.join( template_dir, config.template_name )
	with open( template_path, 'r' ) as srcf:
		try:
			with open( config.template_name, 'x' ) as dstf:
				dstf.write( srcf.read() )
		except FileExistsError:
			# if it exists, pose a set of options
			print( f"There is already a file by the name of {config.template_name} in the current directory.\nYou have three options:\n\t- Truncate and overwrite the file (press d)\n\t- Save this file to a different name (press n)\n\t- Quit (press any other key)\n> ", end = '' )
			key = win.getkey()
			if key == 'd':
				print( "\nAre you sure? (press y or quit)\n> ", end = '' )
				if key == 'y':
					print( "\nTruncating..." )
					with open( config.template_name, 'w' ) as dstf:
						dstf.write( srcf.read() )
				else:
					return
			elif key == 'n':

			else:
				return

	while True:
		if os.system( f'{editor} {template_name}' ) != 0:
			print( "Warning: text editor exited with error" )

		# next, we compile and link the program
		if os.system( f'gcc {gcc_args} {template_name}' ) != 0:
			print( "Error: compilation error" )
			sys.exit( 1 )

		# finally, we run the program and see if it failed or succeeded
		if os.system( f'./{template_name}' ) != 0:
			print( "Program exited in error." )
			sys.exit( 1 )
		else:
			print( "Program ran successfully!" )

		print( "Press q to quit, s to save " )
		key = win.getch()
		if key == 'q':

		elif key == 's':

		else:

	if preserve_code == False:
		os.rm( template_name )
	if preserve_exc == False:
		os.rm(

curses.wrapper( main )
