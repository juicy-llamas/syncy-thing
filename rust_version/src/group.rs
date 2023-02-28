
/**
 * Groups
 * 
 * These are the foundational building blocks for the management of the file system.
 * They will include a list of folders and files, called nodes.
 * Groups also have default policies for sharing with other peers.
 * Peers will have a list of Membership structures, which will include 
 * The purpose of this application is to monitor these folders and files for changes, and to update those
 */

use std::time::SystemTime;
use std::path;
use rand::random;
use bitbag::{ BitBag, BitBaggable };
use notify::{ Watcher, RecommendedWatcher, RecursiveMode, Event, EventHandler };
use notify::event::{ EventKind, AccessKind, CreateKind, ModifyKind, RemoveKind };

pub mod group {

	/**
	 * Group Permissions
	 * 
	 * These are permissions that individual peers have with respect to a group.
	 * There is one permission, view permission, that is not included, as it is implicit that the peer can view the group if it has a
	 * GroupPermissions structure. Thus, peers that want to send 
	 */
	#[derive(BitBaggable, EnumIter, Clone, Copy)]
	#[repr(u8)]
	pub enum Permissions {
		Write = 0x01,
		Read = 0x02,
		Administrator = 0x04,
		AddAdministrator = 0x08,
		ViewMembers = 0x10,
		// view the *permissions* of the members, not view
		ViewPermissions = 0x20,
		// special value for creation
		Empty = 0x80
		// group view permission is implicit in the creation of this structure
	}

	/**
	 * Group Policies
	 * 
	 * If the flag bit is 0, the group does NOT allow the policy.
	 * All 0s means the group is effectively closed off permanently except to people already in the group.
	 * All 1s means the group is not only open to any peer that joins the network, the permissions structure can also be modified and
	 * the group can even be deleted.
	 */
	#[derive(BitBaggable, EnumIter, Clone, Copy)]
	#[repr(u16)]
	pub enum Policy {
		// This allows every peer to write to the group's nodes.
		OpenWrite = 0x01,
		// This allows every peer to read and be updated on the group's nodes by writers.
		OpenRead = 0x02,
		// This allows every peer to request write permission. Without request permission, the administrators have to explicitly
		// add peers to the group.
		RequestWrite = 0x04,
		// This allows peers to request read permission.
		RequestRead = 0x08,
		// This allows every peer to view the members in the group. If not set, only group members (readers, writers, viewers, or
		// administrators)
		ShareMembers = 0x10,
		SharePerms = 0x20,
		Visible = 0x40,
		// Administrators can add (or remove) readers and writers, and even administrators if they have 'AddAdministrator'
		AllowAdministrators = 0x80,
		// Members can request admin rather than explicitly being added
		RequestAdmin = 0x100,
		// Group can be deleted by Admins, otherwise it will persist until there are no nodes left to monitor, or alternately forever.
		AllowDeletion = 0x200,
		// Members with write permission can watch and unwatch paths without explicitly deleting or adding them
		// This may lead to a loss of syncronization of the file directories
		AllowNodeAccess = 0x400,
		// Delete the group if it has no nodes.
		DeleteOnEmpty = 0x800,
		// Sync the Access Times
		SyncAccessTime = 0x1000,
		// Admins can change the policy
		AdminChangePolicy = 0x2000,
		// more stuff to come perhaps

	}

	#[derive(BitBaggable, EnumIter, Clone, Copy)]
	#[repr(u8)]
	enum State {
		Listening = 0x01,
	}

	struct Path {
		path: path::Path,   // lol
		recurse: bool,
	}

	pub struct Group {
		// Path of each file / folder
		nodes: Vec<group::Path>,
		// The watcher
		watcher: Watcher,
		// We need peer structures to notify them and update the specific file.
		peers: Vec<*Peer>,
		policies: BitBag<Policy>,
		def_perms: BitBag<Permissions>,
		name: String,
		// upper half is random, lower half is the # of sec since unix epoch
		ident: u128,
		state: BitBag<State>,
	}

	// default policy is full access, delete on empty, and no sync access times
	const DEFAULT_POLICY: u16 = 0xEFFF;
	const DEFAULT_STATE: u8 = 0;

	impl Group {
		pub fn new ( name: String ) -> Self {
			let big_time = SystemTime::now().duration_since( SystemTime::UNIX_EPOCH ).unwrap().as_secs();
			Group {
				nodes: Vec::new(),
				watcher: Watcher::new::<Group>( self, notify::Config::default() ),
				peers: Vec::new(),
				policies: BitBag::<Policy>::new( DEFAULT_POLICY ),
				name: name,
				ident: ( big_time as u128 ) + ( ( random::<u64>() as u128 ) << 64 ),
				state: BitBag::<State>::new( DEFAULT_STATE )
			}
		}

		// These are operations controlled at the system level and irrelevant to permissions
		pub fn monitor ( &mut self ) {
			nodes.into_iter().for_each( | p | { watcher.watch( p.path, p.recurse ); } );
		}
		pub fn stop_monitor ( &mut self ) {
			nodes.into_iter().for_each( | p | { watcher.unwatch( p.path ); } ); 
		}
		
		pub fn add_node ( &mut self, path: path::Path, recurse: bool, perms: BitBag<Permissions> ) -> Result<(), usize> {
			if perms.is_set( Permissions::Write ) {
				nodes.push( group::Path { path, recurse } );
				// sync with the peers
				Ok( () )
			} else {
				Err( 1 )
			}
		}
		pub fn remove_node ( &mut self, path: path::Path, perms: BitBag<Permissions> ) -> Result<(), usize> {
			let mut it_removed: bool = false;
			nodes.retain( |p| { if p.path == path { 
				it_removed = true; 
				false 
			} else { true } );
			
			if it_removed {
				// sync with the peers
				Ok( () )
			} else {
				Err( 1 )
			}
		}

		pub fn add_peer ( &mut self, new_peer: *Peer, new_perms: Permissions, perms: Permissions ) -> Result<(), usize> {
			if new_perms.is_set( Permissions::Empty ) {
				// create peer with default permissions
			} else {
				// do our permissions violate group rules?
			}
		}
		pub fn remove_peer ( &mut self, peer: *Peer ) {

		}
		pub fn is_peer ( &self ) {
			
		}

		pub fn policies ( &self ) -> BitBag::<Policy> {
			self.policies
		}
		pub fn name ( &self ) -> BitBag::<Policy> {
			self.name
		}
		pub fn ident ( &self ) -> u128 {
			self.ident
		}
	}

	impl EventHandler for Group {
		fn handle_event ( &mut self, event: notify::Result<Event> ) {
			println!("Event: {:?}", event);
			match event {
				Ok( Access( kind: AccessKind ) ) => {
					
				},
				Ok( Create( kind: CreateKind ) ) => {

				},
				Ok( Modify( kind: ModifyKind ) ) => {

				}, 
				Ok( Remove( kind: RemoveKind ) ) => {
					self.
				},
				Err( e ) => {
					warn!( "Watcher: File event is err", e );
				},
				_ => {
					warn!( "Watcher: event", None );
				}
			}
		}
	}

}