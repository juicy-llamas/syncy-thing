use std::net::TcpStream;
use std::io::{Read, Write};
use std::string::String;
use std::ascii::escape_default;
use std::str;

fn show(bs: &[u8]) -> String {
    let mut visible = String::new();
    for &b in bs {
        let part: Vec<u8> = escape_default(b).collect();
        visible.push_str(str::from_utf8(&part).unwrap());
    }
    visible
}

fn main () {
	let send: &str = "gay";
	let mut recv = [0 as u8; 128];
	let mut connection = TcpStream::connect( "127.0.0.1:8000" ).unwrap();

	connection.read( &mut recv ).unwrap();
	println!( "{}", show( &recv ) );
	connection.write( send.as_bytes() ).unwrap();
	println!( "{}", send );
}
