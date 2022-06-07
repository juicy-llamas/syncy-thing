use std::net::TcpStream;
use std::fs::File;
use std::io::prelude::*;

fn main () -> std::io::Result<()> {
	let mut connection = TcpStream::connect( "127.0.0.1:8000" ).unwrap();
	let mut recv_buf = [0 as u8; 512];

	let mut file = File::create( "temp" ).unwrap();

	println!( "Receving file..." );

	let mut size = 0;
	let mut code = connection.read( &mut recv_buf );
	while code.is_ok() {
		let v = code.unwrap();
		if v == 0 {
			break;
		}

		size += v;
		file.write( &recv_buf[ 0..v ] );
		code = connection.read( &mut recv_buf );
	}

	println!( "Received {} bytes", size );
	Ok(())
}
