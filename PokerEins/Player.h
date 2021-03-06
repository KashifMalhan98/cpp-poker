
#pragma once


#include <string>
#include <vector>

#include "Stack.h" 
#include "SingleBet.h"
#include "Actions.h"
#include "Card.h" 

using namespace std; 

class Table;

enum playerStatus { PS_ACTIVE = 0, 
	                PS_FOLDED,
			        PS_ALL_IN
		          };


/*
  represents a participant (player) for a game.
  there can be different implementations for players,
  computer and real players. or different implementations for computer players,
  playing there own algorithm.
*/ 
class Player {

private: 
	std::string playerID; 

	// the internal current amount of coins the player has in reserve to play 
	Stack coinStack; 

	// a copy of the value at the beginning of this single Game 
	Stack roundStartStack; 

	// his/its private cards, may be null if there is no game actvie or no card yet 
	// PocketCards * pocketCards; 
	vector<Card *> pocketCards; // should be 0, 3, 4 or 5 ; encapsulate in class if required 

	SingleBet * singleBet;  // current bet per round (stake, wager)

	// only for test !!! 
	bool doRase ; 

protected: 
	playerStatus gameStatus; 


private: 
	Player() {
		doRase = false; 
		singleBet = NULL; 
	}

public:

	Player(string playerIdIn) : gameStatus(PS_ACTIVE) {
		playerID = playerIdIn; 
		doRase = false; 
		singleBet = NULL; 
		
		coinStack.addCoins(200); 
	}

	playerStatus getPlayerStatus() {
		return this->gameStatus; 
	}

	vector<Card *> getPocketCards() {
		return this->pocketCards; 
	}


	void setRaise() {
		doRase = true; 
	}


	virtual ~Player(void) {

		if(singleBet!=NULL) {
			delete singleBet; 
		}

	}

	string getPlayerId() {
		return playerID;
	}

	// TODO return your current action ! 
	// IF call then add difference to your single Bet pot
	// IF you RAISE then add call diference AND addtional RAISE amount to singleBet amount 
	// return only your action, table will check your single-Bet Pot automatically 
	
	virtual Actions playActionStep( Table * table ) = 0 ; 


	// get a card from dealer during deal
	void receiveCard( Card * card ); 

	// Table dealer will call restet bet pot bevore each bet-round
	// TABLE dealer MUST delete the Single bet when not needed anymore 
	SingleBet * resetSingleBet() {
		SingleBet * tmp = this->singleBet;
		this->singleBet = new SingleBet(); 
		return tmp; 
	}

	SingleBet * getSingleBetPot () {
		return singleBet; 
	}


	// table will call this for the small and big blind players, 
	// providing the correct values already ! 
	void makeBlind(unsigned blindValue ) {

		betCoins(blindValue); 

	}

protected:

	void betCoins(unsigned coins ) {
		
		if(coins > coinStack.getCoins() ) {
			throw "Invalid Amount of coins to bet" ; 
		}

		this->coinStack.removeCoins(coins);
		this->singleBet->addCoins(coins); 
	}


};

