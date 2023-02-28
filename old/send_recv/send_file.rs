use std::net::TcpListener;
use std::fs::File;
use std::io::prelude::*;
use std::io::BufReader;
use std::io::SeekFrom;
use std::env;

fn main () -> std::io::Result<()> {
	let args: Vec<String> = env::args().collect();

	let file_to_send = File::open( &args[ 1 ] ).unwrap();	// panics if file open fails
	let file_size = file_to_send.metadata().unwrap().len();
	let chunk_size: usize = 4096;
	let mut reader = BufReader::with_capacity( chunk_size, file_to_send );

	let listener = TcpListener::bind("127.0.0.1:8000").unwrap();

	for client in listener.incoming() {
		match client {
			Ok( mut socket ) => {
				println!( "Sending file to client..." );
				let mut result: std::io::Result<usize> = Ok(1);
				let mut bytes_sent = 0;
				while result.is_ok() && result.unwrap() != 0 {
					let request = reader.fill_buf();
					result = match request {
						Ok( buf ) => socket.write( buf ),
						Err( reason ) => { println!( "request had an error: {}", reason ); Err( reason ) }
					};

					reader.consume( chunk_size );
					bytes_sent += result.as_ref().unwrap();
				}
				reader.seek( SeekFrom::Start( 0 ) );
				println!( "Sent {} bytes", bytes_sent );
			},
			Err( reason ) => println!( "client failed to connect: {}", reason )
		}
	}

	Ok(())
}
