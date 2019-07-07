#include "main.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iterator>

#include <signal.h>
#include <sys/wait.h>

#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/torrent_info.hpp"
#include "libtorrent/magnet_uri.hpp"
#include "libtorrent/create_torrent.hpp"
#include "libtorrent/hasher.hpp"

#define TD_FILE_BUFFER 16000

class generic_alert: public lt::alert {
    std::string memberMessage = "{}";

    generic_alert(std::string message) {
        memberMessage = message;
    }

    static const int alert_type = 200;
    virtual int type() const throw() {
        return alert_type;
    }

    virtual std::string message() const {
        return memberMessage;
    }

    static constexpr lt::alert_category_t static_category = lt::alert::peer_notification;
    virtual lt::alert_category_t category() const throw() {
        return static_category;
    }

    virtual char const* what() const throw() {
        return "Generic Alert";
    }

};

lt::sha1_hash hash_sha1_file(const std::string& fname) {
    char buffer[TD_FILE_BUFFER];
    lt::hasher hash;
    std::ifstream ifs(fname, std::ifstream::binary);

    while (ifs.good()) {
        ifs.read(buffer, TD_FILE_BUFFER);
        hash.update(buffer, ifs.gcount()); 
    }
    ifs.close();

    return hash.final();
}

/**
 * Executes a given command in a new process.
 *
 * @param command       The command and arguments to execute.
 * @param iopaths       The paths to redirect IO to, or null.
 */
void td_host(std::string file) {
    if (fork() == 0) {
        // reset SIGINT (^C) to normal behavior
        signal(SIGINT, SIG_DFL);

        lt::session s;
        lt::add_torrent_params p;
        p.ti = std::make_shared<lt::torrent_info>(file);
        s.add_torrent(p);

        // wait for the user to end
        char a;
        int ret = std::scanf("%c\n", &a);
        (void)ret; // ignore
        
        exit(1);
    } else {
        int status;
        int childpid = waitpid(-1, &status, 0);

        if (childpid == -1) {
            // oof.
            printf("Error running child process.\n");
        } else if (!WIFEXITED(status)) {
            if (WIFSIGNALED(status)) {
                printf("Program terminated with signal: %s.\n", strsignal(WTERMSIG(status)));
            } else {
                printf("Program terminated abnormally.\n");
            }
        }
    }
}

extern "C" {
    void printHello() {
        std::cout << "hi!" << std::endl;
    }

    int init_host(const char** files, const unsigned int files_count, char* to_url) {
        std::string magnet_uri = "";

        lt::file_storage fs;

        for (unsigned int i = 0; i < files_count; i++) {
            lt::add_files(fs, files[i]);
        }

        if (fs.num_files() == 0) {
            std::cerr << "no files specified.\n";
            return 0;
        }

        std::cout << "debug creating torrent\n";

        lt::create_torrent torrent(fs);
        torrent.add_tracker("http://tracker.opentrackr.org:1337/announce");
        torrent.set_creator("libtorrent example");

        // reads the files and calculates the hashes
        for (unsigned int i = 0; i < files_count; i++) {
            torrent.set_file_hash(i, hash_sha1_file(files[i]));
        }

        std::cout << "debug creating file\n";

        std::ofstream out("bit.torrent", std::ios_base::binary);
        lt::bencode(std::ostream_iterator<char>(out), torrent.generate());

        std::cout << "debug analyzing file\n";

        lt::error_code ec;
        lt::torrent_info ti("bit.torrent", ec);
        magnet_uri = lt::make_magnet_uri(ti);

        strcpy(to_url, magnet_uri.c_str());

        td_host("bit.torrent");

        return 1;
    }

    int init_magnet(const char* url) {
        std::cout << url << std::endl;

        lt::session ses;

        lt::add_torrent_params atp = lt::parse_magnet_uri(url);
        atp.save_path = "."; // save in current dir
        lt::torrent_handle h = ses.add_torrent(atp);

        return 1;
    }
}
