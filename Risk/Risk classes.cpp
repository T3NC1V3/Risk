#include <iostream>
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