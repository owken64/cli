#include <stdlib.h>
#include <math.h>

#define SIGN_PLUS 0
#define SIGN_MINUS 1


char* IntToString(int num) {
	int num_alter;
	int sign;
	int keta;
	char* rt;

	if (num < 0) { sign = SIGN_MINUS; }
	else { sign = SIGN_PLUS; }

	num_alter = abs(num);

	if (num_alter != 0)	keta = floor(log10(num_alter) + 1);
	else keta = 1;

	rt = (char*)malloc(sizeof(char) * (sign + keta + 1));

	rt[sign + keta] = '\0';
	int end;
	if (sign == SIGN_PLUS) end = 0;
	else end = 1;

	for (int i = keta - 1; i >= end; i--) {
		int n = num_alter % 10;
		rt[i] = n + '0';
		num_alter = num_alter / 10;
	}

	if (sign == SIGN_MINUS) rt[0] = '-';

	return rt;
}

void ReleaseString(char* str) {
	free(str);
}

int len(char* str) {
	int rt = 0;
	while (str[rt] != '\0') rt++;
	return rt;
}

char* concat(char* str1, char* str2) {
	int keta1, keta2;
	char* rt;

	keta1 = len(str1);
	keta2 = len(str2);

	rt = (char *)malloc(sizeof(char) * (keta1 + keta2 + 1));

	for (int i = 0; i < keta1; i++) {
		rt[i] = str1[i];
	}
	for (int j = 0; j < keta2; j++) {
		rt[keta1 + j] = str2[j];
	}
	rt[keta1 + keta2] = '\0';

	return rt;
}

char* formatedString(char* str, int n_line) {
	int length;
	length = len(str);
	int n_CRLF = length / n_line;
	char* rt;
	rt = (char*)malloc(sizeof(char) * (length + n_CRLF+1));
	
	int j = 0; // CRLF
	for (int i = 0; i < length; i++) {
		rt[i+j] = str[i];
		if ((i != 0) && (i % n_line == 0)) {
			j++;
			rt[i + j] = '\n';
		}
	}
	rt[length + n_CRLF] = '\0';

	return rt;
}
