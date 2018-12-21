/**
 * dhkeyex.c by Dylan Manuel
 * Purpse:
 * 	This program simulates a Diffie-Hellman key exchange between
 * 	two parties and outputs various stages of the process to show
 * 	how the public-key exchange works
 * Command Parameters:
 * 	n/a
 * Input:
 * 	n/a (Will generate random prime numbers and simulate the
 * 	exchange using "Alice" and "Bob")
 * Results:
 * 	Prints each stage of the Diffie-Hellman key exchange at a time,
 * 	showing Alice's variables, the public variables, and Bob's
 * 	variables
 * Returns:
 * 	0 - normal
 * 	2 - issue in key exchange
 * Notes:
 * 	When an issue in the public-key exchange is encountered, the
 * 	program terminates
 **/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "dhkeyex.h"

unsigned int genPrime(unsigned int uiMin, unsigned int uiMax){
	unsigned int uiPrime;
	prime:
		uiPrime = (unsigned int)rand() % (uiMax - uiMin) + uiMin;
		//check to see if the number is prime
		if(uiPrime % 2 == 0){
			goto prime;
		}
		unsigned int ui = 3;
		for( ; ui < uiPrime; ui += 2){
			if(uiPrime % ui == 0){
				goto prime;
			}
		}
	return uiPrime;
}

void printStatus(unsigned int uiStage, Participant alice, unsigned long ulSmallPrime,
	       	unsigned long ulLargePrime, Participant bob){
	printf("%-10d Private Key :: %-5d Small Prime :: %-5d Private Key :: %d\n",
		       	uiStage, alice.ulPrivateKey, ulSmallPrime,
			bob.ulPrivateKey);
	printf("\t   Public Key :: %-6d Large Prime :: %-5d Public Key :: %d\n", 
			alice.ulPublicKey, ulLargePrime, bob.ulPublicKey);
	printf("\t   Shared Key :: %-6d\t\t\t     Shared Key :: %ld\n", 
			alice.ulSharedKey, bob.ulSharedKey);
	switch(uiStage){
		case 0:
			printf("\nStage 0 Description :: Alice and Bob agree to use a small prime number of %ld and a large prime number of %ld\n\n", 
					ulSmallPrime, ulLargePrime); break;
		case 1 :
			printf("\nStage 1 Description :: Alice chooses private key %ld and sends Bob her public key %ld (%ld =  %ld pow %ld mod %ld)\n\n",
					 alice.ulPrivateKey, alice.ulPublicKey, alice.ulPublicKey, ulSmallPrime, alice.ulPrivateKey, ulLargePrime); break;
		case 2 :
			printf("\nStage 2 Description :: Bob chooses private key %ld and sends Alice his public key %ld (%ld = %ld pow %ld mod %ld)\n\n", 
					bob.ulPrivateKey, bob.ulPublicKey, bob.ulPublicKey, ulSmallPrime, bob.ulPrivateKey, ulLargePrime); break;
		case 3 :
			printf("\nStage 3 Description :: Alice computes the shared key of %ld (%ld = %ld pow %ld mod %ld)\n\n", 
					 alice.ulSharedKey, alice.ulSharedKey, bob.ulPublicKey, alice.ulPrivateKey, ulLargePrime); break;
		case 4 :
			printf("\nStage 4 Description :: Bob computes the shared key of %ld (%ld = %ld pow %ld mod %ld)\n\n", 
					bob.ulSharedKey, bob.ulSharedKey, alice.ulPublicKey, bob.ulPrivateKey, ulLargePrime); break;
	}
}

unsigned long genPrivateKey(unsigned long ulSmallPrime){
	return (unsigned long)rand() % ulSmallPrime + 1;
}

unsigned long genPublicKey(unsigned long ulPrivateKey, unsigned long ulSmallPrime, unsigned long ulLargePrime){
	return (unsigned long)pow(ulSmallPrime, ulPrivateKey) % ulLargePrime;
}

unsigned long genSharedKey(unsigned long ulPrivateKey, unsigned long ulPublicKey, unsigned long ulLargePrime){
	return (unsigned long)pow(ulPublicKey, ulPrivateKey) % ulLargePrime;
}

int main(int argc, char *argv[]){
	Participant alice = {0, 0, 0};
	Participant bob = {0, 0, 0};
	unsigned int uiStage; //the stage of the key-exchange process
	srand(time(0)); //use current time to generate a seed
	unsigned long ulSmallPrime = genPrime(3, 10); //generate a small prime number
	unsigned long ulLargePrime = genPrime(100, 500); //generate a large prime number
	
	//print a header
	printf("%-10s %-20s %-20s %-20s\n", 
		"Stage", "Alice", "Public", "Bob");
	printStatus(uiStage++, alice, ulSmallPrime, ulLargePrime, bob); //show the prime numbers
	alice.ulPrivateKey = genPrivateKey(ulSmallPrime); //generate Alice's private key
	alice.ulPublicKey = genPublicKey(alice.ulPrivateKey, ulSmallPrime, ulLargePrime); //generate Alice's public key
	printStatus(uiStage++, alice, ulSmallPrime, ulLargePrime, bob); //show Alice's private key and public key
	bob.ulPrivateKey = genPrivateKey(ulSmallPrime); //generate Bob's private key
	bob.ulPublicKey = genPublicKey(bob.ulPrivateKey, ulSmallPrime, ulLargePrime); //generate Bob's public key
	printStatus(uiStage++, alice, ulSmallPrime, ulLargePrime, bob); //show Bob's private key and public key
	alice.ulSharedKey = genSharedKey(alice.ulPrivateKey, bob.ulPublicKey, ulLargePrime); //Alice generates the shared key
	printStatus(uiStage++, alice, ulSmallPrime, ulLargePrime, bob); //show Alice's shared key
	bob.ulSharedKey = genSharedKey(bob.ulPrivateKey, alice.ulPublicKey, ulLargePrime); //Bob generates the shared key
	printStatus(uiStage++, alice, ulSmallPrime, ulLargePrime, bob); //show Bob's shared key
	if(alice.ulSharedKey != bob.ulSharedKey){
		fprintf(stderr, "Error in key exchange: Alice's shared key and Bob's shared key do not match");
		return 2;
	}
	printf("Result :: Alice and Bob have the same shared keys");
	return 0;
}
