#include <iostream>
#include <string>
#include <unordered_map>
#include <cassert>

using namespace std;

// Trie Node
struct TrieNode {
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false) {}
};

// Trie class
class Trie {
public:
    Trie();
    void insert(const string& word);
    bool search(const string& word);
    void searchAndReplace(string& text, const string& pattern, const string& replacement);

private:
    TrieNode* root;
    void searchUtil(TrieNode* node, const string& word, int index, bool& found);
};

Trie::Trie() {
    root = new TrieNode();
}

void Trie::insert(const string& word) {
    TrieNode* current = root;
    for (char c : word) {
        if (current->children.find(c) == current->children.end()) {
            current->children[c] = new TrieNode();
        }
        current = current->children[c];
    }
    current->isEndOfWord = true;
}

bool Trie::search(const string& word) {
    TrieNode* current = root;
    for (char c : word) {
        if (current->children.find(c) == current->children.end()) {
            return false;
        }
        current = current->children[c];
    }
    return current != nullptr && current->isEndOfWord;
}

void Trie::searchUtil(TrieNode* node, const string& word, int index, bool& found) {
    if (node == nullptr || found) {
        return;
    }
    if (index == word.length()) {
        found = node->isEndOfWord;
        return;
    }
    char c = word[index];
    if (c == '.') { // Wildcard character
        for (auto& child : node->children) {
            searchUtil(child.second, word, index + 1, found);
        }
    } else {
        if (node->children.find(c) != node->children.end()) {
            searchUtil(node->children[c], word, index + 1, found);
        }
    }
}

void Trie::searchAndReplace(string& text, const string& pattern, const string& replacement) {
    size_t pos = 0;
    while ((pos = text.find(pattern, pos)) != string::npos) {
        text.replace(pos, pattern.length(), replacement);
        pos += replacement.length();
    }
}

class TextEditor {
private:
    string text;
    size_t cursorPosition;

public:
    TextEditor();
    void insert(char ch);
    void deleteChar();
    void display();
    void moveCursorLeft();
    void moveCursorRight();
    void performCommand(const string& command);
};

TextEditor::TextEditor() : cursorPosition(0) {}

void TextEditor::insert(char ch) {
    text.insert(cursorPosition, 1, ch);
    ++cursorPosition;
}

void TextEditor::deleteChar() {
    if (cursorPosition > 0) {
        text.erase(cursorPosition - 1, 1);
        --cursorPosition;
    }
}

void TextEditor::display() {
    cout << "Text: " << text << endl;
    cout << "Cursor Position: " << cursorPosition << endl;
}

void TextEditor::moveCursorLeft() {
    if (cursorPosition > 0) {
        --cursorPosition;
    }
}

void TextEditor::moveCursorRight() {
    if (cursorPosition < text.length()) {
        ++cursorPosition;
    }
}

void TextEditor::performCommand(const string& command) {
    if (command.empty()) {
        return;
    }

    char cmd = command[0];
    switch (cmd) {
        case 'i': {
            if (command.length() > 2) {
                char ch = command[2];
                insert(ch);
            }
            break;
        }
        case 'd':
            deleteChar();
            break;
        case 'l':
            moveCursorLeft();
            break;
        case 'r':
            moveCursorRight();
            break;
        case 's': {
            size_t spacePos = command.find(' ');
            if (spacePos != string::npos && command.length() > spacePos + 1) {
                size_t spacePos2 = command.find(' ', spacePos + 1);
                if (spacePos2 != string::npos && command.length() > spacePos2 + 1) {
                    string pattern = command.substr(spacePos + 1, spacePos2 - spacePos - 1);
                    string replacement = command.substr(spacePos2 + 1);
                    Trie trie;
                    trie.searchAndReplace(text, pattern, replacement);
                }
            }
            break;
        }
        default:
            cout << "Invalid command. Please try again." << endl;
            break;
    }
}

int main() {
    TextEditor editor;

    cout << "Welcome to the Text Editor!" << endl;
    cout << "Commands:" << endl;
    cout << "- i <char>: Insert character" << endl;
    cout << "- d: Delete character at cursor position" << endl;
    cout << "- l: Move cursor left" << endl;
    cout << "- r: Move cursor right" << endl;
    cout << "- s <pattern> <replacement>: Search and replace" << endl;
    cout << "- q: Quit" << endl;

    string command;
    bool quit = false;

    while (!quit) {
        cout << "Enter command: ";
        getline(cin, command);

        if (command.empty()) {
            continue;
        }

        if (command[0] == 'q') {
            quit = true;
        } else {
            editor.performCommand(command);
            editor.display();
        }
    }

    cout << "Exiting Text Editor. Goodbye!" << endl;

    return 0;
}
