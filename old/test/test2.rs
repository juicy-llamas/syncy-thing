use std::io::{BufReader, Read};
use std::fs::{OpenOptions, File};

fn main () {
	let f_p = File::open( "test text" ).unwrap();
	let mut f_iter = BufReader::new( f_p )
						   .bytes()
						   .filter( |x| x.is_ok() )
						   .map( |x| x.unwrap() as char )
						   .peekable();
	let mut i = 0;
	while f_iter.peek().is_some() {
		match str_quote( &mut f_iter, '"' ) {
			Ok( k ) => { i += 1; println!( "{}: {}", i, k ) },
			Err( k ) => { i += 1; println!( "{}: {}", i, k ) }
		}
	}
}

fn str_unquote<'a, I: Iterator<Item = char>> ( iter: &mut I ) -> String {
	let is_not_whitesp = |&x: &char| !( x == ' ' || x == '\t' || x == '\n' || x == '\r' );
	let result: String = iter.take_while( is_not_whitesp ).collect::<String>();
	result
}

fn str_quote<'a, I: Iterator<Item = char>> ( iter: &mut I, quote: char ) -> Result<String, &str> {
	let c = iter.next();
	if c != Some( quote ) {
		return Err( "a quoted string was expected" );
	}
	let not_quote = |&x: &char| x != quote;
	let result: String = iter.take_while( not_quote ).collect::<String>();
	Ok( result )
}
