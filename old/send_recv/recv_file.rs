use std::path::Path;
use std::net::TcpStream;
use std::fs::File;
use std::io::prelude::*;
use std::io::BufReader;
use std::env;

fn main () -> std::io::Result<()> {
	let mut connection = TcpStream::connect( "127.0.0.1:8000" ).unwrap();
	let mut recv_buf = [0 as u8; 4096];

	connection.read( &mut recv_buf );
	let mut f_name = String::new();
	for ch in &recv_buf {
		if *ch == 0 {
			break;
		}
		f_name.push( *ch as char );
	}
	let mut i = f_name.len()+1;
	let mut f_size: u64 = 0;
	while i < f_name.len() + 9 {
		f_size += ( recv_buf[ i ] as u64 ) << 8*(i-f_name.len()-1);
		i += 1;
	}

	let f_path = Path::new( f_name.as_str() );
	let mut file = File::create( f_path ).unwrap();

	println!( "Receving file {} with size {}.", f_name, f_size );

	let mut j = f_size;
	while ( match connection.read( &mut recv_buf ) { Ok(_) => {true} Err(_) => {false} } ) && ( j > 0 ) {
		file.write( &recv_buf );
		j = if j as i64 - 4096 < 0 { 0 } else { j - 4096 };
	}

	Ok(())
}
