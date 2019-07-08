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
#define ltd libtorrentdata

/*class generic_alert: public lt::alert {
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

};*/

namespace ltd {

    std::string m_torrent_trackers[] = { "http://tracker.opentrackr.org:1337/announce" };

    /**
     * Calculates the SHA1 hash of a given file.
     *
     * @param hash          The hasher to write to.
     * @param fname         The absolute path of the file to hash.
     * @returns 1 if hashing failed; 0 otherwise.
     * @see https://www.libtorrent.org/reference-Utility.html#hasher
     */
    int hash_sha1_file(lt::hasher& hash, const std::string& fname) {
        char buffer[TD_FILE_BUFFER];
        std::ifstream ifs(fname, std::ifstream::binary);
    
        while (ifs.good()) {
            ifs.read(buffer, TD_FILE_BUFFER);
            hash.update(buffer, ifs.gcount()); 
        }
        ifs.close();
    
        return 0;
    }

    /**
     * Hosts (seeds) a torrent file.
     *
     * @param file          The path of the torrent file.
     */
    void torrent_host(std::string file) {
        lt::session s;
        lt::add_torrent_params p;
        p.ti = std::make_shared<lt::torrent_info>(file);
        p.save_path = "./";
        s.add_torrent(p);
    
        // wait for the user to end
        char a;
        int ret = std::scanf("%c\n", &a);
        (void)ret; // ignore
    }

    /**
     * Hosts a torrent file in a new process.
     *
     * @param file          The path of the torrent file.
     */
    /*void torrent_host_async(std::string file) {
        if (fork() == 0) {
            // reset SIGINT (^C) to normal behavior
            signal(SIGINT, SIG_DFL);
            torrent_host(file);
            
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
    }*/

    int fs_add_files(lt::file_storage& fs, const char** files, const unsigned int files_count) {
        for (unsigned int i = 0; i < files_count; i++) {
            lt::add_files(fs, files[i]);
        }

        if (fs.num_files() == 0) {
            std::cerr << "no files specified.\n";
            return 1;
        }

        return 0;
    }

    int torrent_create(lt::create_torrent& torrent) {
        for (std::string& tracker : m_torrent_trackers) {
            torrent.add_tracker(tracker);
        }
        
        torrent.set_creator("libtorrent-data");
        return 0;
    }

    int torrent_hash_files(lt::create_torrent& torrent, const char** files, const unsigned int files_count) {
        lt::hasher hash;
        for (unsigned int i = 0; i < files_count; i++) {
            if (ltd::hash_sha1_file(hash, files[i])) {
                hash.reset();
                return 1;
            }

            torrent.set_file_hash(i, hash.final());
            hash.reset();
        }

        return 0;
    }

    int torrent_write(lt::create_torrent torrent, std::string file) {
        std::vector<char> temp;
        lt::bencode(std::back_inserter(temp), torrent.generate());

        std::fstream out;
        out.exceptions(std::ifstream::failbit);
        out.open(file.c_str(), std::ios_base::out | std::ios_base::binary);
        out.write(temp.data(), temp.size());

        return 0;
    }
    
}

extern "C" {

    int init_host(const char** files, const unsigned int files_count, char* url_buffer, const unsigned int url_buffer_size) {
        std::string magnet_uri = "";

        lt::file_storage fs;
        if (ltd::fs_add_files(fs, files, files_count))
            return 1;

        lt::create_torrent torrent(fs);
        if (ltd::torrent_create(torrent))
            return 1;

        if (ltd::torrent_hash_files(torrent, files, files_count))
            return 1;

        if (ltd::torrent_write(torrent, "bit.torrent"))
            return 1;

        lt::error_code ec;
        lt::torrent_info ti("bit.torrent", ec);
        magnet_uri = lt::make_magnet_uri(ti);
        
        strncpy(url_buffer, magnet_uri.c_str(), url_buffer_size-1);

        ltd::torrent_host("bit.torrent");

        return 0;
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
