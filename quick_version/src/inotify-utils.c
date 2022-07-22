
#include "root.h"
#include "config.h"
#include "file-watch.h"

int IFILE_INST;

int int_dir_watcher ( const char* path ) {
	int this_inst = inotify_init();
	int inot_watch = inotify_add_watch( inot_inst, path, IN_MOVE_SELF | IN_MODIFY | IN_DELETE_SELF | IN_ATTRIB );
}

int init () {
	IFILE_INST = inotify_init();
}

int watcher (

int main () {

}
