#include <iostream>
#include <vector>

int ctanodos = 0;

class nodeTrie {
public:
    bool getEnd() {
        return end;
    };

    void setEnd(bool valuereceived) {
        end = valuereceived;
    };

    char getLetter() {
        return letter;
    };

    nodeTrie(char c, nodeTrie *p) : parent(p), letter(c), end(false) {
        for (int i = 0; i < 26; i++) {
            child[i] = nullptr;
        }
    }

    nodeTrie *getNode(int i) {
        return child[i];
    };

    nodeTrie *getChild(char c) {
        return child[c - 'a'];
    }

    void addChild(nodeTrie *node) {
        child[node->letter - 'a'] = node;
    }

private:
    char letter;
    bool end;
    nodeTrie *parent;
    nodeTrie *child[26];
};

class trie {
public:
    trie() {
        root = new nodeTrie('\0', nullptr);
    }

    void insert(std::string word);

    bool search(std::string word);

    nodeTrie *getRoot() {
        return root;
    };
private:
    nodeTrie *root;
};

void trie::insert(std::string word) {
    nodeTrie *current = root;
    for (int i = 0; i < word.length(); i++) {
        char c = word[i];
        nodeTrie *sub = current->getChild(c);
        if (sub != nullptr) {
            current = sub;
        } else {
            current->addChild(new nodeTrie(c, current));
            current = current->getChild(c);
        }
        if (i == word.length() - 1) {
            current->setEnd(true);
        }
    }
}

bool trie::search(std::string word) {
    nodeTrie *current = root;
    for (int i = 0; i < word.length(); i++) {
        char c = word[i];
        nodeTrie *sub = current->getChild(c);
        if (sub == nullptr) {
            return false;
        }
        current = sub;
    }
    return current->getEnd();
}

void printTriePh(int padre, nodeTrie *node) {
    if (node == nullptr) {
        return;
    }
    ctanodos++;
    int actual;
    actual = ctanodos;
    std::cout << "nodo padre: " << padre << ", nodo actual: " << actual << ",letra actual: " << node->getLetter()
              << std::endl;
    for (int i = 0; i < 26; i++) {
        if (node->getNode(i) != nullptr) {
            printTriePh(actual, node->getNode(i));
        }
    }
}

int main() {
    trie myTrie;
    int n;
    std::string word;
    std::vector<bool> isInTrie;

    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::cin >> word;
        myTrie.insert(word);
    }
    std::cin >> n;
    for (int j = 0; j < n; j++) {
        std::cin >> word;
        isInTrie.push_back(myTrie.search(word));
    }
    printTriePh(0, myTrie.getRoot());
    for (int k = 0; k < isInTrie.size(); k++) {
        std::cout << (isInTrie[k] ? "true\n" : "false\n");
    }
    std::cout.flush();
    return 0;

}
