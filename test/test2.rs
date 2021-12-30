use std::io::{BufReader, Read};
use std::fs::{OpenOptions, File};

fn main () {
	let f_p = File::open( "test text" ).unwrap();
	let mut f_iter = BufReader::new( f_p )
						   .bytes()
						   .filter( |x| x.is_ok() )
						   .map( |x| x.unwrap() as char );
	let k = get_string_whitesp( &mut f_iter ).unwrap();
	println!( "{}", k );
}

fn get_string_whitesp<'a, I: Iterator<Item = char>> ( iter: &mut I ) -> Result<&'a str, &'a str> {
	let is_not_whitesp = |&x| !( x == ' ' || x == '\t' || x == '\n' || x == '\r' );
	let result: &str = iter.take_while( is_not_whitesp ).collect::<String>();
	if result.len() > 0 {
		Ok( result )
	} else {
		Err( "could not get a valid string" )
	}
}
