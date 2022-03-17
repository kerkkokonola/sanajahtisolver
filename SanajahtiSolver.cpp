// SanajahtiSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

vector<char> ruudukko;
map<int, bool> used;
set<string> found;
map<string, vector<int>> reitit;

vector<int> posmoves(int ruutu) {
	vector<vector<int>> ans = { {1, 4, 5}, {0, 2, 4, 5, 6}, {1, 3, 5, 6, 7}, {2, 6, 7}, {0, 1, 5, 8, 9}, {0, 1, 2, 4, 6, 8, 9, 10}, {1, 2, 3, 5, 7, 9, 10, 11}, {2, 3, 6, 10, 11}, {4, 5, 9, 12, 13}, {4, 5, 6, 8, 10, 12, 13, 14}, {5, 6, 7, 9, 11, 13, 14, 15}, {6, 7, 10, 14, 15}, {8, 9, 13}, {8, 9, 10, 12, 14}, { 9, 10, 11, 13, 15 }, {10, 11, 14} };
	return ans[ruutu]; //mahdolliset liikkeet ruudusta n, hardcodettu
}

vector<int> etsiAlut(string toFind) { //palauttaa sanojen alkukirjainten paikat listassa
	vector<int> starts;
	for (int l = 0; l < 16; l++) {
		if (ruudukko[l] == toFind[0]) {
			starts.push_back(l);
		}
	}
	return starts; 
}

bool dfs(string word, int index, int ruutu, vector<int> reitti) {
	for (auto i : posmoves(ruutu)) { //kaydaan kaikki mahdolliset lailliset siirrot lapi
		if (ruudukko[i] == word[index] && used[i] == 0) { //Oikea kirjain ja non-used ruutu
			if (index + 1 == word.length()) { //sanan vika kirjain loyty
				reitti.push_back(i);
				found.insert(word);
				reitit[word] = reitti;
				reitti.pop_back();
				return true; //Flagataan loydetyksi
			} else {
				used[i] = true; //Kaytetaan ruutu
				reitti.push_back(i);
				dfs(word, index + 1, i, reitti);
				reitti.pop_back();
				used[i] = false;
			}
		}
	}
	return false;
}

bool pituusSort(string eka, string toka) {
	return eka.length() > toka.length();
}

int main()
{
	//alustetaan sanalista
	vector<string> sanat = {};
	ifstream infile("sanat.txt");
	string sana;
	while (infile >> sana) {
		sanat.push_back(sana);
	}
	infile.close();
	cout << "SANAT ALUSTETTU JA NIITÄ OLI " << sanat.size() << " KPL" << endl;
	//ruotsinoo on 2, a pisteilla on 1 ja o pisteilla on 0
	//sanojen alustus valmis


	//INIT used-lista falseksi
	for (int i = 0; i < 16; i++) {
		used[i] = false;
	}
	//INIT used loppuu



	//INIT ruudukko
	while (true) {
		string ruudut;
		cin >> ruudut;
		for (int p = 0; p < ruudut.length(); p++) {
			ruudut[p] = tolower(ruudut[p]);
		}
		for (int i = 0; i < 16; i++) { //suomalaisten aakkosten kasittely
			if (ruudut[i] == '†') {
				ruudukko.push_back('2');
			}
			else if (ruudut[i] == '„') {
				ruudukko.push_back('1');
			}
			else if (ruudut[i] == '”') {
				ruudukko.push_back('0');
			}
			else {
				ruudukko.push_back(ruudut[i]);
			}
		}
		//INIT loppuu

		//Etsitään alut
		for (auto i : sanat) {
			for (auto k : etsiAlut(i)) {
				used[k] = true;
				dfs(i, 1, k, {k});
				used[k] = false;
			}
		}
		//syvyyshaku loppuu

		//Tulostetaan loydetyt sanat pituusjarjestyksessa
		vector<string> tulostukset;
		for (auto k : found) {
			tulostukset.push_back(k);
		}
		sort(tulostukset.begin(), tulostukset.end(), pituusSort);

		for (auto s : tulostukset) {
			for (auto k : s) {
				if (k != '0' && k != '1' && k != '2') {
					cout << k;
				}
				if (k == '0') {
					cout << "”";
				}
				if (k == '1') {
					cout << "„";
				}
				if (k == '2') {
					cout << "†";
				}
			}
			cout << "   ";
			for (auto i : reitit[s]) { //tulostetaan reitti
				cout << i << " ";
			}
			cout << endl;
		}

		//alustetaan uuden ruudukon syotto
		for (int i = 0; i < 16; i++) {
			used[i] = false;
		}
		
		found.clear();
		ruudukko = {};
		for (int i = 0; i < 5; i++) {
			cout << endl;
		}
	}

	//Valmis
	int odotus;
	cin >> odotus;
	return 0;
}