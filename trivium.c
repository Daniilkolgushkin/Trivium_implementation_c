#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>

bool state[2][288];
bool key[2][80];

#define ROUND(i)\
		bool t[3];\
		unsigned int second_index;\
       		for (unsigned char j = 0; j < 3; j ++) {\
			second_index = (j == 0) ? 90 : ((j == 1) ? 174 : 285);\
			t[j] = state[i % 2][(j == 0) ? 65 : (j == 1) ? 161 : 242] ^ state[i % 2][second_index] & state[i % 2][second_index + 1] ^ state[i % 2][second_index + 2] ^ state[i % 2][(170 + j * 93) % 288];\
		}\
		for (unsigned int j = 0; j < 288; j ++) {\
			state[(i + 1) % 2][j] = (j == 0) ? t[2] : ((j == 92) ? t[0] : (j == 176) ? t[1] : state[i % 2][j + 1]);\
		}

#define INIT(key[0], key[1])\
	for (unsigned int i = 0; i < 288; i ++) {\
		state[0][i] = (i < 80) ? key[0][i] : ((i < 93) ? false : ((i < 173) ? key[1][i - 93] : ((i < 285) ? false : true)));\
	}\
	for (unsigned int i = 0; i < 4 * 288; i ++) {\
		ROUND(i);\
	}

#define MAKE_KEY(passphrase)\
	unsigned int k = 0;\
	memset(key[0], '\0', 10);\
	memset(key[1], '\0', 10);\
	while (passphrase[k / 8] != '\0') {\
		key[k / 80][k % 80] ^= (passphrase[(k % 160) / 8] >> (k % 8)) % 2;\
		k ++;\
	}

#define FINISH(exit_status)\
	memset(key[0], '\0', 10);\
	memset(key[1], '\0', 10);\
	memset(state[0], '\0', 36);\
	memset(state[1], '\0', 36);\
	return exit_status;

void encrypt() {
	INIT(key[0], key[1]);
	unsigned char curchar = '\0';
	while (curchar != (unsigned char) EOF) {
		curchar = getchar();
		if (curchar == (unsigned char) EOF) {
			break;
		}
		for (unsigned char i = 0; i < 8 ; i ++) {
			bool z = state[i % 2][65] ^ state[i % 2][92] ^ state[i % 2][161] ^ state[i % 2][176] ^ state[i % 2][242] ^ state[i % 2][287];
			ROUND(i);
			curchar ^= (unsigned char) z << (8 - i);
		}
		putchar(curchar);
	}
}

unsigned int main(unsigned int argc, char *argv[]) {
	char* keyflag[2] = {"-k=", "--key="};
	char* helpflag[2] = {"-h", "--help"};

	for (char i = 1; i < argc; i ++) {
		bool compare = false;
		unsigned char l = 0;
		unsigned int m = 0;
		unsigned int n = 0;
		while (argv[i][m] != '\0') {
			compare = (keyflag[0][l] == argv[i][l]) || (keyflag[1][l] == argv[i][l]);
			l += (compare) ? 1 : 0;
			m ++;
		}
		n = m - l;
		char passphrase[n];
		m = 0;
		while (argv[i][l + m] != '\0' && (l == 3 || l == 6)) {
			passphrase[m] = argv[i][l + m];
			m ++;
		}
		if (m != 0) {
			MAKE_KEY(passphrase);
			memset(passphrase, '\0', n);
		}
		else {
			while (argv[i][m] != '\0') {
				compare = (helpflag[0][m] == argv[i][m]) || (helpflag[1][m] == argv[i][m]);
				m ++;
			}
			if (compare) {
				printf("Triggered --help option. Exiting...\nThis is lightweight utility to encode you file via trivium cipher\nSyntax: trivium {-k=passphrase | -h}\n    -k=, --key=passphrase  specify passphrase\n    -h, --help  show this message and exit\n");
				FINISH(0);
			}
			else {
				printf("The %s option is unknown. Try -h. Exiting...\n", argv[i]);
				FINISH(1);
			}
		}
	}
	encrypt();
	FINISH(0);
}
