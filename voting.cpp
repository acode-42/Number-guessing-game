#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Candidate {
    string name;
    int votes;
};

struct Voter {
    string name;
    string voterID;
};

void loadCandidates(vector<Candidate>& candidates) {
    ifstream file("candidates.txt");
    if (file.is_open()) {
        Candidate candidate;
        while (file >> candidate.name) {
            candidate.votes = 0; // Initialize votes to 0
            candidates.push_back(candidate);
        }
        file.close();
    } else {
        cerr << "No existing candidates file found. Please enter candidate details.\n";
    }
}

void loadVoters(vector<Voter>& voters) {
    ifstream file("voters.txt");
    if (file.is_open()) {
        Voter voter;
        while (file >> voter.name >> voter.voterID) {
            voters.push_back(voter);
        }
        file.close();
    } else {
        cerr << "No existing voters file found. Please enter voter details.\n";
    }
}

void saveCandidates(const vector<Candidate>& candidates) {
    ofstream file("candidates.txt");
    for (const auto& candidate : candidates) {
        file << candidate.name << endl;
    }
    file.close();
}

void saveVoters(const vector<Voter>& voters) {
    ofstream file("voters.txt");
    for (const auto& voter : voters) {
        file << voter.name << " " << voter.voterID << endl;
    }
    file.close();
}

void inputCandidates(vector<Candidate>& candidates) {
    int numCandidates;
    cout << "Enter the number of candidates: ";
    cin >> numCandidates;
    cin.ignore(); // Clear the newline character from the input buffer

    for (int i = 0; i < numCandidates; ++i) {
        Candidate candidate;
        cout << "Enter the name of candidate " << i + 1 << ": ";
        getline(cin, candidate.name);
        candidate.votes = 0; // Initialize votes to 0
        candidates.push_back(candidate);
    }

    saveCandidates(candidates); // Save candidates to file
}

void inputVoters(vector<Voter>& voters) {
    int numVoters;
    cout << "Enter the number of voters: ";
    cin >> numVoters;
    cin.ignore(); // Clear the newline character from the input buffer

    for (int i = 0; i < numVoters; ++i) {
        Voter voter;
        cout << "Enter the name of voter " << i + 1 << ": ";
        getline(cin, voter.name);
        cout << "Enter the voter ID of voter " << i + 1 << ": ";
        getline(cin, voter.voterID);
        voters.push_back(voter);
    }

    saveVoters(voters); // Save voters to file
}

void castVote(vector<Candidate>& candidates, const vector<Voter>& voters) {
    for (const auto& voter : voters) {
        cout << "Voter: " << voter.name << " (ID: " << voter.voterID << ")\n";
        cout << "Candidates:\n";
        for (size_t i = 0; i < candidates.size(); ++i) {
            cout << i + 1 << ". " << candidates[i].name << "\n";
        }

        int choice;
        cout << "Enter the candidate number you want to vote for: ";
        cin >> choice;
        if (choice > 0 && choice <= candidates.size()) {
            candidates[choice - 1].votes++;
        } else {
            cout << "Invalid choice.\n";
        }
    }
}

void determineWinner(const vector<Candidate>& candidates) {
    int maxVotes = -1;
    string winner;

    for (const auto& candidate : candidates) {
        if (candidate.votes > maxVotes) {
            maxVotes = candidate.votes;
            winner = candidate.name;
        }
    }

    cout << "The winner is: " << winner << " with " << maxVotes << " votes.\n";
}

int main() {
    vector<Candidate> candidates;
    vector<Voter> voters;

    // Load existing candidates and voters, if available
    loadCandidates(candidates);
    loadVoters(voters);

    // Input candidates if none are loaded
    if (candidates.empty()) {
        inputCandidates(candidates);
    }

    // Input voters if none are loaded
    if (voters.empty()) {
        inputVoters(voters);
    }

    // Cast votes and determine the winner
    castVote(candidates, voters);
    determineWinner(candidates);

    return 0;
}
