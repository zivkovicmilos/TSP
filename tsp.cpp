#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <fstream>
#include <time.h>

using namespace std;

struct Elem {
	int city;
	string cityName;
	Elem* next;
	Elem* prev;
	Elem(int c, string n) {
		next = nullptr;
		city = c;
		cityName = n;
	}
};

struct Head {
	Elem* root;
	Elem* last;
	int count;

	Head() {
		root = last = nullptr;
		count = 0;
	};
};

void copy(Head** dest, Head* source) {
	Elem* temp = source->root;

	while (temp) {

		if (!(*dest)->root) {
			(*dest)->root = new Elem(temp->city, temp->cityName);
			(*dest)->root->prev = nullptr;
			(*dest)->last = (*dest)->root;
		}
		else {
			Elem* currTemp = new Elem(temp->city, temp->cityName);
			(*dest)->last->next = currTemp;
			currTemp->prev = (*dest)->last;
			(*dest)->last = currTemp;
		}
		(*dest)->count++;
		temp = temp->next;
	}
}

void deleteTour(Head** h) {
	Elem* temp = (*h)->root;
	while (temp) {
		(*h)->root = (*h)->root->next;
		delete temp;
		temp = (*h)->root;
	}
	(*h)->root = (*h)->last = nullptr;
	(*h)->count = 0;
}

class Graph {
	double** distMatrix;
	int size;
	int minCost;
	int bruteMinCost;

	vector<int> remainingVect;
	Head* bruteSol;

	Head* tour;
	Head* citiesLst;

	vector <string> cities;
public:
	Graph() {
		minCost = 0;
		bruteMinCost = INT_MAX;
		tour = new Head;
		bruteSol = new Head;
		citiesLst = new Head;

		formDistMatrix();
	}
	~Graph() {
		deleteTour(&tour);
		deleteTour(&bruteSol);
		deleteTour(&citiesLst);

		cities.clear();
		remainingVect.clear();

		for (int i = 0; i < size; i++) {
			delete[] distMatrix[i];
		}
		delete[] distMatrix;
	}

	void addToList(Head** h, int city, string name) {
		if (!(*h)->root) {
			(*h)->root = new Elem(city, name);
			(*h)->root->prev = nullptr;
			(*h)->last = (*h)->root;
		}
		else {
			Elem* temp = new Elem(city, name);
			(*h)->last->next = temp;
			temp->prev = (*h)->last;
			(*h)->last = temp;
		}
		(*h)->count++;
	}

	void removeFromList(Head** h, int city) {
		Elem* temp = (*h)->root;
		Elem* last = (*h)->last;
			while (temp && temp->city != city) {
				temp = temp->next;
			}
			if(temp) {
				if (temp == (*h)->root) {
					(*h)->root = temp->next;
					if ((*h)->root) {
						(*h)->root->prev = nullptr;
					}
					delete temp;
				}
				else if (temp == (*h)->last) {
					temp->prev->next = nullptr;
					(*h)->last = temp->prev;
					delete temp;
				} else {
					Elem* prev = temp->prev;
					prev->next = temp->next;
					temp->next->prev = prev;
					delete temp;
				}
				if (!(*h)->root) {
					(*h)->last = nullptr;
				}
				(*h)->count--;
			}
	}

	void printMatrix() const {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				cout << distMatrix[i][j] << " ";
				std::cout.flush();
			}
			cout << endl;
			std::cout.flush();
		}
	}

	void formDistMatrix() {
		SetConsoleOutputCP(CP_UTF8);
		setvbuf(stdout, nullptr, _IOFBF, 1000);

		string fname;
		ifstream file;
		cout << "\nEnter file name: ";
		cin >> fname;
		file.open(fname);

		if (file) {
			cout << "\nFile is open" << endl;
		}
		else {
			cout << "\nFile is not open" << endl;
			exit(1);
		}
		string list;

		getline(file, list);

		string part;
		string dlmtr = ",";
		size_t pos = 0;
		char delim = ',';
		int count = 0;
		// Remove first few csv characters
		pos = list.find(dlmtr);
		part = list.substr(0, pos);
		list.erase(0, pos + dlmtr.length());

		while ((pos = list.find(dlmtr)) != string::npos) {
			part = list.substr(0, pos);
			if (!(part == "")) {
				addToList(&citiesLst, count++, part);
				cities.push_back(part);
			}
			list.erase(0, pos + dlmtr.length());
		}
		// Add last city
		part = list.substr(0, pos);
		addToList(&citiesLst, count++, part);
		cities.push_back(part);

		size = count;
		distMatrix = new double*[size];
		for (int i = 0; i < size; i++) {
			distMatrix[i] = new double[size];
		}

		string str;
		int i = 0;
		int j;
		bool first = true;
		while (!file.eof()) {
			while (i < size) {
				getline(file, str);
				pos = 0;
				j = 0;
				while ((pos = str.find(dlmtr)) != string::npos) {
					part = str.substr(0, pos);
					if (!first) {
						if (part == "") {
							distMatrix[i][j] = 0;
						}
						else {
							distMatrix[i][j] = stod(part);
						}
						first = false;
						j++;
					}
					else {
						first = false;
					}
					str.erase(0, pos + dlmtr.length());
				}
				if (j == size - 1) {
					if (str == "") {
						distMatrix[i][j] = 0;
					}
					else {
						distMatrix[i][j] = stod(str);
					}
				}
				i++;
				first = true;
			}
		}
		file.close();
	}

	bool inTour(Head* h, int city) {
		Elem* temp = h->root;
		while (temp) {
			if (temp->city == city) {
				return true;
			}
			temp = temp->next;
		}
		return false;
	}

	vector <string> getCities() const {
		return cities;
	}

	int findNearest(Head* h, int city) {

		double min;

		for (int i = 0; i < size; i++) {
			if (inTour(h, i) && i != city) {
				min = distMatrix[city][i];
				break;
			}
		}

		int res = -1;
		int currCity;
		
		for (int i = 0; i < size; i++) {

			currCity = i;
			if (inTour(h, currCity) && distMatrix[city][currCity] <= min 
				&& distMatrix[city][currCity]!=0) {
				min = distMatrix[city][currCity];
				res = i;
			}
		}

		return res;
	}

	bool isEmpty(Head* h) {
		if (h->count) {
			return false;
		}
		else {
			return true;
		}
	}

	void printTour(Head* h) {
		Elem* temp = h->root;
		while (temp) {
			if (!(temp == h->last)) {
				cout << temp->cityName << " - ";
			}
			else {
				cout << temp->cityName;
			}
			cout.flush();
			temp = temp->next;
		}
		if (h == tour) {
			cout << "\nLength: " << minCost <<"km" << endl;
		}
		else {
			cout << "\nLength: " << bruteMinCost << "km" << endl;
		}
	}

	void printDB(Head* h) {
		Elem* temp = h->root;
		while (temp) {
			cout << temp->city << ". " << temp->cityName << endl;
			cout.flush();
			temp = temp->next;
		}
	}

	void formTour(int city) {
		vector <string> cities = getCities();
		
		Head* headCopy = new Head;
		minCost = 0;
		copy(&headCopy, citiesLst);

		int startingCity = city;
		int lastAdded = city;
		addToList(&tour, startingCity, cities[startingCity]);
		
		while(!isEmpty(headCopy)) {
			int nearest = findNearest(headCopy, lastAdded);

			addToList(&tour, nearest, cities[nearest]);
			removeFromList(&headCopy, nearest);
			minCost += distMatrix[lastAdded][nearest];

			if (startingCity == lastAdded) {
				removeFromList(&headCopy, startingCity);
			}
			lastAdded = nearest;
			
		}
		addToList(&tour, startingCity, cities[startingCity]);
		minCost += distMatrix[lastAdded][startingCity];
	}

	Head* getCHead() const {
		return citiesLst;
	}

	Head* getTHead() const {
		return tour;
	}

	Head* getBTour() const {
		return bruteSol;
	}

	vector<int> getRTour() const {
		return remainingVect;
	}

	string getName(int num) const {
		return cities[num];
	}

	void bruteCalc(vector<int> bruteTour, vector<int> remaining, 
					double totalCost, int startCity) {

		int lastVisited = bruteTour.back();

		if (totalCost > bruteMinCost) {
			return;
		}

		if (remaining.empty()) {

			bruteTour.push_back(startCity);

			totalCost += distMatrix[lastVisited][startCity];

			if (totalCost <= bruteMinCost) {
				bruteMinCost = totalCost;
				deleteTour(&bruteSol);
				for (int c : bruteTour) {
					addToList(&bruteSol, c, getName(c));
				}
			}
			return;
		}

		for(int nextCity: remaining){

			vector<int> newTour;
			for (int c : bruteTour) {
				newTour.push_back(c);
			}
			newTour.push_back(nextCity);

			double newTotalCost = totalCost + distMatrix[lastVisited][nextCity];

			vector<int> newRemaining;
			for (int c : remaining) {
				newRemaining.push_back(c);
			}
			int pos = 0;

			for (int i = 0; i < newRemaining.size(); i++) {
				if (newRemaining[i] == nextCity) {
					pos = i;
					break;
				}
			}
			newRemaining.erase(newRemaining.begin()+pos);

			bruteCalc(newTour, newRemaining, newTotalCost, startCity);
			newTour.clear();
			newRemaining.clear();
		}
	}
	void clearBMin() {
		bruteMinCost = INT_MAX;
	}

	void clearTMin() {
		minCost = INT_MAX;
	}
};

bool inRange(vector<int> arr, int k) {
	for (int i = 0; i < arr.size(); i++) {
		if (arr[i] == k) {
			return true;
		}
	}
	return false;
}


void menu() {
	cout << endl;
	cout << "\n  * * * * * * * * * * * * * * * * * * *" << endl;
	cout << "  *            Main Menu	      *" << endl;
	cout << "  * * * * * * * * * * * * * * * * * * *" << endl;
	cout << "  *                                   *" << endl;
	cout << "  * 1. Import DB                      *" << endl;
	cout << "  * 2. Print DB                       *" << endl;
	cout << "  * 3. Find Nearest City              *" << endl;
	cout << "  * 4. Form Tour (Nearest Neighbor)   *" << endl;
	cout << "  * 5. Form Tour (All Permutations)   *" << endl;
	cout << "  * 6. Print Tour                     *" << endl;
	cout << "  * 7. Delete Tour                    *" << endl;
	cout << "  * 8. Stop                           *" << endl;
	cout << "  *                                   *" << endl;
	cout << "  * * * * * * * * * * * * * * * * * * *" << endl;
	cout << endl;
}

int main() {
	
	Graph* g = nullptr;
	int odg;
	int del;
	int cnum = 0;
	int cnt = 0;
	int randNum = 0;
	int sc;
	int nrst;

	vector <int> random;

	while (true) {
		menu();
		cout << "> ";
		cin >> odg;
		switch (odg) {
		default:
			g = new Graph;
			break;
		case 2:
			if (g->getCHead()->root) {
				g->printDB(g->getCHead());
			}
			else {
				cout << "\nDB not imported";
			}
			cout.flush();
			break;
		case 3:
			if (g->getCHead()->root) {
				cout << "Enter starting city:\n";
				cout << "> ";
				cin >> sc;
				cout << "Finding nearest city to: " << g->getCities()[sc];
				nrst = g->findNearest(g->getCHead(), sc);
				cout << "\nNearest city is: " << g->getCities()[nrst];
			}
			else {
				cout << "\nDB not imported";
			}
			cout.flush();
			break;
		case 4:
			if (g->getCHead()->root) {
				if (!(g->isEmpty(g->getTHead()))) {
					Head* bt = g->getTHead();
					deleteTour(&bt);
					g->clearTMin();
				}
				cout << "Enter starting city:\n";
				cout << "> ";
				cin >> sc;
				g->formTour(sc);
				cout << "\nTour Formed";
			}
			else {
				cout << "\nDB not imported";
			}
			cout.flush();
			break;
		case 5:
			if (g->getCHead()->root) {
				if (!(g->isEmpty(g->getBTour()))) {
					Head* bt = g->getBTour();
					deleteTour(&bt);
					g->clearBMin();
					random.clear();
				}

				cout << "Enter number of cities (max 10):\n";
				cout << "> ";
				cout.flush();
				cin >> cnum;
				while (cnum < 1 && cnum >10) {
					cout << "\n> ";
					cout.flush();
					cin >> cnum;
				}
				// Adding n random cities
				srand(time(NULL));
				for (int i = 0; i < cnum; i++) {
					randNum = rand() % (g->getCities().size() + 1);
					if (random.empty()) {
						random.push_back(randNum);
						continue;
					}

					while (inRange(random, randNum)) {
						randNum = rand() % (g->getCities().size() + 1);
					}
					random.push_back(randNum);
				}

				Head* bTourHead = g->getBTour();
				vector<int> bVect;
				bVect.push_back(random[0]);

				vector<int> rVect = g->getRTour();
				for (int c : random) {
					rVect.push_back(c);
				}

				rVect.erase(rVect.begin());
				
				g->bruteCalc(bVect, rVect, 0, random[0]);
				g->printTour(bTourHead);
				}
			else {
				cout << "\nDB not imported";
			}
			cout.flush();
			break;
		case 6:
			if (!(g->isEmpty(g->getTHead()))){
				g->printTour(g->getTHead());
			}
			else {
				cout << "\nNo tour formed!";
			}
			cout.flush();
			break;
		case 7:
			if (g->getCHead()->root) {

				if (!(g->isEmpty(g->getBTour())) || !(g->isEmpty(g->getTHead()))) {
					cout << "Select tour to delete: \n";
				
					if (!(g->isEmpty(g->getTHead()))) {
						cout << "1. Nearest Neighbor Tour\n";
					}
					if (!(g->isEmpty(g->getBTour()))) {
						cout << "2. Permutation Tour\n";
					}
					cout << "> ";
					cin >> del;
					Head* dt = g->getCHead();
					del == 1 ? dt = g->getTHead() : dt = g->getBTour();

					deleteTour(&dt);
				}
				else {
					cout << "\nNo tour formed!";
				}
				cout.flush();
			}
			else {
				cout << "\nDB not imported" << endl;
			}
			break;
		case 8:
			return 0;
		}
	}
}