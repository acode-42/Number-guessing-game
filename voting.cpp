#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <ctime>

using namespace std;

struct Candidate {
    string name;
    int votes;
};

struct Voter {
    string name;
    string voterID;
    bool hasVoted;
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
            voter.hasVoted = false; // Initialize voting status to false
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
        file << voter.name << " " << voter.voterID << " " << voter.hasVoted << endl;
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
        voter.hasVoted = false; // Initialize voting status to false
        voters.push_back(voter);
    }

    saveVoters(voters); // Save voters to file
}

bool isVoterEligible(const string& voterID, const vector<Voter>& voters) {
    for (const auto& voter : voters) {
        if (voter.voterID == voterID) {
            return !voter.hasVoted; // Check if the voter has not voted yet
        }
    }
    return false; // Voter ID not found or already voted
}

void castVote(vector<Candidate>& candidates, vector<Voter>& voters) {
    for (auto& voter : voters) {
        if (voter.hasVoted) continue; // Skip if already voted

        cout << "Voter: " << voter.name << " (ID: " << voter.voterID << ")\n";
        cout << "Candidates:\n";
        for (size_t i = 0; i < candidates.size(); ++i) {
            cout << i + 1 << ". " << candidates[i].name << "\n";
        }

        int choice;
        cout << "Enter the candidate number you want to vote for: ";
        cin >> choice;

        // Validate choice
        if (choice > 0 && choice <= candidates.size()) {
            candidates[choice - 1].votes++;
            voter.hasVoted = true; // Mark voter as having voted
            cout << "Vote cast successfully!\n";
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
    }

    saveVoters(voters); // Save updated voter information
}

void determineWinner(const vector<Candidate>& candidates, vector<Voter>& voters) {
    int maxVotes = -1;
    vector<Candidate> tiedCandidates;

    // Find the maximum votes and the candidates with those votes
    for (const auto& candidate : candidates) {
        if (candidate.votes > maxVotes) {
            maxVotes = candidate.votes;
            tiedCandidates.clear();
            tiedCandidates.push_back(candidate);
        } else if (candidate.votes == maxVotes) {
            tiedCandidates.push_back(candidate);
        }
    }

    // If there are ties, conduct a tiebreaker
    if (tiedCandidates.size() > 1) {
        cout << "It's a tie between the following candidates:\n";
        for (const auto& candidate : tiedCandidates) {
            cout << candidate.name << " with " << maxVotes << " votes.\n";
        }

        cout << "Conducting a tiebreaker voting...\n";
        vector<Candidate> tiebreakers = tiedCandidates; // Copy tied candidates for tiebreaker
        for (const auto& voter : voters) {
            cout << "Voter: " << voter.name << " (ID: " << voter.voterID << ")\n";
            cout << "Tied Candidates:\n";
            for (size_t i = 0; i < tiebreakers.size(); ++i) {
                cout << i + 1 << ". " << tiebreakers[i].name << "\n";
            }

            int choice;
            cout << "Enter the candidate number you want to vote for: ";
            cin >> choice;
            if (choice > 0 && choice <= tiebreakers.size()) {
                tiebreakers[choice - 1].votes++; // Increment votes for the selected tiebreaker candidate
            } else {
                cout << "Invalid choice.\n";
            }
        }

        // Determine the final winner from the tiebreaker
        maxVotes = -1;
        string finalWinner;

        for (const auto& candidate : tiebreakers) {
            if (candidate.votes > maxVotes) {
                maxVotes = candidate.votes;
                finalWinner = candidate.name;
            } else if (candidate.votes == maxVotes) {
                // Introduce random selection for persistent ties
                if (rand() % 2 == 0) { // Randomly choose between tied candidates
                    finalWinner = candidate.name;
                }
            }
        }

        cout << "The winner after the tiebreaker is: " << finalWinner << " with " << maxVotes << " votes.\n";
    } else {
        cout << "The winner is: " << tiedCandidates[0].name << " with " << maxVotes << " votes.\n";
    }

    // Display detailed results
    cout << "\nVoting Results:\n";
    for (const auto& candidate : candidates) {
        cout << candidate.name << ": " << candidate.votes << " votes\n";
    }
}

void displayMenu() {
    cout << "Menu:\n";
    cout << "1. Add Candidates\n";
    cout << "2. Add Voters\n";
    cout << "3. Cast Votes\n";
    cout << "4. Determine Winner\n";
    cout << "5. Exit\n";
}

int main() {
    vector<Candidate> candidates;
    vector<Voter> voters;

    // Load existing candidates and voters, if available
    loadCandidates(candidates);
    loadVoters(voters);

    srand(static_cast<unsigned int>(time(0))); // Seed random number generator

    while (true) {
        displayMenu();
        int choice;
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                inputCandidates(candidates);
                break;
            case 2:
                inputVoters(voters);
                break;
            case 3:
                castVote(candidates, voters);
                break;
            case 4:
                determineWinner(candidates, voters);
                break;
            case 5:
                cout << "Exiting the program.\n";
                return 0;
            default:
                cout << "Invalid option. Please try again.\n";
                break;
        }
    }

    return 0;
}
