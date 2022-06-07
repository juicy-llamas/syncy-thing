use std::net::TcpListener;
use std::net::TcpStream;
use std::fs::File;
use std::io;
// use std::io::BufReader;
// use std::io::BufWriter;
// use std::io::SeekFrom;
use std::io::prelude::*;

fn main () -> io::Result<()> {
	let listener = TcpListener::bind("127.0.0.1:8000").unwrap();

	match listener.accept() {
		Ok( ( mut socket, addr ) ) => {
			println!( "new client: {}", addr );
			manager()
		},
		Err( reason ) => println!( "client failed to connect: {}", reason )
	}

	Ok(())
}

fn manager ( socket: mut TcpStream ) -> () {
	const BLOCK_SIZE: usize = 4096;
	let buffer: &mut [u8] = &mut [ 0; BLOCK_SIZE ];
	let mut read_bytes: io::Result<usize> = socket.read( buffer );

	while read_bytes.is_ok() and read_bytes.unwrap() > 0 {
		/*
			0x0 == error, something went wrong? (what to do in this situation)
			0x1 == sharing file: host is giving us access to a file with the name encoded as a null terminated array, it's identifier (hash of name?), the size as 64 bits after the name, and a list of peers
				(addresses, other than client/server) to send this file to / receive this file from in case it's updated. Each peer also has individual access rights as well, which will be included as a
				byte (or two possibly) after the address of that peer. After that,
			0x2 == request access: once we have a file shared with us, or we just want a certain file, we can request it from a peer. If we get a file shared with us, we should send this command to every
				peer in the list of peers included in 0x1. We share the identifier of the file, and optionally, the peer who gave us access
			0x3 == sending file: file was updated, so host is sending us the updated file. We start with the identifer of the file, and then proceed to read bytes from packets until our file size is up.
				At the end of the file, there will be a checksum, and we will check to make sure the checksum checks out (we're using TCP so there should be no issues here, but we do it regardless).
			0x4 == received file: This is just a message to let the sender know we have received a file. This would be sent to the sender and include the name of the message and the checksum.

			If I think of anything else I should add it here!
		*/
		match buffer[ 0 ] {
			1 => {

			},
			2 => {

			},
			3 => {

			},
			4 => {

			},
			_ => println!( "error in reading" );
		}
		socket.read( buffer );
	}

	socket.shutdown();
}

// 		println!( "Sending file to client..." );
// 		let mut result: std::io::Result<usize> = Ok(1);
// 		let mut bytes_sent = 0;
// 		while result.is_ok() && result.unwrap() != 0 {
// 			let request = reader.fill_buf();
// 			result = match request {
// 				Ok( buf ) => socket.write( buf ),
// 				Err( reason ) => { println!( "request had an error: {}", reason ); Err( reason ) }
// 			};
//
// 			reader.consume( chunk_size );
// 			bytes_sent += result.as_ref().unwrap();
// 		}
// 		reader.seek( SeekFrom::Start( 0 ) );
// 		println!( "Sent {} bytes", bytes_sent );

