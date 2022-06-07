use std::io::{BufReader, Read};
use std::fs::{OpenOptions, File};
use std::path::Path;

fn main () {
	let path = Path::new( "test text" );
	let conf_file = match OpenOptions::new().read( true ).open( path ) {
		Ok( fp ) => fp,
		Err( aAaA ) => panic!( "nO" )	// generates the config file and returns default args
	};
	let conf_reader = BufReader::new( conf_file );
	let conf_iter = conf_reader.bytes();

// 	let mut host_list: Vec<Host> = Vec::new();
	let mut stwing = absorb_string( conf_iter, None );
	while stwing.is_ok() {
		println!( "STWING: {}", stwing.unwrap() );
		stwing = absorb_string( conf_iter, None );
	}
}

fn is_whitesp ( c: char ) -> bool {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

fn absorb_string<'a> ( reader: std::io::Bytes<BufReader<File>>, delm: Option<char> ) -> Result<&'a str, &'a str> {
	let mut s: String = String::new();
	let mut c: Option<Result<u8>> = reader.next();
	match delm {
		None => {
			while c != None && c.unwrap().is_ok() && !is_whitesp( c.unwrap().unwrap() ) {
				s.push( c.unwrap().unwrap() );
				c = reader.next();
			}
			if c.is_some() {
				return Ok( s.as_str() )
			} else {
				return Err( "File ended before we could read string" );
			}
		}
		d => {
			if c != d {
				return Err( "Expected a delimiter at the start of a string." );
			}
			c = reader.next();
			while c != None && c != d {
				s.push( c.unwrap() );
				c = reader.next();
			}
			if c == d {
				return Ok( s.as_str() )
			} else {
				return Err( "File ended before we could read string" )
			}
		}
	}
}

