/* 
IMPORTANT NOTE: 

	Add the following line to Tools > Compiler Options > General:
		-std=c++11 
		
	or else the program will not compile
*/

#include <iostream>         // including for cout/cin
#include <fstream>          // including for file handling
#include <regex>            // including for regular expressions and string replacements
#include "lib/json.hpp"     // including for JSON data structure support

using namespace std;
using json = nlohmann::json;        // defining json as data type

// this function converts a given string to upper case
string strUpper(string str) {       
    string upper;
    // loop through each character and convert to upper case using toupper() built-in function
    for (int x=0; x<str.length(); x++) {
        upper += toupper(str[x]);
    }
    
    return upper;
}
// this function converts a given string to lower case
string strLower(string str) {       
    string upper;
    // loop through each character and convert to lower case using tolower() built-in function
    for (int x=0; x<str.length(); x++) {
        upper += tolower(str[x]);
    }
    
    return upper;
}
// this function replaces special characters in given string using the regex library
string replaceSpecialChars(string str) {
    // replace special characters, including ? . ! '
    // this is for increasing similarity between question and input
    str = regex_replace(str, regex("\\?"), "");
    str = regex_replace(str, regex("\\."), "");
    str = regex_replace(str, regex("\\!"), "");
    str = regex_replace(str, regex("\\'"), "");
    return str;
}

// the Bot class contains variables and functions for communicating with the bot
class Bot {
	private:
    	json botData;      // contains a series of question/answers in an array
    public:
        string name;    // bot's name

    public: 
	    Bot() {        // the constructor method to iniatialize class variables
            // assign random name to the bot
            string namesArr[] = {"AlbertOne","A.L.I.C.E.","Charlix","Cleverbot","EugeneGoostman","Fred","Jabberwacky","JeeneyAI","Lenny","MegaHAL","SimSimi","Spookitalk","UltraHal","Verbot"};
            srand(time(NULL));      // randomize the seed
            name = namesArr[rand() % 14];       // name is a class variable. rand() % 13 returns an index between 0 and 13

            // get file contents
	        string fileContent;
	        string line;
	        ifstream readFile("data/qna.json");     // this file contains json formatted data for frequent chat conversations
            // read each line and append to fileContent variable
	        while (getline (readFile, line)) {     // read the file line by line and append to fileContent var
	            fileContent += line;
	        }
            // parsing/converting the read string into json data structure
	        botData = json::parse(fileContent);
            return;
	    }

        // this function implements an algorithm for matching 2 strings and return similarity between 0 and 100%
        float calcSimilarity(string question, string input) {   
            // Used to split string by space character and convert into words
            istringstream quesStream(question);     
            istringstream inputStream(input);
            // for storing each word
            string word1; 
            string word2;
        
            int matches = 0;
            float total = 0;
            // loop through all the words in both string and increment matches on match
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

        // this function evaluates an answer for a given question
        string getAnswer(string input) {    
            input = replaceSpecialChars(strLower(input));   // replace any special characters and convert string into lower case
            json answers;
            json best_answers;
            float highest_similarity = 0;
            for (int i=0; i<this->botData.size(); i++) {    // iterate through the array botData
                string question = replaceSpecialChars(strLower(this->botData[i]["question"]));   // replace any special characters and convert string into lower case
                float similarity = calcSimilarity(question,input);      // returns a number between 0 and 100
                if (similarity > 50) {      // if similarity is greater than 50
                    answers.push_back(this->name + ": " + to_string(this->botData[i]["answer"]));   // push to answers
                    if (similarity >= highest_similarity) {     // if similarity is very high then consider it best answer
                        highest_similarity = similarity;
                        best_answers.push_back(this->name + ": " + to_string(this->botData[i]["answer"]));      // push to best_answers
                    }
                }
            }

            if (best_answers.size() > 0) {      // if there is a best answer, return a random best answer
                srand(time(NULL));      // randomize the seed
                return best_answers[rand() % best_answers.size()];       // rand() % best_answers.size() returns an index between 0 and size of best_answers
            } else {     // else return a negative response
                return (this->name + ": Sorry, I did not understand.");
            }
        }
};
Bot* bot = new Bot();       // create a new bot object and store in pointer

// this function gets input from the user and fetches answer from Bot class
int initializeConversation() {      
    cout<<endl<<"You: ";
    string input;
    getline(cin, input);
    if (strLower(input) == "bye" || strLower(input) == "goodbye" || strLower(input) == "good bye") {    // if condition is true, then terminate program
        cout<<bot->name<<": It was fun talking. Good bye!";
        return 0;
    } else {    
        cout<<bot->getAnswer(input)<<endl;  // fetch and print answer from the bot
        initializeConversation();       // get input recursively until user types 'bye'
    }
}
// this function prints Welcome screen and initializes conversation
int printWelcomeScreen() {
    cout<<"Welcome to Simple Chatbot"<<endl;
    cout<<"-------------------------\nType 'bye' to exit program.\n\n";
    cout<<bot->name<<": "<<"Hey there, how may I help you?"<<endl;      // bot->name returns the bot name through the object

    initializeConversation();
    return 0;
}

int main() {
    printWelcomeScreen();    // print welcomeScreen and wait till it returns 0
    delete bot;     // clear bot from memory
    return 0;      // terminate program
}
