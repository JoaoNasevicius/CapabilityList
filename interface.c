#include "interface.h"

#define TRUE 	0
#define FALSE 	1
#define ANSI_COLOR_RESET   	"\x1b[0m"
#define ANSI_COLOR_GREEN   	"\x1b[32m"
#define ANSI_COLOR_BLUE		"\x1B[34m"

void shell();
int findUser(char *userName, char *userPassword);
char getch();

void initialize() {
	char user[30], password[30], c = ' ';
	int i;

	printf("Username: ");
	scanf("%s", &user);
	
	printf("Password: ");
	while (i < 30){
	    password[i] = getch();
	    c = password[i];
	    if(c < 33 || c > 176) 
	    	break;
	    	else printf("*");
	    i++;
	}
	password[i]='\0';
	printf("\n");

	if (findUser(user, password) == TRUE)
		shell(user);

}

void shell(char *userName) {
	int aux;
	char inputCommand[100], defaultPathname[8] = "./Files", 
		 pathname[100] = "/", command[261];
	strcat(pathname, userName);

	while(TRUE == TRUE) {
		printf(ANSI_COLOR_GREEN "user@system:" ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "$ ", pathname);
		fgets(inputCommand, 100, stdin);
		fflush(stdin);

		if (strncmp(inputCommand, "cd ", 2) == TRUE) {
			if (strstr(inputCommand, "..") != NULL) {
				for (int i = 0; i < strlen(pathname); ++i) {
					if (pathname[i] == '/')
						aux = i;			
				}

				pathname[aux] = '\0';
			}

			else if (strstr(inputCommand, "./") != NULL) {
				strcat(pathname, "/");
				strcat(pathname, inputCommand + 5);
				pathname[strlen(pathname) - 1] = '\0';
			}

			else {
				strcat(pathname, "/");
				strcat(pathname, inputCommand + 3);
				pathname[strlen(pathname) - 1] = '\0';
			}
		}

		else if(strncmp(inputCommand, "pwd", 3) == TRUE) 
			printf("%s\n", pathname);

		else if(strncmp(inputCommand, "whoami", 6) == TRUE)
			printf("%s\n", userName);

		else {
			strcpy(command, "cd ");
			strcat(command, defaultPathname);
			strcat(command, pathname);
			strcat(command, " && ");
			strcat(command, inputCommand);

			system(command);


		//
		//	aqui entra a função que é responsável por ver se o comando pode ser executado!
		//  Tem que fazer uma função que recebe como parametro o inputCommand (o comando do usuário), verifica o que ele quer fazer e 
		//	ve se é possivel ou não.	
		//
//		system(inputCommand);
		}
	}
}

int findUser(char *userName, char *userPassword) {
	FILE *accounts;
	char stringRead[30];

	accounts = fopen("./Files/users.txt", "r+");

	while(fscanf(accounts, "%s", stringRead) != EOF) {
		if(strcmp(userName, stringRead) == TRUE) {
			fscanf(accounts, "%s", stringRead);

			if (strcmp(userPassword, stringRead) == TRUE){
				fclose(accounts);
			
				return TRUE;
			}
		}
		fscanf(accounts, "%s", stringRead);
	}
	fclose(accounts);
	return FALSE;
}

char getch(){
	char character;
	struct termios old;
	int oldVMIN, oldVTIME;
	fflush(stdout);

	/* Carrega configurações do terminal atual para preencher os demais campos */
	/* da struct termios, garantindo boa execução do terminal após as mudanças */
	tcgetattr(0, &old);				

	old.c_lflag     &= ~ICANON;
	old.c_lflag     &= ~ECHO;
	oldVMIN 		= old.c_cc[VMIN];
	oldVTIME		= old.c_cc[VTIME];
	old.c_cc[VMIN]  = 1;
	old.c_cc[VTIME] = 0;
	
	tcsetattr(0, TCSANOW, &old); /* Realiza as mudanças necessárias */
	
	read(0, &character, 1); 	 /* Le o caracter escrito */

	old.c_lflag 	|= ICANON;
	old.c_lflag 	|= ECHO;
	old.c_cc[VMIN] 	= oldVMIN;
	old.c_cc[VTIME]	= oldVTIME;
	
	tcsetattr(0, TCSADRAIN, &old); /* Volta para as configurações antigas */ 
	
	return character;
}