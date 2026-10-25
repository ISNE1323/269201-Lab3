#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include "tree.h"
using namespace std;


class WordEntry {
public:
	WordEntry() {}
	WordEntry(const string& w, int line = 0) : word(w) {
		if (line > 0)
			lines.push_back(line);
	}

	void addLine(int line) {
		if (lines.empty() || lines.back() != line)
			lines.push_back(line);
	}

	bool operator<(const WordEntry& other) const { return word < other.word; }
	bool operator==(const WordEntry& other) const { return word == other.word; }

	string word;
	vector<int> lines;
};

ostream& operator<<(ostream& out, const WordEntry& e) {
	out << e.word << " : ";
	for (size_t i = 0; i < e.lines.size(); i++) {
		if (i > 0)
			out << ", ";
		out << e.lines[i];
	}
	return out;
}

void addWord(string word, int lineNo, Tree<WordEntry>& tree) {
	// strip apostrophes at both ends (e.g. 'hello' -> hello, but don't stays don't)
	while (!word.empty() && word[0] == '\'')
		word.erase(0, 1);
	while (!word.empty() && word[word.size() - 1] == '\'')
		word.erase(word.size() - 1);
	if (word.empty())
		return;

	WordEntry entry(word, lineNo);
	WordEntry* found = tree.search(entry);
	if (found != 0)
		found->addLine(lineNo);
	else
		tree.insert(entry);
}

void addWordsFromLine(const string& line, int lineNo, Tree<WordEntry>& tree) {
	string word;
	for (size_t i = 0; i <= line.size(); i++) {
		// at the end of the line, pretend there is a space to flush the last word
		unsigned char c = (i < line.size()) ? (unsigned char)line[i] : ' ';
		if (isalnum(c) || c == '\'') {
			word += (char)tolower(c);
		}
		else if (!word.empty()) {
			addWord(word, lineNo, tree);
			word.clear();
		}
	}
}

int main(int argc, char* argv[])
{
	string filename;
	if (argc > 1) {
		filename = argv[1];
	}
	else {
		cout << "Enter text file name: ";
		getline(cin, filename);
	}

	ifstream fin(filename.c_str());
	if (!fin) {
		cerr << "Cannot open file: " << filename << endl;
		return 1;
	}

	Tree<WordEntry> mytree;

	string line;
	int lineNo = 0;
	while (getline(fin, line)) {
		lineNo++;
		addWordsFromLine(line, lineNo, mytree);
	}
	fin.close();

	mytree.inorder();

	return 0;
}