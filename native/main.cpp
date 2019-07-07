#include "main.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>

#include "libtorrent/entry.hpp"
#include "libtorrent/bencode.hpp"
#include "libtorrent/session.hpp"
#include "libtorrent/torrent_info.hpp"
#include "libtorrent/magnet_uri.hpp"
#include "libtorrent/create_torrent.hpp"

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

void td_init_host(char* to_url) {
    std::string magnet_uri = "";
        
    lt::file_storage fs;
        
    lt::create_torrent torrent(fs);
    //torrent.add_tracker("http://my.tracker.com/announce");
    torrent.set_creator("libtorrent example");
        
    // reads the files and calculates the hashes
    lt::set_piece_hashes(torrent, ".");
        
    std::ofstream out("bit.torrent", std::ios_base::binary);
    lt::bencode(std::ostream_iterator<char>(out), torrent.generate());

    lt::error_code ec;
    lt::torrent_info ti("bit.torrent", ec);
    magnet_uri = lt::make_magnet_uri(ti);
        
    strcpy(to_url, magnet_uri.c_str());
}

void td_init_magnet(const char* url) {
    std::cout << url << std::endl;
            
    lt::session ses;
    
    lt::add_torrent_params atp = lt::parse_magnet_uri(url);
    atp.save_path = "."; // save in current dir
    lt::torrent_handle h = ses.add_torrent(atp);
}

extern "C" {
    void printHello() {
        std::cout << "hi!" << std::endl;
    }

    void init_host(char* to_url) {
        td_init_host(to_url);
    }

    void init_magnet(const char* url) {
        td_init_magnet(url);
    }
}

int main() {
}
