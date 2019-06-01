#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../library/finger.h"

void lower_string(char s[]) {
   int c = 0;
   
   while (s[c] != '\0') {
      if (s[c] >= 'A' && s[c] <= 'Z') {
         s[c] = s[c] + 32;
      }
      c++;
   }
}

int checktoken(char s[]){
    if (s[0] != '('){ 
        return 0;
        }
    else if (s[4] != ')') {
        return 0;
    }else if (s[5] != '\n') return 2;
    else return 1;
}

int checkvalues(char s[]){
    if (isdigit(s[1]) == 0 || isdigit(s[3]) == 0) return 0;
    else return 1;
}

int checkcomma (char s[]){
    if (s[2] != ',') return 0;
    else return 1;
}
int main(int argc, char *argv[]){
    if (argc != 3) {
        printf("Error: Wrong number of arguments\n");
        return 1;
    } else if (strcmp(argv[1],"-c") != 0){
        printf("Error: Wrong option input (write -c)\n");
        return 1;
    }
    //set_device("/dev/ttyardu0");
   // printf("Device Set\n");
    //printf("Processing: STARTED\n");
    int allgood = 1;
    char *line_buf = NULL;
    char * token = (char*) malloc(2 * sizeof(char*));
    char * prev_token = (char*) malloc(2 * sizeof(char*));
    int x = 0;
    int y = 0;
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;
    const char s[2] = " ";
    /* Open the file for reading */
    FILE *fp = fopen(argv[2], "r");
    //FILE *outputfile = fopen("output_code", "w");
    //fclose(outputfile);
    //outputfile = fopen("output_code", "a");
    if (!fp)
    {
        fprintf(stderr, "Error opening file '%s'\n", argv[2]);
        fclose(fp);
        //fclose(outputfile);
        return 1;
    }
    set_device("/dev/ttyardu0");
    printf("Device Set\n");
    printf("Processing: STARTED\n");
    /* Loop through until we are done with the file. */
    while (line_size >= 0)
    {

        /* Get the next line */
        line_size = getline(&line_buf, &line_buf_size, fp);
        if (line_size <0){
            break;
        }
        token = strtok(line_buf, s);
        lower_string(token);
        
        if (strcmp(token,"pick\n") == 0){
            //printf("Found pick\n");
            pick();
            //wait(1);
        }else if(strcmp(token,"drop\n") == 0){
            //printf("Found drop\n");
            drop();
            //wait(1);
        }else if (strcmp(token,"move") == 0 || strcmp(token,"move&pick") == 0 || strcmp(token,"move&drop") == 0) {
            
            //fprintf(outputfile, "%s", token);
            prev_token = token;
            token = strtok(NULL, s);
            if (checktoken(token) != 1){
                if (checktoken(token) == 2){
                    printf("Syntax Error: Wrong Instruction ending in line  %i . All Instructions must end with line jump (not space or anything else)\n",line_count+1);    
                    allgood = 0;
                    break;
                }else
                {
                    printf("Syntax Error: Wrong Parenthesis use in line %i. Remember to close and open parenthesis. Remember that no spaces shall go between characters in the position definition arguments \n",line_count+1);
                    allgood = 0;
                    break;
                }
            }else if (checkcomma(token) != 1){
                printf("Syntax Error: Wrong comma use or lack of it between values in line %i \n",line_count+1);
                allgood = 0;
                break;
            }else if (checkvalues(token) != 1){
                printf("Syntax Error: Values introduced are not valid decimal values in range [0-9] in line %i \n",line_count+1);
                allgood = 0;
                break;
            }else {
                //printf("Device Set\n");
                x = atoi(&token[1]);
                y = atoi(&token[3]);
                x++;
                y++;
                //printf("x: %i\n",x);
                //printf("y: %i\n",y);
                //printf("Line Number: %i\n",line_count);
                //printf("prev_token: %s\n",prev_token);
                if (strcmp(prev_token,"move") == 0) {
                    //printf("Found move\n");
                    move(x,y);
                    //wait(1);
                }else if (strcmp(prev_token,"move&pick") == 0){
                    //printf("Found move&pick\n");
                    move(x,y);
                    //wait(1);
                    pick();
                    //wait(1);
                } else if (strcmp(prev_token,"move&drop") == 0) {
                    //printf("Found move&drop\n");
                    move(x,y);
                    //wait(1);
                    drop();
                    //wait(1);
                }
                //fprintf(outputfile, "%s", " ");
                //fprintf(outputfile, "%s", token);
            }

        }else {
            printf("Error: Instruction Not Recognized in line %i \n",line_count+1);
            allgood = 0;
            break;
            }
        
        /* Increment our line count */
        line_count++;
    }
    if (allgood == 1){
        printf("Finished configuration file Processing succesfully \n");
    }else
    {
        printf("Errors where found \n");
    }
    /* Free the allocated line buffer */
    free(line_buf);
    line_buf = NULL;

    /* Close the file now that we are done with it */
    fclose(fp);
    //fclose(outputfile);

    return 0;
}


