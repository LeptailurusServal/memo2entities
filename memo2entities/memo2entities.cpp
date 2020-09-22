#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
using std::fixed;
using std::ios;
using std::left;
using std::right;
using std::showpoint;

#include <fstream> // file stream
using std::ifstream; // input file stream
using std::ofstream; // output file stream

#include <iomanip>
using std::setw;
using std::setprecision;

#include <string>
using std::string;

#include <cstdlib>
using std::exit; // exit function prototype

#include <vector> 
#include <algorithm>
using namespace std;

void outputLine(const string, const string); // prototype

string text1 = "{\"archetype\":2,\"data\":{\"index\":0,\"values\":[";
string text2 = "]}}";

int posCheck(string s) {
	//char c[3];
	//strcpy_s(c, s.c_str());
	int v = 0;
	if (s[0] == -30 && s[1] == -111) {
		v = s[2] + 97;

		//v = s[2] + 145;
		//v = (v > 57) ? v + 39 : v;
		return v;
	}
	else return 0;

}

class Note{
	public:
		Note();
		Note(float t, int p);
		float getTime() { return time; };
		int getPos() { return pos; };
		void setTime(float t) { time = t; };
		void setPos(int l) { pos = l; };
		bool operator > (const Note& str) const {
			if (time < str.time) return true;
			else return false;
		}
private:
		float time;
		int pos;
};

Note::Note(float t, int p)
{
	time = t;
	pos = p;
}
Note::Note()
{
	time = 0;
	pos = 0;
}

int main()
{
	string input = "chart.txt";
	string output = "entities.json";
	cout << "Where is your memo file?" << endl;
	cin >> input;
	cout << "Where is the destination of your file?" << endl;
	cin >> output;
	// ifstream constructor opens the file
	ifstream inClientFile(input, ios::in);
	// ofstream constructor opens file
	ofstream outClientFile(output, ios::out);

	// exit program if ifstream could not open file
	if (!inClientFile)
	{
		cerr << "No read" << endl;
		exit(1);
	} // end if

	if (!outClientFile) // overloaded ! operator
	{
		cerr << "No write" << endl;
		exit(1);
	} // end if

	//char time1[15];
	//char time2[15];
	//char note[30];
	string line;
	string test;
	int pos = 0;
	int bpm = 0;
	int startpos = 0;
	vector<string> file;
	//vector<string> file2;
	vector<float> noteTime;
	vector<int> notePos;
	notePos.resize(16);
	vector<int> temp;
	vector<Note> notes;

	cout << "Reading file..." << endl;

	while (getline(inClientFile, line)) {
		file.push_back(line);
	}

	int stoppos = file.size();
	string filepos;

	pos = 0;
	for (auto i = file.begin(); i != file.end(); ++i) {
		if (file.at(pos).find("BPM: ") != string::npos) {
			bpm = stoi(file.at(pos).substr(4));
		}
		if (file.at(pos).length() > 26) {
			file.at(pos).erase(file.at(pos).length() - 1);
			file.at(pos).erase(12, 2);
		}
		//cout << file.at(pos) << endl;

		pos++;
	}

	cout << "Reading completed!" << endl;

	float secTime = 240.0 / (float)bpm;

	int secpos = 0;
	bool stop = 0;
	pos = 0;
	int c = 0;
	int timeSize = 0;

	cout << "Processing notes..." << endl;

	for (auto i = file.begin(); i != file.end() && !(stop); ++i) {

		//cout << secpos + 1 << endl;
		filepos = to_string(secpos + 1);

		auto spos = find(file.begin(), file.end(), filepos);
		if (spos != file.end()) {
			startpos = distance(file.begin(), spos) + 1;
		}

		filepos = to_string(secpos + 2);

		spos = find(file.begin(), file.end(), filepos);
		if (spos != file.end()) {
			stoppos = distance(file.begin(), spos);
		}
		else {
			stoppos = file.size();
			stop = true;
		}

		for (int i = startpos; i < stoppos; i++) {
			//timeSize = (file.at(i).size() / 3) - 4;
			for (int j = 0; j < file.at(i).size()/3; j++) {
				if (j < 4) notePos.push_back(posCheck(file.at(i).substr(3 * j, 3)));
				else temp.push_back(posCheck(file.at(i).substr(3 * j, 3)));
			}
			//cout << file2.at(pos) << endl;
			pos++;
		}

		c = 0;
		for (auto i = temp.begin(); i != temp.end(); ++i) {
			if (temp.at(c) > 0) noteTime.push_back((secTime*(float)secpos) + (15.0*(16.0 / (float)temp.size())*(float)c) / bpm);
			//cout << noteTime.at(c) << " ";
			++c;
		}

		c = 0;
		for (auto i = notePos.begin(); i != notePos.end(); ++i) {
			if (notePos.at(c) > 0) notes.push_back(Note(noteTime.at(notePos.at(c) - 1), (c % 16) + 1));
			++c;
		}
		//cout << endl;

		notePos.clear();
		noteTime.clear();
		temp.clear();
		secpos++;
		//cout << endl;
	}

	sort(notes.begin(), notes.end(), greater<Note>());
	//cout << "Pos Time" << endl;

	c = 0;
	for (auto i = notes.begin(); i != notes.end(); ++i) {
		//cout << notes.at(c).getPos() << " " << notes.at(c).getTime() << endl;
		++c;
	}
	cout << endl;

	cout << "Processing Completed!" << endl;

	outClientFile << '[';
	// display each record in file


	c = 0;
	for (auto i = notes.begin(); i != notes.end(); ++i) {
		outClientFile << text1 << notes.at(c).getPos() << "," << notes.at(c).getTime() << text2;
		++c;
		if (c < notes.size()) outClientFile << "," << endl;
	}
	cout << endl;
	outClientFile << ']';
	cout << "You can now find your files at: " << output << endl;
	return 0; // ifstream destructor closes the file
} // end main

// display single record from file
void outputLine(const string account, const string name)
{
	cout << left << setw(15) << account << setw(15) << name << endl;
} // end function outputLine