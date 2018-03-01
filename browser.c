#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "mystack.h"
	
void writewebfile(FILE *htmlfile) {
	fputs("<html>\n",htmlfile);
	fputs("<head>\n",htmlfile);
	fputs("<title>CSC 317 Browser implementation with stack</title>\n",htmlfile);
	//fputs("CSC 317 Browser implementation with stack\n",htmlfile);
	//fputs("</title>\n",htmlfile);
	fputs("</head>\n",htmlfile);
	fputs("<body>\n",htmlfile);
	fputs("<p color='red'>This paragraph has red font colour</p>\n",htmlfile);
	//fputs("This is a paragraph\n",htmlfile);
	//fputs("</p>\n",htmlfile);
	//fputs("<p some='some'>\n",htmlfile);
	fputs("<center>\n",htmlfile);
	fputs("A center tag\n",htmlfile);
	fputs("</center>\n",htmlfile);
	fputs("<p color='green'>This paragraph has green font colour</p>\n",htmlfile);
	fputs("<p color='yellow'>This paragraph has pruple font colour</p>\n",htmlfile);
	fputs("<p color='grey'>This paragraph has grey font colour</p>\n",htmlfile);
	//fputs("</p>\n",htmlfile);
	fputs("</body>\n",htmlfile);
	fputs("</html>\n",htmlfile);
}

int isopeningtag(char* opening, char* closing) {
	int k;
	for (k=0; k<strlen(closing); k++) {
		if (opening[k] != closing[k]) return 0;
		return 1;
	}
}

applyattr (char* attr, char* value) {
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!strcmp(attr, "color")) {
		if (!strcmp(value, "blue")) SetConsoleTextAttribute(hConsole, 249);
		else if (!strcmp(value, "red")) SetConsoleTextAttribute(hConsole, 252);
		else if (!strcmp(value, "green")) SetConsoleTextAttribute(hConsole, 250);
		else if (!strcmp(value, "grey")) SetConsoleTextAttribute(hConsole, 248);
		else if (!strcmp(value, "yellow")) SetConsoleTextAttribute(hConsole, 254);
		else if (!strcmp(value, "purple")) SetConsoleTextAttribute(hConsole, 253);
		else SetConsoleTextAttribute(hConsole, 240);
	}
	else if (!strcmp(attr, "bgcolor")) {
		
	}
}
unapplyattr (char* attr) {
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!strcmp(attr, "color") || !strcmp(attr, "bgcolor")) SetConsoleTextAttribute(hConsole, 240);
	else if (!strcmp(attr, "h1")) SetConsoleTextAttribute(hConsole, 240);
	return;
}



int render (char* tag, char* data) {
	char tagtemp[7], attr[10], value[10];
	int i,j=0,k=0,withattr=0;
	
	for (i=0; i<strlen(tag); i++) {
		if(tag[i] != ' ') tagtemp[j++] = tag[i];
		else {
			withattr=1;
			while (tag[++i] != '=')
				attr[k++] = tag[i];
			++i;
			attr[k] = '\0';
			k=0;
			while (tag[++i] != '\'') {
				value[k++] = tag[i];
			}
			value[k] = '\0';
		}
		tagtemp[j] = '\0';
	}
	
	if (!strcmp(tag, "html") || !strcmp(tag, "head") || !strcmp(tag, "body")) //tags with no visible display
	{
		if (!withattr) return 0; //Don't do anything
		else {
			//handle the attribute
			return 1;
		}
	}
	else {
		if (!strcmp(tagtemp, "title")) {
			SetConsoleTitle(data);
		}
		else if (!strcmp(tagtemp, "center")){
			if(!withattr) {
				printf ("\n");
				for(i=0; i<((80-strlen(data))/2); i++) printf(" ");
				printf ("%s\n",data);
			}
			else {
				applyattr (attr, value);
				printf ("\n");
				for(i=0; i<((80-strlen(data))/2); i++) printf(" ");
				printf ("%s\n",data);
				unapplyattr (attr);
			}
		}
		else if (!strcmp(tagtemp, "p")){
			if(!withattr) printf ("\n%s\n",data);
			else {
				applyattr (attr, value);
				printf ("\n%s\n",data);
				unapplyattr (attr);
			}
		}
	}
	return 1;
}

int main () {
	char buffer[maxContentLen], c, tempContent[maxContentLen], data[maxContentLen], tempType[maxContentLen];
	int i=0, closetag;
	stack webstack;
	FILE *htmlfile = fopen("webpage.htm", "w");
	system("COLOR F0");
	initialize (&webstack);
	writewebfile(htmlfile);
	fclose(htmlfile);
	//Reading the file
	htmlfile = fopen("webpage.htm", "r");

	while ((c=fgetc(htmlfile)) != EOF) {
		if (c == '\n') continue;
		if (c == '<') {
			if (i) {
				buffer[i] = '\0';
				push (&webstack, "data", buffer);
				strcpy (buffer,"");
				i=0;
			}
			closetag = 0;
			while ((c=fgetc(htmlfile)) != '>'){
				if (c != '/') buffer[i++] = c;
				else closetag = 1;
			}
			if (!closetag) {
				buffer[i] = '\0';
				push (&webstack, "tag", buffer);
				strcpy (buffer,"");
				i=0;
			}
			else {
				//pop until and render
				buffer[i] = '\0';
				
				top(webstack, tempContent, tempType);
				while (!isopeningtag(tempContent, buffer)) {
					strcpy(data,tempContent);
					pop (&webstack);
					top(webstack, tempContent, tempType);
				}
				if (isopeningtag(tempContent, buffer) && strlen(data)) {
					render (tempContent, data);
					strcpy (data,"");
				}
				
				pop (&webstack);
				strcpy (buffer,"");
				i=0;
			}
		}
		else {
			buffer[i++] = c;
		}
	}
	//Reading the file
	
	fclose(htmlfile);
	return 0;
}
