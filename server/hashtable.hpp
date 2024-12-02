// constructor
template<typename K, typename V>
HashTable<K, V>::HashTable(size_t size) {
    // Create default hash table
    theLists = vector<list<pair<K, V>>>(prime_below(size));
    currentSize = prime_below(size);
    // Create the vector
    if(prime_below(size) == 0){
        // Adjust to default size
        theLists.resize(default_capacity);
        currentSize = 0;
    }
    else{
        // Make sure the vector is the right size
        theLists.resize(prime_below(size));
        currentSize = 0;
    }
    // Output added to match output of proj4.x even though it wasn't mentioned in the Canvas
    cout << "In total there are " << theLists.size() << " rows in the vector. " << endl;
}

// destructor
template<typename K, typename V>
HashTable<K, V>::~HashTable() {
    clear();
}

//contains
template<typename K, typename V>
bool HashTable<K, V>::contains(const K &k) const {
    auto & whichList = theLists[myhash(k)];
    // Loop through this list to see if it has a matching key
    auto itr = whichList.begin();
    while (itr != whichList.end()) {
        if (itr->first == k) {
            return true;
        }
        itr++;
    }
    // Not found
    return false;
}

//match
template<typename K, typename V>
bool HashTable<K, V>::match(const pair <K, V> &kv) const {
    auto & whichList = theLists[myhash(kv.first)];
    // Loop through this list to see if it has a matching key
    auto itr = whichList.begin();
    while (itr != whichList.end()) {
        if ((itr->first == kv.first) && (itr->second == kv.second)) {
            return true;
        }
        itr++;
    }
    // Not found
    return false;
}

//insert
template<typename K, typename V>
bool HashTable<K, V>::insert(const pair <K, V> &kv) {
    // Check if the pair is already in the table
    if (match(kv)){
        return false;
    }
    if (contains(kv.first)){
        auto & whichList = theLists[myhash(kv.first)];
        // Loop through this list to see if it has a matching key
        auto itr = whichList.begin();
        while (itr != whichList.end() && !(itr->first == kv.first)) {
            itr++;
        }
        // itr now points to the pair we want to update
        itr->second = kv.second;
    }
    else {
        auto & whichList = theLists[myhash(kv.first)];
        // Push back the pair
        whichList.push_back(kv);
        currentSize++;
        // Resize if necessary
        if (currentSize > theLists.size()) {
            rehash();
        }
    }
    return true;
}

//move insert
template<typename K, typename V>
bool HashTable<K, V>::insert(pair <K, V> &&kv) {
    // Check if the pair is already in the table
    if (!match(kv)){
        return false;
    }
    if (contains(kv.first)){
        auto & whichList = theLists[myhash(kv.first)];
        // Loop through this list to see if it has a matching key
        auto itr = whichList.begin();
        while (itr != whichList.end() && !(itr->first == kv.first)) {
            itr++;
        }
        // itr now points to the pair we want to update
        itr->second = std::move(kv.second);
    }
    else {
        auto & whichList = theLists[myhash(kv.first)];
        // Push back the pair
        whichList.push_back(std::move(kv));
        currentSize++;
        // Resize if necessary
        if (currentSize > theLists.size()) {
            rehash();
        }
    }
    return true;
}

//remove
template<typename K, typename V>
bool HashTable<K, V>::remove(const K &k) {
    auto & whichList = theLists[myhash(k)];
    // Find the specified key
    auto itr = whichList.begin();
    while ((itr != whichList.end()) && (itr->first != k)) {
        itr++;
    }
    // Return false if the key is not found
    if (itr == whichList.end()){
        return false;
    }
    // Perform the removal
    whichList.erase(itr);
    // Update the size
    currentSize--;
    return true;
}

// Clear
template<typename K, typename V>
void HashTable<K, V>::clear() {
    makeEmpty();
}

// Load
template<typename K, typename V>
bool HashTable<K, V>::load(const char *filename) {
    // Open file
    ifstream file;
    file.open(filename);
    // Loop through the whole file
    if (file.is_open()) {
        // Read each line from file
        string line;
        // Loop
        while(getline(file, line)) {
            // Create variables
            K k;
            V v;
            // Create input string stream to perform extraction
            istringstream iss(line);
            iss >> k >> v;
            // Check if contains k
            if (contains(k)) {
                cout << "Issue reading line in file: " << line << '\n';
            }
            else {
                // Create the pair and insert it
                pair<K,V> kv = make_pair(k,v);
                insert(kv);
            }
        }
        // Cleanup
        file.close();
        return true;
    }
    else {
        return false;
    }
}

template<typename K, typename V>
void HashTable<K, V>::dump() const {
    // Output added to match output of proj4.x even though it wasn't mentioned in the Canvas
    cout << "In dump, in total there are " << theLists.size() << " rows in the vector. " << endl;
    // Loop through the vector
    for (int i =0; i < theLists.size(); i++){
        // Print
        cout << "v[" << i << "]: ";
        // Get iterator for the attached list
        auto itr = theLists[i].begin();
        // Create a tracking variable as well
        int count = 0;
        // Loop through
        while (itr != theLists[i].end()) {
            if (count >= 1) {
                // Need a colon
                cout << ":";
            }
            // Print the kv pair
            cout << itr->first << " " << itr->second;
            // Iterate
            count++;
            itr++;
        }
        // New line
        cout << endl;
    }
}

template<typename K, typename V>
bool HashTable<K, V>::write_to_file(const char *filename) const {
    ofstream fh(filename);
    if (fh.is_open()){
        // Loop through the vector
        for (int i =0; i < theLists.size(); i++){
            // Get iterator for the attached list
            auto itr = theLists[i].begin();
            while (itr != theLists[i].end()) {
                fh << itr->first << " " << itr->second << "\n";
                itr++;
            }
        }
        fh.close();
        return true;
    }
    return false;
}



// size_t size
template<typename K, typename V>
size_t HashTable<K, V>::size() const {
    return currentSize;
}

// make empty
template<typename K, typename V>
void HashTable<K, V>::makeEmpty() {
    for (int i = 0; i < theLists.size(); i++) {
        // Clear individual lists
        theLists[i].clear();
    }
    // Clear the lists
    theLists.clear();
}

// Rehash
template<typename K, typename V>
void HashTable<K, V>::rehash() {
    vector<list<pair<K, V>>> oldLists = theLists;
    // Create new double sized prime, and empty the table
    theLists.resize(prime_below(2 * theLists.size()));
    for (auto & thisList : theLists){
        thisList.clear();
    }
    //Copy the table over
    currentSize = 0;
    for (auto & thisList : oldLists){
        for (auto & x : thisList){
            insert(x);
        }
    }
}



// My hash function
template<typename K, typename V>
size_t HashTable<K, V>::myhash(const K &k) const {
    static hash<K> hf;
    // Adding this output here to match the proj4.x outputs given, even though it wasn't included in the canvas directions.
    size_t index = hf(k)%theLists.size();
    cout << "hashed position of " << k << " is " << hf(k) << endl;
    return (index);
}


template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below(unsigned long n)
{
  if (n > max_prime)
    {
      std::cerr << "** input too large for prime_below()\n";
      return 0;
    }
  if (n == max_prime)
    {
      return max_prime;
    }
  if (n <= 1)
    {
		std::cerr << "** input too small \n";
      return 0;
    }

  // now: 2 <= n < max_prime
  std::vector <unsigned long> v (n+1);
  setPrimes(v);
  while (n > 2)
    {
      if (v[n] == 1)
	return n;
      --n;
    }

  return 2;
}

template <typename K, typename V>
void HashTable<K, V>::setPrimes(vector<unsigned long> & vprimes)
{
  int i = 0;
  int j = 0;

  vprimes[0] = 0;
  vprimes[1] = 0;
  int n = vprimes.capacity();

  for (i = 2; i < n; ++i)
    vprimes[i] = 1;

  for( i = 2; i*i < n; ++i)
    {
      if (vprimes[i] == 1)
        for(j = i + i ; j < n; j += i)
          vprimes[j] = 0;
    }
}

