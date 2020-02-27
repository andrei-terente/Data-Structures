// copyright Luca Istrate, Andrei Medar
#include "randomForest.h"
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "decisionTree.h"

using std::vector;
using std::pair;
using std::string;
using std::mt19937;

std::random_device rd;
std::mt19937 mt_rand(rd());


vector<vector<int>> get_random_samples(const vector<vector<int>> &samples,
                                       int num_to_return) {
    // TODO(you)
    // Intoarce un vector de marime num_to_return cu elemente random,
    // diferite din samples
    std::random_device rd;
    std::mt19937 mt_rand(rd());
    // Se genereaza indici random in range-ul [0, samples.size() - 1]
    std::uniform_int_distribution<int> dist(0, samples.size() - 1);
    std::vector<int> indexes;
    vector<vector<int> > ret;
    int x;
    // Pentru a stabili daca indexul curent a mai fost generat
    // anterior folosim un hashtable care stocheaza perechile
    // (indice curent, nr. aparitii)
    std::map<int, int> table;
    while (num_to_return) {
        x = dist(mt_rand);
        // Actualizarea nr. de aparitii al indicelui generat curent
        table[x]++;
        // Daca nr. de aparitii = 1, acesta nu a fost generat anterior
        if (table[x] == 1) {
        	// Se adauga indicele in vectorul de indici si
        	// se actualizeaza nr. de elem. ramase de generat
        	indexes.push_back(x);
        	num_to_return--;
        }
    }
    // Se adauga sample-urile corespunzatoare indicilor
    // in vectorul de return
    for (int i = 0; i < indexes.size(); ++i) {
        ret.push_back(samples[indexes[i]]);
    }
    return ret;
}

RandomForest::RandomForest(int num_trees, const vector<vector<int>> &samples)
    : num_trees(num_trees), images(samples) {}

void RandomForest::build() {
    // Aloca pentru fiecare Tree cate n / num_trees
    // Unde n e numarul total de teste de training
    // Apoi antreneaza fiecare tree cu testele alese
    assert(!images.empty());
    vector<vector<int>> random_samples;

    int data_size = images.size() / num_trees;

    for (int i = 0; i < num_trees; i++) {
        // cout << "Creating Tree nr: " << i << endl;
        random_samples = get_random_samples(images, data_size);

        // Construieste un Tree nou si il antreneaza
        trees.push_back(Node());
        trees[trees.size() - 1].train(random_samples);
    }
}

int RandomForest::predict(const vector<int> &image) {
    // TODO(you)
    // Va intoarce cea mai probabila prezicere pentru testul din argument
    // se va interoga fiecare Tree si se va considera raspunsul final ca
    // fiind cel majoritar
    return 1;
}
