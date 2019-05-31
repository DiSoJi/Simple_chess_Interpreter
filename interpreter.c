#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void lower_string(char s[]) {
   int c = 0;
   
   while (s[c] != '\0') {
      if (s[c] >= 'A' && s[c] <= 'Z') {
         s[c] = s[c] + 32;
      }
      c++;
   }
}
/*
void clean_token(char s[]){
    int c = 0;
   
    while (s[c] != '\0') {
      if ((s[c] == '(' ) || (s[c] == ')') || (s[c] == ',')) {
        s[c] = ' ';
      }
      else
      {
        c++;
      }
      
   }

}*/
int checktoken(char s[]){
    //printf("Token in checktoken:%s \n",s);
    //printf("Last char %c \n",s[5]);
    if (s[0] != '('){ 
        //printf ("s[0] %c \n", s[0]);
        return 0;
        }
    else if (s[4] != ')') {
        //printf ("s[-1] %c \n", s[4]);
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
    printf("Processing: STARTED\n");
    int allgood = 1;
    char *line_buf = NULL;
    char * token = (char*) malloc(2 * sizeof(char*));
    size_t line_buf_size = 0;
    int line_count = 0;
    ssize_t line_size;
    const char s[2] = " ";
    /* Open the file for reading */
    FILE *fp = fopen(argv[2], "r");
    FILE *outputfile = fopen("output_code", "w");
    fclose(outputfile);
    outputfile = fopen("output_code", "a");
    if (!fp)
    {
        fprintf(stderr, "Error opening file '%s'\n", argv[2]);
        fclose(fp);
        fclose(outputfile);
        return 1;
    }

    /* Get the first line of the file. */
    //line_size = getline(&line_buf, &line_buf_size, fp);

    /* Loop through until we are done with the file. */
    while (line_size >= 0)
    {
        //printf("Enter while\n");
        /* Show the line details */
        //printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s", line_count,
        //    line_size, line_buf_size, line_buf);
        
        /* Get the next line */
        line_size = getline(&line_buf, &line_buf_size, fp);
        if (line_size <0){
            break;
        }
        //printf("After get line\n");
        token = strtok(line_buf, s);
        //printf("After tokenizing\n");
        lower_string(token);
        //clean_token(token);
        //printf("Token: %s \n",token);
        if (strcmp(token,"pick\n") == 0 || strcmp(token,"drop\n") == 0){
            fprintf(outputfile, "%s", token);
            //printf("Found pick or drop \n");
        }else if (strcmp(token,"move") == 0 || strcmp(token,"move&pick") == 0 || strcmp(token,"move&drop") == 0) {
            fprintf(outputfile, "%s", token);
            //printf("Found move \n");
            token = strtok(NULL, s);
            /*printf( "This is the token: %s\n", token );
            int Tokensize = strlen(token);
            printf("This is Tokensize: %i \n",Tokensize );*/
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
                fprintf(outputfile, "%s", " ");
                fprintf(outputfile, "%s", token);
                //printf("Valid line \n");
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
    fclose(outputfile);

    return 0;
}


