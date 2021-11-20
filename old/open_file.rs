// hewp me mr obama

use std::path::Path;
use std::fs::File;
use std::io::prelude::*;
use std::io::ErrorKind::*;
use std::io::BufReader;

fn main () -> std::io::Result<()> {
	static CONFIG_PATH: &str = "./hewwo-mr-obawma.txt";
	static CONFIG_DEFAULT: &str = "# This is a config fileeee\n";
	let path = Path::new( CONFIG_PATH );

	let mut file = match File::open( &path ) {
		Err( e ) if e.kind() == NotFound => {
			let path_str = path.display();
			println!( "No config file found! Creating a new one in {}...", path_str );
			let mut fil = match File::create( &path ) {
				Ok( f ) => f,
				Err( e ) if e.kind() == NotFound => panic!( "Directory not found :(" ),
				Err( e ) => panic!( "Cannot create file: {}", e ),
			};
			match fil.write( CONFIG_DEFAULT.as_bytes() ) {
				Ok( _ )  => fil,
				Err( e ) => panic!( "Error writing config file: {}", e ),
			}
		},
		Ok( file ) => file,
		Err( e ) => panic!( "Error reading config file: {}", e ),
	};

	let mut s = String::new();
	let

	while
	match file.read_to_string( &mut s ) {
		Err( why ) => panic!( "couldn't read {}: {}", CONFIG_PATH, why ),
		Ok( _ ) => {
			print!( "{} contains:\n{}", CONFIG_PATH, s );
			Ok(())
		}
	}
}
