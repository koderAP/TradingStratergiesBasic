#include <string>
#include <vector>
#include <fstream>
#include <cmath>
using namespace std;

struct Order {
    string date;
    string direction;
    int quantity;
    double price;
};

struct Record {
    int date;
    double price; 
    double high;
    double low;
};

int converet_to_int(string date){
    stringstream ss(date);
    int day, month, year;
    char delimiter;
    ss >> day >> delimiter >> month >> delimiter >> year;

    int startDateInt = year * 10000 + month * 100 + day;

    return startDateInt;
}

string convert_to_date(Record a){
    int day = a.date / 1000000;
    int month = (a.date / 10000) % 100;
    int year = a.date % 10000;

    string date = to_string(day) + "/" + to_string(month) + "/" + to_string(year);
    return date;
}

int pos_start(const vector<Record>& data, int start_date){
    for(int i = 0; i < data.size(); ++i){
        if (data[i].date > start_date){
            return i;
        }
    }
    return -1;
}

vector<Record> readBinary(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Error opening binary file: " << filename << endl;
        exit(EXIT_FAILURE);
    }

    file.seekg(0, ios::end);
    streamsize fileSize = file.tellg();
    file.seekg(0, ios::beg);

    int numRecords = fileSize / sizeof(Record);

    vector<Record> records(numRecords);

    file.read(reinterpret_cast<char*>(records.data()), fileSize);

    file.close();

    return records;
}