#include "uno.h"
#include <iostream>
#include <stack>
#include <algorithm>
#include <random>
#include <stdexcept>

using namespace std;

enum class CardType { NUMBER, SKIP, REVERSE, DRAW_TWO };
enum class Direction { CLOCKWISE, COUNTERCLOCKWISE };

// ----------------------
//  internal struct
// ----------------------
struct InternalCard {
    string color;
    string value;
    CardType type;

    string toString() const { return color + " " + value; }
    bool matches(const InternalCard &other) const {
        return color == other.color || value == other.value;
    }
};

// Each player has a vector of InternalCard
struct Player {
    vector<InternalCard> hand;
};

// ----------------------
// UNOGame Implementation
// ----------------------
class UNOGameImpl {
public:
    int numPlayers;
    vector<Player> players;
    vector<InternalCard> deck;
    stack<InternalCard> discardPile;
    int deckIndex = 0;
    Direction direction = Direction::CLOCKWISE;
    int currentPlayer = 0;
    bool gameOver = false;
    int winner = -1;

    mt19937 rng;

    UNOGameImpl(int n){
    	numPlayers=n;
    	rng=mt19937(1234);
        players.resize(numPlayers);
    }

    void buildDeck() {
        static const vector<string> colors = {"Red", "Green", "Blue", "Yellow"};
        deck.clear();

        for (auto &c : colors) {
            deck.push_back({c, "0", CardType::NUMBER});
            for (int i = 1; i <= 9; ++i) {
                deck.push_back({c, to_string(i), CardType::NUMBER});
                deck.push_back({c, to_string(i), CardType::NUMBER});
            }
            for (int i = 0; i < 2; ++i) {
                deck.push_back({c, "Skip", CardType::SKIP});
                deck.push_back({c, "Reverse", CardType::REVERSE});
                deck.push_back({c, "Draw Two", CardType::DRAW_TWO});
            }
        }
    }

    InternalCard drawCard() {
        if (deckIndex >= (int)deck.size())
            throw runtime_error("Deck empty!");
        return deck[deckIndex++];
    }

    void initialize() {
        buildDeck();
        shuffle(deck.begin(), deck.end(), rng);//shuffle the deck using randomly generated number


        for (int i = 0; i < 7; ++i) {//7 cards per player
            for (int p = 0; p < numPlayers; ++p) {
                players[p].hand.push_back(drawCard());
            }
        }

        discardPile.push(drawCard());//draw 1 card to start game
    }

    void nextPlayer(int skipCount = 1) {
        int dir = (direction == Direction::CLOCKWISE) ? 1 : -1;
        currentPlayer = (currentPlayer + dir * skipCount + numPlayers) % numPlayers;
    }

    bool playCard(Player &p, const InternalCard &top, InternalCard &played) {
        // Priority: color → value → action cards (Skip → Reverse → Draw Two)
        // Color match
        for (size_t i = 0; i < p.hand.size(); ++i) {
            if (p.hand[i].color == top.color) {
                played = p.hand[i];
                p.hand.erase(p.hand.begin() + i);
                return true;
            }
        }
        // Value match
        for (size_t i = 0; i < p.hand.size(); ++i) {
            if (p.hand[i].value == top.value) {
                played = p.hand[i];
                p.hand.erase(p.hand.begin() + i);
                return true;
            }
        }
        // Action match
        vector<string> actions = {"Skip", "Reverse", "Draw Two"};
        for (auto &act : actions) {
            for (size_t i = 0; i < p.hand.size(); ++i) {
                if (p.hand[i].value == act && p.hand[i].matches(top)) {
                    played = p.hand[i];
                    p.hand.erase(p.hand.begin() + i);
                    return true;
                }
            }
        }
        return false;
    }
	//apply action if action card is played
    void applyAction(const InternalCard &played) {
        if (played.type == CardType::SKIP) {
            nextPlayer(2);
        } else if (played.type == CardType::REVERSE) {
            direction = (direction == Direction::CLOCKWISE)
                            ? Direction::COUNTERCLOCKWISE
                            : Direction::CLOCKWISE;
            nextPlayer(1);
        } else if (played.type == CardType::DRAW_TWO) {
            nextPlayer(1);
            if (deckIndex + 2 <= (int)deck.size()) {
                players[currentPlayer].hand.push_back(drawCard());
                players[currentPlayer].hand.push_back(drawCard());
            }
            nextPlayer(1);
        } else {
            nextPlayer(1);
        }
    }

    void playTurn() {
        if (gameOver) return;

        InternalCard top = discardPile.top();
        Player &p = players[currentPlayer];
        InternalCard played;
		//check if current player have any matching card or not
        bool canPlay = playCard(p, top, played);

        if (canPlay) {
            discardPile.push(played);
            // Check for UNO
            if (p.hand.size() == 1) {
                cout << "UNO! Player " << currentPlayer << " has one card left!" << endl;
            }
            // Check for win
            if (p.hand.empty()) {
                gameOver = true;
                winner = currentPlayer;
                return;
            }
            applyAction(played);
        } else {
            // Draw one
            if (deckIndex < (int)deck.size()) {
                InternalCard drawn = drawCard();
                p.hand.push_back(drawn);
                if (drawn.matches(top)) {
                    InternalCard temp;
                    if (playCard(p, top, temp)) {
                        discardPile.push(temp);
                        if (p.hand.empty()) {
                            gameOver = true;
                            winner = currentPlayer;
                            return;
                        }
                        applyAction(temp);
                        return;
                    }
                }
            }
            nextPlayer(1);
        }
    }

    bool isGameOver() const { return gameOver; }

    int getWinner() const { return winner; }
	//print current game state
    string getState() const {
        string s = "Player " + to_string(currentPlayer) + "'s turn, Direction: ";
        s += (direction == Direction::CLOCKWISE) ? "Clockwise" : "Counter-clockwise";
        s += ", Top: " + discardPile.top().toString() + ", Players cards: ";
        for (int i = 0; i < numPlayers; ++i) {
            s += "P" + to_string(i) + ":" + to_string(players[i].hand.size());
            if (i != numPlayers - 1) s += ", ";
        }
        return s;
    }
};


// Use a single static instance pointer internally
static UNOGameImpl *impl = nullptr;

//implement given UNO function by using scope resolution operator
UNOGame::UNOGame(int numPlayers) {
    impl = new UNOGameImpl(numPlayers);
}

void UNOGame::initialize() {
    impl->initialize();
}

void UNOGame::playTurn() {
    impl->playTurn();
}

bool UNOGame::isGameOver() const {
    return impl->isGameOver();
}

int UNOGame::getWinner() const {
    return impl->getWinner();
}

std::string UNOGame::getState() const {
    return impl->getState();
}


int main() {
    UNOGame game(2);
    game.initialize();

    while (!game.isGameOver()) {
        cout << game.getState() << endl;
        game.playTurn();
    }

    cout << "Winner: Player " << game.getWinner() << endl;
    return 0;
}



