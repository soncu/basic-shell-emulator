#include <iostream>	//cout,endl
#include <unistd.h>	//getcwd,chdir
#include <cstring>	//strcpy,strcat,strcmp,strlen,
#include <cstdlib>	//system
#include <sstream>	//stringstream
#include <cassert>	//assert
#include <dirent.h>	//opendir,readdir,closedir

#define MY_LEN 200	//probable max path length
#define MY_TOK 3	//max num of command options

using namespace std;

char *pcurdir = NULL;	//current path variable (might be in a static field of class)

void Init(void)
{
	if(pcurdir == NULL) {
		pcurdir = new char[MY_LEN];
	}
	getcwd(pcurdir, MY_LEN);
}

void Clear(void)
{
	if(pcurdir != NULL) {
		delete [] pcurdir;
	}
}

void MyHelp(void)
{
	cout << "----Active Commands----" << endl;
	cout << "clear : clear current prompt" << endl;
	cout << "exit : terminate the Ismulator" << endl;
	cout << "pwd : show current directory" << endl;
	cout << "ls : list files and directories in the current folder" << endl;
	cout << "cd : change directory to targer directory" << endl;
	cout << "./runnable : execute a program" << endl;
}

void MyPwd(void)
{
	assert(pcurdir != NULL);
	getcwd(pcurdir, MY_LEN);
	cout << pcurdir << endl;
}

void MyLs(const string& opt)
{
	if(opt.size()) {	//lets do a trick
		char ls[50];
		strcpy(ls, "ls ");
		strcat(ls, opt.c_str());
		system(ls);
	}
	else {
		struct dirent *entry;
		DIR *dp = opendir(pcurdir);	//list files in current dir
	
		if(dp != NULL) {
			while((entry = readdir(dp))) {
				cout << entry->d_name << endl;
			}
			closedir(dp);
		}
	}
}

void MyCd(const string& opt)
{
	if(!opt.compare("/")) {
		if(chdir("/") == 0) {
			strcpy(pcurdir, "/");
		}
		
	}
	if(!opt.compare(".")) {	//do nothing for current dir
		return;
	}
	else if(!opt.compare("..")) {	//back to parent
		
		for(int i = strlen(pcurdir) - 1; i >= 0; --i) {
			if(pcurdir[i] == '/') {
				pcurdir[i == 0 ? i + 1 : i] = '\0';	//handling root folder('/')
				break;
			}
		}
		
		chdir(static_cast<const char*>(pcurdir));
	}
	else { 
	
		char temp[100];
		strcpy(temp, pcurdir);

		if(strcmp(temp, "/"))		//careful for root folder(if not root)
			strcat(temp, "/");

		strcat(temp, opt.c_str());
	
		if(chdir(temp) == 0) {	//0 on success
			strcpy(pcurdir, temp);
		}
	}
}

void ProcessCommand(const string& line)
{
	istringstream iss(line);
	string toks[MY_TOK];	//max number of tokens supported
	int i = 0;		//index, also token counter

	iss >> toks[0];
	for(i = 1; i < MY_TOK && iss >> toks[i]; ++i)
	;

	if(!toks[0].compare("exit")) {
		exit(0);
	}
	else if(!toks[0].compare("clear")) {
		system("clear");
	}
	else if(!toks[0].compare("help")) {
		MyHelp();
	}
	else if(!toks[0].compare("pwd")) {
		MyPwd();
	}
	else if(!toks[0].compare("ls")) {
		if(i > 1) {
			MyLs(toks[1]);
		}
		else {
			MyLs("");
		}
	}
	else if(!toks[0].compare("cd")) {
		if(i > 1) {
			MyCd(toks[1]);
		}	//else wrong cd command
	}
	else if(system(toks[0].c_str()) == -1) {	//execute an executable
		cout << "Unknow command: " << toks[0] << endl; 
	}
}

int main(int argc, char* argv[])
{
	string line;		      //commands	

	cout << "Welcome to Ismulator, 'help' for command lists." << endl;
	
	Init();
	
	while(true){
		cout << pcurdir << "$>";
		getline(cin, line);
		ProcessCommand(line);
	}
	
	Clear();	//release heap memory(for clearity)
	
	return 0;
}
