#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

using namespace std;

// Educational hash function.
// This is NOT secure enough for real passwords.
uint64_t demoHash(const string& s) {
    uint64_t h = 1469598103934665603ULL;

    for (unsigned char c : s) {
        h = (h ^ c) * 1099511628211ULL;
    }

    return h;
}
// Used for the collision strategy implementations. 
size_t simpleHash (const string& key, size_t size) {
    return demoHash(key) % size; 
}

struct User {
    string username; 
    string salt; 
    uint64_t passwordHash; 
};

const string PEPPER = "Carolina_Reaper$%92@x"; 

class LinearProbingTable {
    private: 
        vector<string> table; 
    
    public: 
        LinearProbingTable(size_t size) {
            table.resize(size, "");
        }

        void insert(const string& key) {

            if (find(key)) {
                cout << "Key already exists.\n";
                return; 
            }
            
            size_t index = simpleHash(key, table.size());

            for (size_t i = 0; i < table.size(); i++) {
                size_t newIndex = (index + i) % table.size();

                if (table[newIndex] == "") {
                    table[newIndex] = key;
                    return; 
                }
            }

            cout << "Linear probing table is full.\n";
        }

        bool find(const string& key) {
            size_t index = simpleHash(key, table.size());

            for (size_t i = 0; i < table.size(); i++) {
                size_t newIndex = (index + i) % table.size();

                if (table[newIndex] == key) {
                    return true; 
                }
                else if (table[newIndex] == "") {
                    return false;
                }
            }

            return false; 
        }

        void printTable() const {
            for (size_t i = 0; i < table.size(); i++) {
                if (table[i] != "") {
                    cout << "Index " << i << ": "
                         << table[i] << "\n";
                }
            }
        }
};

class QuadraticProbingTable {
    private: 
        vector<string> table; 
    
    public: 
        QuadraticProbingTable(size_t size) {
            table.resize(size, "");
        }

        void insert(const string& key) {

            if (find(key)) {
                cout << "Key already exists.\n";
                return; 
            }

            size_t index = simpleHash(key, table.size());

            for (size_t i = 0; i < table.size(); i++) {
                size_t newIndex = (index + i * i) % table.size();

                if (table[newIndex] == "") {
                    table[newIndex] = key; 
                    return; 
                }
            }

            cout << "Quadratic probing table is full.\n";
        }

        bool find(const string& key) {
            size_t index = simpleHash(key, table.size());

            for (size_t i = 0; i < table.size(); i++) {
                size_t newIndex = (index + i * i) % table.size();

                if (table[newIndex] == key) {
                    return true; 
                }

                if (table[newIndex] == "") {
                    return false; 
                }
            }

            return false; 
        }

        void printTable() const {
            for (size_t i = 0; i < table.size(); i++) {
                if (table[i] != "") {
                    cout << "Index " << i << ": "
                         << table[i] << "\n";
                }
            }
        }
};

class DoubleHashingTable {
    private: 
        vector<string> table; 

        size_t secondHash(const string& key) {
            return 1 + (demoHash(key) % (table.size() - 1));
        }
    
    public: 
        DoubleHashingTable(size_t size) {
            table.resize(size, "");
        }

        void insert(const string& key) {

            if (find(key)) {
                cout << "Key already exists.\n";
                return; 
            }

            size_t index = simpleHash(key, table.size());
            size_t step = secondHash(key);

            for (size_t i = 0; i < table.size(); i++) {
                size_t newIndex = (index + i * step) % table.size();

                if (table[newIndex] == "") {
                    table[newIndex] = key;
                    return;
                }
            }

            cout << "Double hashing table is full.\n";
        }

        bool find(const string& key) {
            size_t index = simpleHash(key, table.size());
            size_t step = secondHash(key);

            for (size_t i = 0; i < table.size(); i++) {
                size_t newIndex = (index + i * step) % table.size();

                if (table[newIndex] == key) {
                    return true; 
                }
                else if (table[newIndex] == "") {
                    return false; 
                }
            }

            return false; 
        }

        void printTable() const {
            for (size_t i = 0; i < table.size(); i++) {
                if (table[i] != "") {
                    cout << "Index " << i << ": "
                         << table[i] << "\n";
                }
            }
        }
};

class HashTable {
    private: 
        vector<vector<User>> table; 
        size_t userCount; 

        size_t hash(const string& username) {
            return demoHash(username) % table.size();
        }

        void rehash() {
            cout << "\nRehashing table from "
                 << table.size() << " buckets to "
                 << table.size() * 2 << " buckets.\n";
            
            vector<vector<User>> oldTable = table; 

            table.clear();
            table.resize(oldTable.size() * 2);

            for (const vector<User>& bucket: oldTable) {
                for (const User& user: bucket) {
                    size_t index = hash(user.username);
                    table[index].push_back(user);
                }
            }
        }
    
    public: 
        HashTable(size_t size = 0) {

            if (size == 0) {
                size = 1; 
            }

            table.resize(size);
            userCount = 0; 
        }

        void insert(const User& user) {
            size_t index = hash(user.username);

            for (User& currentUser: table[index]) {
                if (currentUser.username == user.username) {
                    currentUser = user;
                    return;
                }
            }

            table[index].push_back(user);
            userCount++;
            
            if (loadFactor() > 0.75) {
                rehash();
            }
        }

        User* find(const string& username) {
            size_t index = hash(username); 

            for (User& currentUser: table[index]) {
                if (currentUser.username == username) {
                    return &currentUser; 
                }
            }

            return nullptr; 
        }

        bool remove(const string& username) {
            size_t index = hash(username);

            for (size_t i = 0; i < table[index].size(); i++) {
                if (table[index][i].username == username) {
                    table[index].erase(table[index].begin() + i);
                    userCount--; 
                    return true; 
                }
            }

            return false;
        }

        double loadFactor() const {
            return (double)userCount / table.size();
        }

        void printInfo() const{
            cout << "Users: " << userCount << "\n";
            cout << "Buckets: " << table.size() << "\n";
            cout << "Load Factor: " << loadFactor() << "\n";
        }

        void printTable() const {
            for(size_t i = 0; i < table.size(); i++) {
                if (!table[i].empty()) {
                    cout << "Bucket " << i << ": ";

                    for (const User& user: table[i]) {
                        cout << user.username << " "; 
                    }

                    cout << "\n";
                }
            }
        }
};

bool login(HashTable& users, 
                   const string& username, 
                   const string& password) {

            User* user = users.find(username);

            if (user == nullptr) {
                cout << "Login failed: User not found.\n";
                return false; 
            }

            uint64_t calculatedHash = demoHash(user->salt + password + PEPPER);

            if (calculatedHash == user->passwordHash) {
                cout << "Login successful for "
                     << username << ".\n";
                return true; 
            }

            cout << "Login failed: Incorrect password.\n";
            return false; 
}

int main() {
    cout << "\nCollision Strategy Demonstrations\n";

    cout << "\nLinear Probing:\n\n";

    LinearProbingTable linearTable(12);

    linearTable.insert("evan");
    linearTable.insert("maya");
    linearTable.insert("henry");

    linearTable.printTable();

    cout << "\nQuadratic Probing:\n\n";

    QuadraticProbingTable quadraticTable(12); 

    quadraticTable.insert("evan");
    quadraticTable.insert("maya");
    quadraticTable.insert("henry");

    quadraticTable.printTable();

    cout << "\nDouble Hashing:\n\n";

    DoubleHashingTable doubleHash(17);

    doubleHash.insert("evan");
    doubleHash.insert("maya");
    doubleHash.insert("henry");
    
    doubleHash.printTable();

    cout << "\nSeparate Chaining\n\n";

    HashTable hashTable(12);

    User evanEntry = {"evan", "salt1", 123};
    User mayaEntry = {"maya", "salt2", 456};
    User henryEntry = {"henry", "salt3", 789};

    hashTable.insert(evanEntry);
    hashTable.insert(mayaEntry);
    hashTable.insert(henryEntry);

    hashTable.printTable();

    HashTable users(5);
    
    string evanSalt = "saltEvan";
    string mayaSalt = "saltMaya";
    string henrySalt = "saltHenry";
    string sofiaSalt = "saltSofia";
    string jamesSalt = "saltJames";

    User evan = {
        "evan", 
        evanSalt, 
        demoHash(evanSalt + "JpL2f+221t)N" + PEPPER)
    };

    User maya = {
        "maya",
        mayaSalt, 
        demoHash(mayaSalt + "Dcsh774Kk/+4" + PEPPER)
    };

    User henry = {
        "henry", 
        henrySalt, 
        demoHash(henrySalt + "]5)1AQy67nfT" + PEPPER)
    };

    User sofia = {
        "sofia", 
        sofiaSalt, 
        demoHash(sofiaSalt + "t26P6+1AXD_Y" + PEPPER)
    };

    User james = {
        "james", 
        jamesSalt, 
        demoHash(jamesSalt + "4Pp97G<zL!rL" + PEPPER)
    };

    users.insert(evan);
    users.insert(maya);
    users.insert(henry);
    users.insert(sofia);
    users.insert(james);

    cout << "\nHash Table Information:\n";
    users.printInfo();

    cout << "\nSuccessful Login Attempt\n";
    login(users, "evan", "JpL2f+221t)N");

    cout << "\nFailed Password Attempt\n";
    login(users, "maya", "wrongPassword");

    cout << "\nUnknown User Attempt\n";
    login(users, "unknownUser", "password");

    cout << "\nRemoving User:\n";

    if (users.remove("henry")) {
        cout << "Henry was removed successfully.\n";
    } else {
        cout << "Could not remove Henry.\n";
    }

    if (users.find("henry") == nullptr) {
        cout << "Henry is no longer in the table.\n";
    }

    cout << "\nLogin Attempt After Removal\n";
    login(users, "henry", "]5)1AQy67nfT");
    
    cout << "\n";

    return 0; 
}