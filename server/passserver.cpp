#include "passserver.h"

// Xian: in a shell, type "man crypt", and the MD5 can be found (which uses 22 characters of the password), and the usage will be shown as well.
// Note that it takes c_str, char [], not string directly as the input arguments.
string PassServer::encrypt(const string & str) {
    char salt[] = "$1$########";
    char * password = new char [35];
    strcpy(password, crypt(str.c_str(), salt));

    string pword(password);
    string delim = "$";
    int pos = pword.find(delim, 0);
    pos = pword.find(delim, pos+1);
    pos = pword.find(delim, pos+1);
    pword.erase(0, pos+1);
    return pword;
    /*char salt[] = "$1$########";
    string ret = crypt(str.c_str(), salt);
    return ret.substr(1, 22);*/
}

PassServer::PassServer(size_t size) : h(size) {}

PassServer::~PassServer() {
    // Clear the hash table
    h.clear();
}

bool PassServer::load(const char *filename) {
    return h.load(filename);
}

bool PassServer::addUser(pair<string, string> &kv) {
    if (h.contains(kv.first) != true){
        auto newPair = std::make_pair(kv.first, encrypt(kv.second));
        h.insert(newPair);
        return true;
    }
    return false;
}

bool PassServer::addUser(pair<string, string> &&kv) {
    if (h.contains(kv.first) != true){
        pair<string, string> myPair = std::make_pair(std::move(kv.first), std::move(kv.second));
        pair<string, string> myPairEncrypted = std::make_pair(myPair.first, encrypt(myPair.second));
        h.insert(myPairEncrypted);
        return true;
    }
    return false;
}

bool PassServer::removeUser(const string &k) {
    return h.remove(k);
}

bool PassServer::changePassword(const pair<string, string> &p, const string &newpassword) {
    // See if user exists in table
    if (!h.contains(p.first)) {
        return false;
    }
    // Compare current and previous passwords
    // Encrypt password first
    pair<string,string> pNew = std::make_pair(p.first, encrypt(p.second));
    if (!h.match(pNew)) {
        return false;
    }
    // Check if new and old password are the same
    if (p.second == newpassword) {
        return false;
    }
    // Insert
    pair<string,string> pNewEnc = std::make_pair(p.first, encrypt(newpassword));
    h.insert(pNewEnc);
    return true;
}

bool PassServer::find(const string &user) const {
    return h.contains(user);
}

void PassServer::dump() const {
    h.dump();
}

size_t PassServer::size() const {
    return h.size();
}

bool PassServer::write_to_file(const char *filename) const {
    return h.write_to_file(filename);
}