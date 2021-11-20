use std::path::Path;
use std::net::TcpListener;
use std::fs::File;
use std::io::prelude::*;
use std::io::BufReader;
use std::env;

fn main () -> std::io::Result<()> {
	let args: Vec<String> = env::args().collect();
	let file_path = Path::new( &args[ 1 ] );			// panics if arg 1 is missing
	let file_to_send = File::open( &file_path ).unwrap();	// panics if file open fails
	let file_size = file_to_send.metadata().unwrap().len();
	let chunk_size: usize = 4096;	// 4 KB is the usual page/sector size for modern disks

	let mut reader = BufReader::with_capacity( chunk_size, file_to_send );



	Ok(())
}
