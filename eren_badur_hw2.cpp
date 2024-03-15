// Eren Badur

#include <vector>
#include <fstream>
#include "AVLSearchTree.h"
#include <string>
using namespace std;


struct DocumentItem {
    string documentName;
    int count;
    DocumentItem(const string & name, int cnt) {
        documentName = name;
        count = cnt;
    }
};

struct WordItem {
    string word;
    AVLSearchTree<string, DocumentItem*>* documentTree;

    explicit WordItem(const string & w) {
        word = w;
        documentTree = nullptr;
    }

    void updateCount(const string & documentName, int increment) {
        if (!documentTree) {
            documentTree = new AVLSearchTree<string, DocumentItem*>();
        }

        DocumentItem** docItemPtr = documentTree->find(documentName);
        if (docItemPtr) {
            (*docItemPtr)->count += increment;
        } else {
            DocumentItem* newDocItem = new DocumentItem(documentName, increment);
            documentTree->insert(documentName, newDocItem);
        }
    }

    int countInDoc(const string& documentName) {
        if (documentTree != nullptr) {
            DocumentItem** docItemPtr = documentTree->find(documentName);
            if (docItemPtr && *docItemPtr != nullptr) {
                return (*docItemPtr)->count;
            }
        }
        return 0;
    }
};

// Checks if the count is valid
bool countIsValid(string str) {
    if(str.find_first_not_of("123456") != string::npos) {
        return false;
    }
    return true;
}

// Checks if the filename is valid
bool nameIsValid(string filename) {
    string validNames[] = {"a.txt", "b.txt", "c.txt", "d.txt", "e.txt", "f.txt"};
    for (int i = 0; i < 6; i++) {
        if (filename == validNames[i]) {
            return true;
        }
    }
    return false;
}

// Converts a string to all lowercase letters
string toLower(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] += 'a' - 'A';
        }
    }
    return str;
}


vector<string> splitWords(const string& str) {
    vector<string> words;
    string word;
    for (char ch : str) {
        if (ch == ' ' || ch == '\t' || ch == '\n') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        else {
            word += ch;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }
    return words;
}

void printWordItem(const WordItem* wordItem) {
    if (wordItem != nullptr) {
        cout << "Word: " << wordItem->word << endl;
        if (wordItem->documentTree != nullptr) {
            wordItem->documentTree->print();
        } else {
            cout << "No documents associated with this word." << endl;
        }
    }
}


int main() {
    AVLSearchTree<string, WordItem*> myTree;

    while(true) {
        string countInput;
        cout << "Enter number of input files: ";
        cin >> countInput;
        cin.ignore();

        if(countInput == "ENDOFINPUT") {
            break;
        }

        while(!countIsValid(countInput)) {
            cout << "Enter number of input files: ";
            cerr << "Please enter a valid input (Between 1 and 6)." << endl;
            cin >> countInput;
            cin.ignore();
        }

        int fileCount;
        fileCount = stoi(countInput);

        vector<string> files(fileCount);
        for (int i = 0; i < fileCount; i++) {
            cout << "Enter " << (i + 1) << ". file name: ";
            getline(cin, files[i]);

            while (!nameIsValid(files[i])) {

                cout << "Invalid file name. Please try again: ";
                getline(cin, files[i]);
            }
        }

        string search;
        cout << "Enter queried words in one line: ";
        getline(cin, search);

        if(search == "ENDOFINPUT") {
            break;
        }


        if(search.substr(0, 6) == "REMOVE") {
            string searchSubstr = search.substr(7);

            myTree.remove(searchSubstr);
            cout << searchSubstr << " has been REMOVED" << endl;

            cout << endl;
            cout << "Enter queried words in one line: ";
            getline(cin, search);

            if(search == "ENDOFINPUT") {
                break;
            }
        }

        vector<string> searchWords = splitWords(toLower(search));

        for (int i = 0; i < files.size(); i++) {
            string file = files[i];
            ifstream inputFile(file);
            if (inputFile) {
                string line;
                while (getline(inputFile, line)) {
                    vector<string> wordsInLine = splitWords(toLower(line));
                    for (int j = 0; j < wordsInLine.size(); j++) {
                        string word = wordsInLine[j];
                        WordItem** temp = myTree.find(word);
                        WordItem* wordItem = nullptr;
                        if (temp != nullptr) {
                            wordItem = *temp;
                        }

                        if (wordItem == nullptr) {
                            wordItem = new WordItem(word);
                            myTree.insert(word, wordItem);
                        }
                        if (wordItem != nullptr) {
                            wordItem->updateCount(file, 1);
                        }
                    }
                }
                inputFile.close();
            } else {
                cerr << "Unable to open file " << file << endl;
            }
        }

        bool anyDocumentContainsAll = false;

        for (int i = 0; i < files.size(); i++) {
            string file = files[i];
            bool allWordsFound = true;
            for (int j = 0; j < searchWords.size(); j++) {
                string word = searchWords[j];
                WordItem** temp = myTree.find(word);
                WordItem* wordItem = nullptr;
                if (temp != nullptr) {
                    wordItem = *temp;
                }

                if (wordItem == nullptr || wordItem->countInDoc(file) == 0) {
                    allWordsFound = false;
                    break;
                }
            }

            if (allWordsFound) {
                anyDocumentContainsAll = true;
                cout << "In Document " << file;
                for (int j = 0; j < searchWords.size(); j++) {
                    string word = searchWords[j];
                    WordItem* wordItem = *(myTree.find(word));
                    int count = 0;
                    if (wordItem != nullptr) {
                        count = wordItem->countInDoc(file);
                    }

                    cout << ", " << word << " found " << count << " times";
                }
                cout << "." << endl;
            }
        }

        if (!anyDocumentContainsAll) {
            cout << "No document contains the given query" << endl;
        }

        cout << endl;

        myTree.clear(); // The tree is cleared at the end of each iteration

    }

    return 0;

}