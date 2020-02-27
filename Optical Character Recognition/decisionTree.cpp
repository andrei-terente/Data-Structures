// copyright Luca Istrate, Andrei Medar

#include "./decisionTree.h"  // NOLINT(build/include)
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include <map>

using std::string;
using std::pair;
using std::vector;
using std::unordered_map;
using std::make_shared;

// structura unui nod din decision tree
// splitIndex = dimensiunea in functie de care se imparte
// split_value = valoarea in functie de care se imparte
// is_leaf si result sunt pentru cazul in care avem un nod frunza
Node::Node() {
    is_leaf = false;
    left = nullptr;
    right = nullptr;
}

void Node::make_decision_node(const int index, const int val) {
    split_index = index;
    split_value = val;
}

void Node::make_leaf(const vector<vector<int>> &samples,
                     const bool is_single_class) {
    // TODO(you)
    // Seteaza nodul ca fiind de tip frunza (modificati is_leaf si result)
    // is_single_class = true -> toate testele au aceeasi clasa (acela e result)
    // is_single_class = false -> se alege clasa care apare cel mai des
    if (is_single_class == false) {
        std::vector<int> class_count;
        class_count.resize(samples.size());
        for (int i = 0; i < samples.size(); i ++) {
            class_count[samples[i][0]]++;
        }
        int maxi = class_count[0];
        int p;
        for (int i = 0; i < class_count.size(); i ++) {
            if (class_count[i] > maxi) {
                maxi = class_count[i];
                p = i;
            }
        }
        this->result = p;
        this->is_leaf = true;
    } else {
    	this->result = samples[0][0];
    	this->is_leaf = true;
    }
}

pair<int, int> find_best_split(const vector<vector<int>> &samples,
                               const vector<int> &dimensions) {
    // TODO(you)
    // Intoarce cea mai buna dimensiune si valoare de split dintre testele
    // primite. Prin cel mai bun split (dimensiune si valoare)
    // ne referim la split-ul care maximizeaza IG
    // pair-ul intors este format din (split_index, split_value)
    int splitIndex = -1, splitValue = -1;
    return pair<int, int>(splitIndex, splitValue);
}

void Node::train(const vector<vector<int>> &samples) {
    // TODO(you)
    // Antreneaza nodul curent si copii sai, daca e nevoie
    // 1) verifica daca toate testele primite au aceeasi clasa (raspuns)
    // Daca da, acest nod devine frunza, altfel continua algoritmul.
    // 2) Daca nu exista niciun split valid, acest nod devine frunza. Altfel,
    // ia cel mai bun split si continua recursiv
	bool is_same_class = same_class(samples);
	if (is_same_class) {
		this->make_leaf(samples, is_same_class);
	}
}

int Node::predict(const vector<int> &image) const {
    // TODO(you)
    // Intoarce rezultatul prezis de catre decision tree
    return 0;
}

bool same_class(const vector<vector<int>> &samples) {
    // TODO(you)
    // Verifica daca testele primite ca argument au toate aceeasi
    // clasa(rezultat). Este folosit in train pentru a determina daca
    // mai are rost sa caute split-uri
    int curr_class  = samples[0][0];
    for (int i = 1; i < samples.size(); i ++) {
        if (samples[i][0] != curr_class) {
            return false;
        }
    }

    return true;
}

float get_entropy(const vector<vector<int>> &samples) {
    // Intoarce entropia testelor primite
    assert(!samples.empty());
    vector<int> indexes;

    int size = samples.size();
    for (int i = 0; i < size; i++) indexes.push_back(i);

    return get_entropy_by_indexes(samples, indexes);
}

float get_entropy_by_indexes(const vector<vector<int>> &samples,
                             const vector<int> &index) {
    // TODO(you)
    // Intoarce entropia subsetului din setul de teste total(samples)
    // Cu conditia ca subsetul sa contina testele ale caror indecsi se gasesc in
    // vectorul index (Se considera doar liniile din vectorul index)
    // <clasa, nr. aparitii clasa>
    std::map<int, int> classes;
    int n_tests = index.size();
    for (int i = 0; i < n_tests; ++i) {
    	// stabiliarea nr. de aparitii al fiecarei clase
    	classes[samples[index[i]][0]]++;
    }
    float entropy = 0.0f;
    // daca clasa e stocata in map, e garantat faptul ca aceasta
    // a aparut cel putin odata, deci pi != 0
    for (auto it = classes.begin(); it != classes.end(); ++it) {
    	float pi = (float) it->second / n_tests;
    	entropy += pi * log2(pi);
    }
    entropy = (-1) * entropy;
    return entropy;
}

vector<int> compute_unique(const vector<vector<int>> &samples, const int col) {
    // TODO(you)
    // Intoarce toate valorile (se elimina duplicatele)
    // care apar in setul de teste, pe coloana col
    vector<int> uniqueValues;
    std::map<int, int> table;
    for (int i = 0; i < samples.size(); i ++) {
        table[samples[i][col]]++;
    }
    for (auto it = table.begin(); it != table.end(); ++it) {
        uniqueValues.push_back(it->first);
    }
    return uniqueValues;
}

pair<vector<vector<int>>, vector<vector<int>>> split(
    const vector<vector<int>> &samples, const int split_index,
    const int split_value) {
    // Intoarce cele 2 subseturi de teste obtinute in urma separarii
    // In functie de split_index si split_value
    vector<vector<int>> left, right;

    auto p = get_split_as_indexes(samples, split_index, split_value);
    for (const auto &i : p.first) left.push_back(samples[i]);
    for (const auto &i : p.second) right.push_back(samples[i]);

    return pair<vector<vector<int>>, vector<vector<int>>>(left, right);
}

pair<vector<int>, vector<int>> get_split_as_indexes(
    const vector<vector<int>> &samples, const int split_index,
    const int split_value) {
    // TODO(you)
    // Intoarce indecsii sample-urilor din cele 2 subseturi obtinute in urma
    // separarii in functie de split_index si split_value
    vector<int> left, right;
    for (int i = 0; i < samples.size(); i++) {
        if (samples[i][split_index] <= split_value)
            left.push_back(i);
        else
            right.push_back(i);
    }
    return make_pair(left, right);
}

std::vector<int> random_dimensions(const int size) {
    // TODO(you)
    // Intoarce sqrt(size) dimensiuni diferite pe care sa caute splitul maxim
    // Precizare: Dimensiunile gasite sunt > 0 si < size
	std::random_device rd;
    std::mt19937 mt_rand(rd());
    // Se seteaza range-ul cerut pt. generarea numerelor
    std::uniform_int_distribution<int> dist(1 , size - 1);
    // contor = nr. de numere ramase de generat
    int contor = floor(sqrt(size));
    // ret = vectorul de return, ce va contine numerele generate
    std::vector<int> ret;
    // Pentru a stabili daca nr. curent a mai fost generat
    // anterior folosim un hashtable care stocheaza perechile
    // (nr. curent, nr. aparitii)
    std::map<int, int> table;
    int x;
    while (contor) {
    	x = dist(mt_rand);
    	// Actualizarea nr. de aparitii ale lui x
    	table[x]++;
    	// Daca elementul nu a mai fost generat anterior
    	if (table[x] == 1) {
    		// se adauga in vectorul de return si se
    		// modifica nr. de elem. ramase de generat
    		ret.push_back(x);
    		contor--;
    	}
    }
    return ret;
}
