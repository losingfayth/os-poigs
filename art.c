#include "art.h"

/**
 * PRINT ART
 * 	Prints the formatted startup art
*/
void print_art()
{
	printf("\n");
	printf("********************************************************************************************************\n");
	printf("*%s                                               /'  /'                                                 %s*\n", CYN, RESET);
	printf("*%s                     /')                   --/'--/'                                                   %s*\n", WHT, RESET);
	printf("*%s                   /' /' ____              /'  /'__         ____              O  ,____     ,____      %s*\n", CYN, RESET);
	printf("*%s                -/'--' /'    )  /'    /  /'  /'    )      /'    )  /'    /  /'  /'    )   /'    )     %s*\n", WHT, RESET);
	printf("*%s               /'    /'    /' /'    /' /'  /'    /'     /'    /' /'    /' /'  /'    /'  /'    /'      %s*\n", CYN, RESET);
	printf("*%s             /(_____(___,/(__(___,/(__(__/'    /(__    (___,/(__(___,/(__(__/'    /(__/'    /(__      %s*\n", WHT, RESET);
	printf("*%s           /'                   /'                        /'                                          %s*\n", CYN, RESET);
	printf("*%s           /'                   /'                        /'                                          %s*\n", WHT, RESET);
	printf("*%s         /'             /     /'                       -/'-                                           %s*\n", CYN, RESET);
	printf("*%s       /'              (___,/'                        /'                                              %s*\n", WHT, RESET);
	printf("*                                                                                                      *\n");
	printf("*                                                                                                      *\n");
	printf("*%s                                                     :. ,..                                           %s*\n", MAG, RESET);
	printf("*%s                                                   .' :~.':_.,                                        %s*\n", MAG, RESET);
	printf("*%s                                                 .'   ::.::'.'                                        %s*\n", MAG, RESET);
	printf("*%s                                                :     ::'  .:                                         %s*\n", MAG, RESET);
	printf("*%s                                              `.:    .:  .:/                                          %s*\n", MAG, RESET);
	printf("*%s                                               `::--.:'.::'                                           %s*\n", MAG, RESET);
	printf("*%s                                                 |. _:===-'                                           %s*\n", MAG, RESET);
	printf("*%s                                                / /                                                   %s*\n", GRN, RESET);
	printf("*%s                               ,---.---.    __,','                                                    %s*\n", GRN, RESET);
	printf("*%s                              (~`.  \\   )   )','.,---..                                               %s*\n", GRN, RESET);
	printf("*%s                               `v`\\ | ,' .-'.:,'_____   `.                                            %s*\n", GRN, RESET);
	printf("*%s                                   )|/.-~.--~~--.   ~~~-. \\                                           %s*\n", GRN, RESET);
	printf("*%s                                 _/-'_.-~        ""---.._`.|                                            %s*\n", GRN, RESET);
	printf("*%s                            _.-~~_.-~                    ""'                                            %s*\n", GRN, RESET);
	printf("*%s                     _..--~~_.(~~                                                                     %s*\n", GRN, RESET);
	printf("*%s          __...---~~~_..--~~                                                                          %s*\n", GRN, RESET);
	printf("*%s       ,'___...---~~~                                                                                 %s*\n", GRN, RESET);
	printf("*%s                                                                                                      %s*\n", GRN, RESET);
	printf("********************************************************************************************************\n");
	
	printf("                             ");
	printf("%sf%sa%sy%st%sh %sq%su%si%sn%sn%s'%ss %s\"pretty %sokay %si %sguess\" %ss%sh%se%sl%sl%s\n\n", MAG, WHT, MAG, WHT, MAG, WHT, MAG, WHT, MAG, WHT, MAG, WHT, BLU, GRN, YEL, RED, CYN, WHT, CYN, WHT, CYN, RESET);

	printf("                                       ");
	printf("%s(っ◔◡◔)っ%s ❤ %s ᕙ(^▿^-ᕙ)%s\n\n", YEL, RED, CYN, RESET);
}

/**
 * PRINT ENTRY BAR
 * 	Prints a formatted message before the user input prompt
*/
void print_entry_bar()
{ printf("%s%s%s::%spoigs%s:%s%s%s:3 ", MAG, user, WHT, MAG, WHT, CYN, cwd, RESET); }

/**
 * PRINT EXIT MESSAGE
 * 	Prints a formatted message to the terminal on shell exit
*/
void print_exit_msg()
{ printf("%s> exiting ++poigs++%s\n> goodbye, %s%s%s! take care of yourself! %s(✿◠‿◠) %s\n", RED, BLU, CYN, user, BLU, CYN, RESET); }