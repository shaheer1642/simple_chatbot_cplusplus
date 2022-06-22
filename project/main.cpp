/* 
IMPORTANT NOTE: 

	Add the following line to Tools > Compiler Options > General:
		-std=c++11 
		
	or else the program will not compile
*/

#include <iostream>         // includring for cout/cin
#include <fstream>          // including for file handling
#include <iomanip>          // including for setw() function
#include <conio.h>          // including for getch() function
#include <regex>
#include "lib/json.hpp"     // including for JSON data structure support

using namespace std;
using json = nlohmann::json;        // defining json as data type

string strUpper(string str) {       // this function converts a given string to upper case
    string upper;
    // loop through each character and convert to upper case using toupper() built-in function
    for (int x=0; x<str.length(); x++) {
        upper += toupper(str[x]);
    }
    
    return upper;
}
string strLower(string str) {       // this function converts a given string to lower case
    string upper;
    // loop through each character and convert to lower case using tolower() built-in function
    for (int x=0; x<str.length(); x++) {
        upper += tolower(str[x]);
    }
    
    return upper;
}
string replaceSpecialChars(string str) {
    str = regex_replace(str, regex("\\?"), "");
    str = regex_replace(str, regex("\\."), "");
    str = regex_replace(str, regex("\\!"), "");
    str = regex_replace(str, regex("\\'"), "");
    return str;
}

// the Currency class that contains data about each currency and some methods
class Bot {
	private:
    	json botData;      // contains data for upto 150 currency rates
    public:
        string name;    // bot's name

    public: 
	    Bot() {        // the constructor method to iniatialize class variables
            // assign random name to the bot
            string namesArr[] = {"AlbertOne","A.L.I.C.E.","Charlix","Cleverbot","EugeneGoostman","Fred","Jabberwacky","JeeneyAI","Lenny","MegaHAL","SimSimi","Spookitalk","UltraHal","Verbot"};
            srand(time(NULL));
            name = namesArr[rand() % 13];

            // get file contents
	        string fileContent;
	        string line;
	        ifstream readFile("data/qna.json");     // this file contains json formatted data about currency rates
            // read each line and append to fileContent variable
	        while (getline (readFile, line)) {     // read the file line by line and append to fileContent var
	            fileContent += line;
	        }
            // parsing/converting the read string into json data structure
	        botData = json::parse(fileContent);
            return;
	    }

        float calcSimilarity(string question, string input) {
            // Used to split string around spaces.
            istringstream quesStream(question);
            istringstream inputStream(input);
        
            string word1; // for storing each word
            string word2; // for storing each word
        
            // Traverse through all words
            // while loop till we get
            // strings to store in string word
            int matches = 0;
            float total = 0;
            while (inputStream >> word1)
            {
                while (quesStream >> word2) {
                    if (word1 == word2) {
                        matches++;
                        break;
                    }
                }
                total++;
            }
            return (matches/total)*100;
        }

        string getAnswer(string input) {
            input = replaceSpecialChars(strLower(input));
            json answers;
            json best_answers;
            float highest_similarity = 0;
            for (int i=0; i<this->botData.size(); i++) {
                string question = replaceSpecialChars(strLower(this->botData[i]["question"]));
                float similarity = calcSimilarity(question,input);
                if (similarity > 50) {
                    answers.push_back(this->name + ": " + to_string(this->botData[i]["answer"]));
                    if (similarity >= highest_similarity) {
                        highest_similarity = similarity;
                        best_answers.push_back(this->name + ": " + to_string(this->botData[i]["answer"]));
                    }
                }
            }
            cout<<"hits: "<<answers.size()<<endl;
            cout<<"best hits: "<<best_answers.size()<<endl;
            cout<<"highest similarity: "<<highest_similarity<<endl;

            if (best_answers.size() > 0) {
                srand(time(NULL));
                return best_answers[rand() % best_answers.size()];
            } else {
                return (this->name + ": Sorry, I did not understand.");
            }
        }
};
Bot* bot = new Bot();

int initializeConversation() {
    cout<<endl<<"You: ";
    string input;
    getline(cin, input);
    if (strLower(input) == "bye") {
        cout<<bot->name<<": It was fun talking. Good bye!";
        return 0;
    } else {
        cout<<bot->getAnswer(input)<<endl;
        initializeConversation();       // get input recursively until user types 'bye'
    }
}

int printWelcomeScreen() {
    cout<<"Welcome to Simple Chatbot"<<endl;
    cout<<"-------------------------\nType 'bye' to exit program.\n\n";
    cout<<bot->name<<": "<<"Hey there, how may I help you?"<<endl;

    initializeConversation();
    return 0;
}

int main() {
    printWelcomeScreen();    // call main menu until it returns 0
    delete bot;
    return 0;
}
