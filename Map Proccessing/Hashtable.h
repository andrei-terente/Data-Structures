#define VMAX 10000

template <typename Tkey, typename Tvalue>
struct elem_info {
	Tkey key;
	Tvalue value;
};

template <typename Tkey, typename Tvalue>
class Hashtable {
 public:
 	int max_size; // number of buckets
 	int size; // current number of elements
 	std::list<struct elem_info<Tkey, Tvalue> > *H; // array of buckets
 	int (*hash)(Tkey); // pointer to hash function

 	// Constructor
 	Hashtable(int max_size, int (*h)(Tkey)) {
 		this->max_size = max_size;
 		// indicating the hash function I want to use
 		hash = h;
 		// allocating memory for the bucket array
 		H = new std::list<struct elem_info<Tkey, Tvalue> >[max_size];
 	}

 	// Copy-Constructor
 	Hashtable(const Hashtable &t) {
 		max_size = t.max_size;
 		hash = t.hash;
 		H = new std::list<struct elem_info<Tkey, Tvalue> >[max_size];
 		// typename std::list<struct elem_info<Tkey, Tvalue> > it;
 		// deep-copy
 		for (int i = 0; i < max_size; ++i) {
 			for (auto it = t.H[i].begin(); it != t.H[i].end(); ++it) {
 				H[i].push_back(*it);
 			}
 		}
 	}

 	// Copy-Assignment Operator
 	Hashtable operator=(const Hashtable& t) {
 		// if (this != t) {
 		delete[] H;
 		max_size = t.max_size;
 		hash = t.hash;
 		H = new std::list<struct elem_info<Tkey, Tvalue> >[max_size];
 		// typename std::list<struct elem_info<Tkey, Tvalue> >::iterator it;
	 	// deep-copy
		for (int i = 0; i < max_size; ++i) {
			for (auto it = t.H[i].begin(); it != t.H[i].end(); ++it) {
				H[i].push_back(*it);
 			}
 		}
 		return *this;
 	}

 	// Deconstructor
 	~Hashtable() {
 		delete[] H; // freeing the bucket array
 	}

 	// Function to put an element into the hashtable
 	void put(Tkey key, Tvalue value) {
 		// getting the index
 		int index = hash(key);
 		// iterating through the bucket to check if the key was already put
 		typename std::list<struct elem_info<Tkey, Tvalue> >::iterator it;
 		for (it = H[index].begin(); it != H[index].end(); ++it) {
 			// if it is, I update the value and return
 			if (it->key == key) {
 				it->value = value;
 				return;
 			}
 		}
 		// if the key wasn't found, I add the key-value pair to the bucket
 		struct elem_info <Tkey, Tvalue> aux;
 		aux.key = key;
 		aux.value = value;
 		H[index].push_front(aux);
 	}

 	// Function to return the coresponding value of a given key
 	Tvalue get(Tkey key) {
 		// getting the index
 		int index = hash(key);
 		// iterating through the bucket to find the coresponding value
 		typename std::list<struct elem_info<Tkey, Tvalue> >::iterator it;
 		for (it = H[index].begin(); it != H[index].end(); ++it) {
 			if (it->key == key) {
 				return it->value;
 			}
 		}
 		// if the key isn't found, I display an error message
 		std::cout << "Error: Given key(" << key << ") was not found\n";
 	}

 	// Function to check if table has given key
 	bool hasKey (Tkey key) {
 		int index = hash(key);
 		typename std::list<struct elem_info<Tkey, Tvalue> >::iterator it;
 		for (it = H[index].begin(); it != H[index].end(); ++it) {
 			if (it->key == key) {
 				return true;
 			}
 		}
 		return false;
 	}

 	void display() {
 		std::cout << "Hastable contains:\n";
 		typename std::list<struct elem_info<Tkey, Tvalue> >::iterator it;
 		for (int i = 0; i < max_size; ++i) {
 			if (H[i].size()) {
 				std::cout << "[" << i << "]: ";
 				for (it = H[i].begin(); it != H[i].end(); ++it) {
 					std::cout << "(" << it->key << ", " << it->value << ") ";
 				}
 				std::cout << std::endl;
 			}
 		}
 	}

};
