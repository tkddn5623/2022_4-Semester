#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NOFILE
typedef enum _Day { Mon = 0, Tue, Wed, Thu, Fri, Sat, Sun } Day;
const char* const Daystr[7] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
typedef struct _Resultv {
	int diff;
	int month;
	Day day;
} Resultv;
Resultv parser(const char* str) {
	Resultv ret = { 0 };
	char temp[6];
	sscanf(str, "%*d/%d/%*d %s (%d)", &ret.month, temp, &ret.diff);
	if (!strcmp(temp, Daystr[0])) {
		ret.day = Mon;
	}
	else if (!strcmp(temp, Daystr[1])) {
		ret.day = Tue;
	}
	else if (!strcmp(temp, Daystr[2])) {
		ret.day = Wed;
	}
	else if (!strcmp(temp, Daystr[3])) {
		ret.day = Thu;
	}
	else if (!strcmp(temp, Daystr[4])) {
		ret.day = Fri;
	}
	else if (!strcmp(temp, Daystr[5])) {
		ret.day = Sat;
	}
	else if (!strcmp(temp, Daystr[6])) {
		ret.day = Sun;
	}
	else {
		printf("Wrong input file.\n");
		exit(1);
	}
	return ret;
}
int main(int argc, char* argv[]) {
	int total;
	char buffer[128];
	Resultv result[1000] = { 0 };
	FILE* file = NULL;
	if ((file = fopen("date.txt", "r")) == NULL) {
		printf("File open error.\n");
		exit(1);
	}
	for (total = 0; fgets(buffer, sizeof(buffer), file); total++) {
		result[total] = parser(buffer);
	}
	for (int i = 0; i < total; i++) {
		printf("%d, %s, %d\n", result[i].month, Daystr[result[i].day], result[i].diff);
	}
	//전체 평균, 표준편차
	//요일별 평균, 표준편차
	//
	//
}