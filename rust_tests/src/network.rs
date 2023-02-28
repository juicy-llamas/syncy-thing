
// pub struct Thing {
// 	stuff: usize,
// }

// pub fn do_stuff () {
// 	println!( "hello" );
// 	im_not_pub();
// }

// fn im_not_pub () {
// 	println!( "im not pub" );
// }

// #[macro_export]
// macro_rules! im_a_macro {
// 	( $a:ident, $b:expr, $c:expr ) => { println!( "I'm a macro! I say {}, {}, and {}.", $a, $b, $c ) };
// 	( $a:expr, $b:expr, $c:expr ) => { println!( "I'm the second match! I say {}, {}, and {}.", $a, $b, $c ) };
// }

// impl Thing {
// 	pub fn new ( stuff: usize ) -> Self {
// 		Thing { stuff }
// 	}

// 	pub fn do_the_thing ( &self ) {
// 		println!( "The thing did has been the done. Stuff: {}", self.stuff );
// 	}

// 	#[cfg(test)]
// 	pub fn get_the_stuff ( &self ) -> usize {
// 		println!( "Getten the stuff has did the been." );
// 		self.stuff
// 	}
// }

// #[cfg(test)]
// mod test {
// 	use crate::network::Thing;
// 	#[test]
// 	fn test_the_testing_functionality () {
// 		assert_eq!( 4, 2 + 2 );
// 	}

// 	#[test]
// 	fn test_the_testing_functionality_with_the_stuff () {
// 		let thing: Thing = Thing::new( 42 );
// 		assert_eq!( thing.get_the_stuff(), 42 );
// 	}

// 	#[test]
// 	fn test_the_macro () {
// 		im_a_macro!( "butter", "cream", "milk" );
// 	}
// }

use std::io::{ Read, Write, Error, ErrorKind };
use std::net::{ TcpListener, TcpStream, IpAddr, Ipv4Addr, Ipv6Addr, SocketAddr };
use std::thread;
use std::thread::JoinHandle;
use std::path::Path;
use std::fs;

macro_rules! ip_ad {
	( $a:expr, $b:expr, $c:expr, $d:expr, $p:expr ) => {
		SocketAddr::new( IpAddr::V4( Ipv4Addr::new( $a, $b, $c, $d ) ), $p )
	};
	( $st:expr ) => {{
		let res = $st.parse::<SocketAddr>();
		match res {
			Ok( a: SocketAddr ) => a,
			// perhaps change in the future
			Err( e ) => panic!( "Error parsing IP: {}", e )
		}
	}};
	( $a:expr, $b:expr, $c:expr, $d:expr, $e:expr, $f:expr, $g:expr, $h:expr, $p:expr ) => {
		SocketAddr::new( IpAddr::V6( Ipv6Addr::new( $a, $b, $c, $d, $e, $f, $g, $h ) ), $p )
	};
}

fn send_file( mut conn: TcpStream, name: &Path ) -> std::io::Result<()> {
	// this is not efficient at all but whatever
	let buff = fs::read( name );
	let name: String = match name.to_str() {
		Some( p ) => String::from( p ) + "\0",
		None => {
			let msg = "Error: the path could not be converted to UTF-8";
			return Err( Error::new( ErrorKind::NotFound, msg ) );
		}
	};
	match buff {
		Ok( vect ) => {
			conn.write( name.as_bytes() );
			conn.write( vect.as_slice() );
			Ok( () )
		},
		Err( e ) => Err( e )
	}
}

fn recv_file( mut conn: TcpStream ) -> std::io::Result<String> {
	let mut buff: [u8; 4096] = [0; 4096];
	let mut res: std::io::Result<usize> = conn.read( &mut buff );
	let mut name: String = String::new();
	let mut reading_name: bool = true;
	let mut file: Vec<u8> = Vec::with_capacity( 4096 );
	// Read the buffer
	while res.is_ok() {
		if reading_name {
			// transform into an iterator
			let mut iter = buff.clone().into_iter();
			let mut i = iter.next(); 
			while i.is_some() {
				// if we get a 0, we break and have finished reading the name, otherwise we just push the char
				let item = i.unwrap();
				if item == 0 {
					reading_name = false;
					break;
				} else {
					name.push( item as char );
				}
				i = iter.next();
			}
			// move the rest of the iterator into the file
			file.append( &mut iter.collect::<Vec<u8>>() );
		} else {
			// normal operation: append the buffer to the file
			file.extend_from_slice( &buff );
		}
		res = conn.read( &mut buff );
	}
	let fpath = Path::new( name.clone() );
	// Save the file
	fs::write( fpath, file.as_slice() );
	Ok( name )
}

fn handle_conn( mut conn: TcpStream, addr: SocketAddr ) {
	println!( "Called loop handle." );
	let mut buff: [u8; 1] = [0; 1];
	loop {
		match conn.read( buff ) {
			Ok( 1 ) => {
				recv_file( conn );
			},
			Err( e ) => {
				eprintln!( "Error: {}", e );
			}
		}
	}
}

pub fn start_server() -> std::io::Result<()> {
	println!( "Starting server..." );
	let listener = TcpListener::bind( ip_ad!( 127, 0, 0, 1, 8000 ) );
	match listener {
		Ok( listener ) => {
			println!( "Server started." );
			loop {
				let ( conn, addr ) = {
					match listener.accept() {
						Ok( ( s, c ) ) => { ( s, c ) },
						Err( e ) => { eprintln!( "Error: {}", e ); continue; }
					}
				};
				println!( "Log: connection from address {}", addr );
				thread::spawn( move || -> () { handle_conn( conn, addr ) } );
			}
		},
		Err( e ) => Err( e )
	}
}

pub fn start_client() -> std::io::Result<JoinHandle<()>> {
	println!( "Starting client..." );
	let addr = ip_ad!( 127, 0, 0, 1, 8000 );
	let res = TcpStream::connect( addr );
	match res {
		Ok( conn ) => Ok( thread::spawn( move || -> () { handle_conn( conn, addr ) } ) ),
		Err( e ) => Err( e )
	}
}