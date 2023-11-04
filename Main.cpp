#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <locale>


using namespace std;
using namespace std::chrono;

class TrieNode {
public:
    vector<TrieNode*> symbol;
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false), symbol(26, nullptr) {}
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(string word) {
        TrieNode* current = root;
        for (char c : word) {
            int index = c - 'a';
            if (!current->symbol[index]) {
                current->symbol[index] = new TrieNode();
            }
            current = current->symbol[index];
        }
        current->isEndOfWord = true;
    }

    bool search(string word) {
        TrieNode* node = searchNode(word);
        return (node != nullptr) && (node->isEndOfWord);
    }

    bool startsWith(string prefix) {
        return (searchNode(prefix) != nullptr);
    }

    void remove(string word) {
        removeWord(root, word, 0);
    }

private:
    TrieNode* searchNode(string word) {
        TrieNode* current = root;
        for (char c : word) {
            int index = c - 'a';
            if (!current->symbol[index]) {
                return nullptr;
            }
            current = current->symbol[index];
        }
        return current;
    }

    bool removeWord(TrieNode* node, string word, int index) {
        if (index == word.length()) {
            if (node->isEndOfWord) {
                node->isEndOfWord = false;
                return isEmpty(node);
            }
            return false;
        }

        int charIndex = word[index] - 'a';
        if (node->symbol[charIndex] == nullptr) {
            return false;
        }

        bool shouldDeleteNode = removeWord(node->symbol[charIndex], word, index + 1);

        if (shouldDeleteNode) {
            delete node->symbol[charIndex];
            node->symbol[charIndex] = nullptr;
            return isEmpty(node);
        }

        return false;
    }

    bool isEmpty(TrieNode* node) {
        for (TrieNode* child : node->symbol) {
            if (child) {
                return false;
            }
        }
        return true;
    }
};


const int TABLE_SIZE = 100;

class HashTable {
private:
    struct Entry {
        string key;
        int value;
        bool deleted;

        Entry() : deleted(false) {}
    };

    vector<Entry> table;

    int hash(const string& key) {
        int hash = 0;
        for (char c : key) {
            hash += c;
        }
        return hash % TABLE_SIZE;
    }

    int quadraticProbe(int index, int i) {
        return (index + i * i) % TABLE_SIZE;
    }

public:
    HashTable() : table(TABLE_SIZE) {}

    void insert(const string& key, int value) {
        int index = hash(key);

        for (int i = 0; i < TABLE_SIZE; i++) {
            int probeIndex = quadraticProbe(index, i);

            if (table[probeIndex].key.empty() || table[probeIndex].deleted) {
                table[probeIndex].key = key;
                table[probeIndex].value = value;
                table[probeIndex].deleted = false;
                return;
            }
        }

    }

    int get(const string& key) {
        int index = hash(key);

        for (int i = 0; i < TABLE_SIZE; i++) {
            int probeIndex = quadraticProbe(index, i);

            if (table[probeIndex].key == key && !table[probeIndex].deleted) {
                return table[probeIndex].value;
            }
        }

        return -1;
    }

    void remove(const string& key) {
        int index = hash(key);

        for (int i = 0; i < TABLE_SIZE; i++) {
            int probeIndex = quadraticProbe(index, i);

            if (table[probeIndex].key == key) {
                table[probeIndex].deleted = true;
                return;
            }
        }
    }
};


int main() {

    Trie trie;
    HashTable hashTable;

    // Добавление 100 элементов
    auto start = high_resolution_clock::now();
    for (int i = 0; i < 100; i++) {
        string key = "key" + to_string(i);

        bool valid = true;
        for (char c : key) {
            if (c < 'a' || c > 'z') {
                valid = false;
                break;
            }
        }

        if (valid) {
            trie.insert(key);
            hashTable.insert(key, i);
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time for inserting 100 elements: " << duration.count() << " mcs" << endl;

    // Поиск 100 элементов
    start = high_resolution_clock::now();
    for (int i = 0; i < 100; i++) {
        string key = "key" + to_string(i);

        bool valid = true;
        for (char c : key) {
            if (c < 'a' || c > 'z') {
                valid = false;
                break;
            }
        }

        if (valid) {
            bool foundInTrie = trie.search(key);
            int foundInHashTable = hashTable.get(key);
        }
    }
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Time for searching 100 elements: " << duration.count() << " mcs" << endl;

    // Удаление 100 элементов
    start = high_resolution_clock::now();
    for (int i = 0; i < 100; i++) {
        string key = "key" + to_string(i);

        bool valid = true;
        for (char c : key) {
            if (c < 'a' || c > 'z') {
                valid = false;
                break;
            }
        }

        if (valid) {
            trie.remove(key);
            hashTable.remove(key);
        }
    }
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    cout << "Time for removing 100 elements: " << duration.count() << " mcs" << endl;

    return 0;
}
