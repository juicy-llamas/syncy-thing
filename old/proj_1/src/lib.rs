use std::env;

pub struct Arguments {
	pub config_file: String
}

#[allow(unused_variables)]
pub fn do_args () -> Arguments {
	let args: Vec<String> = env::args().collect();
	let mut i = 1;
	// Specifies default args
	let mut ret = Arguments{
		config_file: String::from( "./app_config.conf" )
	};

	while i < args.len() {
		match args[ i ].as_str() {
			"-c" => {
				i += 1;
				ret.config_file = args[ i ].clone();
			}
			default => { panic!( "invalid arguments" ); }
		}
		i += 1;
	}

	ret
}

pub fn get_config ( args: Arguments ) -> Result<Config, &str> {
	let conf_file = match OpenOptions::new().read( true ).open( args.config_file ) {
		Ok( fp ) => fp,
		Err( aAaA ) => return gen_config()	// generates the config file and returns default args
	};
	let conf_reader = BufReader::new( conf_file ).bytes();

	let mut host_list: Vec<Host> = Vec::new();
	while reader {

	}
}

fn is_whitesp ( char c ) -> bool {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

fn absorb_string ( reader: BufReader, delm: char ) -> Result<&str, &str> {
	let mut s: String = String::new();
	let mut c: Option<char> = reader.next();
	if delm == 0 {
		while c != None && !is_whitesp( c.unwrap() ) {
			s.push( c );
			c = reader.next();
		}
		if c.is_some() {
			return Ok( s.as_str() )
		} else {
			return Err( "File ended before we could read string" );
		}
	} else {
		if c != Some( delm ) {
			return Err( "Expected a delimiter at the start of a string." );
		}
		c = reader.next();
		while c != None && c.unwrap() != delim {
			s.push( c );
			c = reader.next();
		}
		if c == Some( delim ) {
			return Ok( s.as_str() )
		} else {
			return Err( "File ended before we could read string" )
		}
	}
}

fn gen_config () -> Result<Config, &str> {
	let mut conf_file = match OpenOptions::new().create( true ).write( true ).open( args.config_file ) {
		Ok( fp ) => fp,
		Err( aAaA ) => Err( aAaA )
	};
	static default_config = Config {

	};
}

pub struct SharedFile {
	pub name: String,
	pub perms: i32,
	// fp?
}

pub struct Host {
	pub name: String,
	pub files: Vec<SharedFile>
	// extra stuff? like methods, callbacks, etc
}

pub struct Config {
	pub hosts: Vec<Hosts>
	// anything else?
}
