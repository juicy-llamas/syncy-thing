
use std::fs;
use std::fs::File;
use std::path::Path;
use std::io::BufReader;
use std::io::{TcpStream, TcpListener};


struct SharedFile {
	path: Path,
	auth: bool,
	write: bool,
	fp: File
}

impl Drop for SharedFile {
	fn drop ( &mut self ) {
		fp.close();
	}
}

impl SharedFile {
	fn open ( path: Path, auth: bool, write: bool ) -> SharedFile {
		SharedFile {
			path: path,
			auth: auth,
			write: write,
			fp: {
				if write {
					fs::OpenOptions::new().read( true ).write( true ).create( true ).open( path ).expect( "aaaaaaaAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" )
				} else {
					fs::OpenOptions::new().write( true ).create( true ).open( path ).expect( "aaaaaaaAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" )
				}
			}
		}
	}
}

struct Host {
	name: &str,
	files: Vec<SharedFile>,
	stream: TcpStream
}

impl Host {

}

fn handle_Host ( ) {

}

fn main () -> isize {
// 	let args: Vec<String> = std::env::args().collect();
// Parse some args and generate relevant data
	let mut i = 1;
// 	let alt_config: &str;
	while i < args.len() {
		match args[ i ] {
			"-c" => {
				i += 1;
				alt_config = args[ i ];
			}
			default => { panic!( "invalid arguments" ); }
		}
		i += 1;
	}
//
// 	static default_config = "./app_config.conf";
// 	let config_path: Path;
// 	if ( alt_config.len() > 0 ) {
// 		config_path = Path::new( alt_config );
// 	} else {
// 		config_path = Path::new( default_config );
// 	}
//
// Load/Parse the config file
// 	let config_file = fs::read_as_string( config_path );
// 	while i < config_file.len() {
// 		let c = config_file[ i ];
// 		if c == ' ' || c == '\t' || c == '\n' {
// 			continue;
// 		}
// 		match c {
// 			'{' => {
//
// 			}
// 			'}' => {
//
// 			}
// 			'\'' => {
//
// 			}
// 			',' => {
//
// 			}
// 			default => {
//
// 			}
// 		}
// 	}


// Try to connect to one Host after the other.
// When one does connect, ping it for a list of online Hosts in config (every process should have one, and we will too).
// Init connections with all of the other Hosts.
// W a i t for any changes in specified files as well as incoming requests (how are we going to do this?)
	0
}
