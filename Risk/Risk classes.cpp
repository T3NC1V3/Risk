#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream> // Writing map graph and copying to txt file
#include <sstream> // Same as above
#include <unordered_map> // Unordered map for graphs

using namespace std;

class WorldMap {
private:
    unordered_map<string, vector<string>> mapIndex; // Main graph listing continents
    vector<string> players; // Player index
    bool gameOver;
public:
    WorldMap();
    void writeToFile(const string& filename) const; // Function to save the graph to a text file
    void readFile(const string& filename);
    void addContinent(const Continent& continent) { mapIndex[continent.getName()] = continent.getCountries(); } 
    // manually add continents, user defined, will provide functions later
};

class Continent { // Continent is subgraph of the map
private:
    string name;
	vector<string> countries; // subgraph/subset, splits countries from index to continents, treated like a list
	int conValue; // control value
    bool isConquered; // true if one owns all
	
	// Returns control value for continent: const member used since this value will not change over the course of the game
	int getConValue() const { return conValue; }
public:
    Continent(const string& name) : name(name) {}
    // manually add countries, user defined, will provide functions later
    void addCountry(const string& countryName) { countries.push_back(countryName); }
    const vector<string>& getCountries() const { return countries; }
    string getName() const { return name; }
};

class Country : public Continent { // countries are stored within continents; each continent will store several countries
private:
	int troops, ownership; // number of troops in zone and their ownership
	string name; // country name
public:
    // Country properties are defined here as a constructor
    Country(string name, int troops, int owner) : name(name), troops(troops), ownership(owner) {}
};

class Player { // Player Class definition
private:
    int playerNumber, armyBonus; // player number determines turn order
    bool isAttacking, isReinforcing, isFortified, isDefending, isOut;
    int countryCount = mapIndex.size / 4; //amount of countries 
    int armies = 0; // amount of armies
    int tradeins = 0; // no. of trade ins
    char usrYN; // user yes or no input
    int cards = 3;  
    vector<string> ownedCountries;
public:
    string playerName;
    Player(string playerName, int playerNo, int extraArmies) : playerName(playerName), playerNumber(playerNo), armyBonus(extraArmies) {}
    void reinforce() {
        countryCount < 9 ? armies += 3 : armies += countryCount / 3; //amount of armies to be given
        //having issues with code for conqure bonus need to check if a contient is conqured and then give amount base on cont value
        //idk if this would work but
        if (Continent.isConquered == true) {
            armies += Continent(conValue); //have a country value int to call to make it easier 
        }
    //somethibg like this should maybe work
    if (cards > 2) {
        cout << "spend 3 cards to gain armies (y|n)? If you have 5+ cards you must trade in\n";
        cin >> usrYN;
        if (usrYN == 'y' || cards > 4) {
            cards -= 3;
            switch (tradeins) {
            case 0:
                armies += 4;
                tradeins++;
                break;
            case 1:
                armies += 6;
                tradeins++;
                break;
            case 2:
                armies += 8;
                tradeins++;
                break;
            case 3:
                armies += 10;
                tradeins++;
                break;
            case 4:
                armies += 12;
                tradeins++;
                break;
            case 5:
                armies += 15;
                tradeins++;
                break;
            default:
                cout << "no cards were traded in";
            }
        }
    }
    cout << "you have " << armies << " armies.";
    return;
    } // the different phases and turns as needed
    void attacking() {
        int atkRoll, defRoll, attacker, defender; // attacker and defender variables are placeholder
        int army = 4; // placeholder number
        char usrYN = 'y';
        vector<int> redDie;  // dice
        vector<int> whiteDie;
        bool firstWin;
        cout << "attack? (y|n) \n";
        if (usrYN == 'y') {
            army < 4 ? atkRoll = army - 1 : atkRoll = 3;
            cout << "you can roll " << atkRoll << " dice \n";
        }
        //defender turn
        army == 1 ? defRoll = 1 : defRoll = 2;
        cout << "you can roll " << defRoll << " dice \n";
        //attack dice roll
        cout << "attacking roll \n";
        for (int i = 0; i < atkRoll; i++) {
            redDie.emplace_back(rand() % 5);
            cout << redDie.at(i) << "\n";
        }
        sort(redDie.begin(), redDie.end());
        reverse(redDie.begin(), redDie.end());
        //defender dice roll
        cout << "Defending roll \n";
        for (int i = 0; i < defRoll; i++) {
            whiteDie.emplace_back(rand() % 5);
            cout << whiteDie.at(i) << "\n";
        }
        sort(whiteDie.begin(), whiteDie.end());
        reverse(whiteDie.begin(), whiteDie.end());
        //winner determination
        whiteDie.at(0) >= redDie.at(0) ? (attacker - 1) : (defender - 1);
        whiteDie.at(1) >= redDie.at(1) ? (attacker - 1) : (defender - 1);
            if (defender == 0) { return; } // transfer owner ship and use atkroll to get the number of armies that will ocupy the space   
    }
    void fortify() {
        string input;
        cout << "\nPlease choose the country to move armies from: ";
        cin >> input;
        for (int i = 0; i < countryCount; i++) {
            if (ownedCountries[i].find(input, 0) != std::string::npos) // Checks if the country you inputted is in your owned countries index
            {
                cout << ownedCountries[i] << endl;
                Country c1;
            }
            else
            {
                cout << "\nThis country isn't owned by you."; 
            }
        }

        string input2;
        cout << "\nPlease choose the country to move armies to: ";
        cin >> input2;
        for (int i = 0; i < countryCount; i++) {
            if (ownedCountries[i].find(input2, 0) != std::string::npos) // Same as above.
            {
                cout << ownedCountries[i] << endl;
                Country c2;
            }
            else
            {
                cout << "\nThis country isn't owned by you.";
            }
        }

        int input3;
        cout << "How many troops do you want to move: ";
        cin >> input3;

        c1.armies -= input3; // Transfer of soldiers
        c2.armies += input3;

    }
};

WorldMap::WorldMap() {} // parameterless constructor

// Function to save the graph to a text file
void WorldMap::writeToFile(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << "\n";
        return;
    }
    for (const auto& pair : mapIndex) {
        file << "Continent: " << pair.first << "\n";
        const vector<string>& countries = pair.second;
        for (const string& country : countries) {
            file << ", Country: " << country << "\n";
        }
    }
    file.close();
}

// Read file into graph
void WorldMap::readFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << "\n";
        return;
    }
    string line;
    string currentContinent;
    while (getline(file, line)) {
        istringstream iss(line); // input stringstream, read lines for key words
        string token;
        iss >> token;
        if (token == "Continent:") { // if continent is detected, get the ext word and input into graph as string
            iss >> currentContinent;
            addContinent(Continent(currentContinent));
        }
        else if (token == "Country:") { // same as above but with country instead
            string countryName;
            iss >> countryName;
            mapIndex[currentContinent].push_back(countryName);
        }
    }
    file.close();
}

int main() {
    WorldMap world;
    world.readFile("map_data.txt"); // loads txt and reads data
    return 0;
}