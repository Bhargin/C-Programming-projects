#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <string.h>
#include <errno.h>
#include "document.h"

/* Command struct. It stores command given by user input. */
typedef struct
{
    char command[MAX_STR_SIZE + 1], line_content[MAX_STR_SIZE + 1], replace_str[MAX_STR_SIZE + 1];
    int paragraph_num, line_num, values_in;
} Command;

/* Prototype. */
void process(Command *, int, int, Document *, int *);

int main(int argc, char *argv[])
{
    /* All required variables for the program. */
    FILE *input;
    Document doc;

    /* Array of commands so that user_input can be processed at once,
    after file is read. */
    Command commands[MAX_STR_SIZE];
    const char *doc_name = "main_document";

    /* These various char array store information about command, line_content,
    any additional content, replace string for replace_text function and
    etc. for further processing. */
    char line[MAX_STR_SIZE + 1], data[MAX_STR_SIZE + 1], command[MAX_STR_SIZE + 1], 
    line_content[MAX_STR_SIZE + 1], additional_cont[MAX_STR_SIZE + 1], replace_txt[MAX_STR_SIZE + 1],
    temp1[MAX_STR_SIZE + 1], temp2[MAX_STR_SIZE + 1];

    /* These integers are either user for loops or to refer to paragraph
    number, line number and etc. */
    int i, k, p, q, t, z, values_read, paragraph_num, line_num, 
    load_file_idxs[MAX_PARAGRAPHS], load_file_nums = 0, cmd_idx = 0, ex = 0;

    /* Init a document. */
    init_document(&doc, doc_name);
    z = 0;

    /* If input is from standard input. */
    if (argc == 1)
    {
        /* Create a input stream. */
        input = stdin;

        /* Loop-until END OF FILE. */
        while (fgets(data, MAX_STR_SIZE + 1, input) != NULL)
        {
            /* Initilizae all struct variables and local variables to default values. 
            This helps in recognizing which commands are invalid or if 
            the scanf wasn't able to read a specific value. */
            strcpy(commands[cmd_idx].command, "N/A");
            strcpy(commands[cmd_idx].line_content, "N/A");
            strcpy(commands[cmd_idx].replace_str, "N/A");
            commands[cmd_idx].paragraph_num = -1;
            commands[cmd_idx].line_num = -1;
            commands[cmd_idx].values_in = -1;

            strcpy(command, "N/A");
            strcpy(line_content, "N/A");
            strcpy(replace_txt, "N/A");
            paragraph_num = -1;
            line_num = -1;
            values_read = -1;

            /* Scan the first string to see what is the command. */
            sscanf(data, "%s", command);

            /* The following code checks what the input command is and then
            creates a command variables to be put into the commands array. */

            /* Load file. */
            if (strcmp(command, "load_file") == 0)
            {
                /* Load-file command should only read two values. */
                values_read = sscanf(data, "%s%s %c", command, line_content, additional_cont);

                /* If invalid entries are given or read. */
                if (strcmp(line_content, "N/A") == 0 || values_read > 2)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }

                /* Else copy the input values and the command itself and append
                it to the commands arr. */
                else
                {
                    load_file_idxs[z] = cmd_idx;
                    ++z;
                    ++load_file_nums;
                    strcpy(commands[cmd_idx].command, command);
                    strcpy(commands[cmd_idx].line_content, line_content);
                    ++cmd_idx;
                }
            }

            /* Append line. */
            else if (strcmp(command, "append_line") == 0)
            {
                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s%d %80c", command, &paragraph_num, line_content);

                /* Print "Invalid command" if any invalid command is given. */
                if (paragraph_num < 0)
                {
                   strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else if (line_content[0] != '*')
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                /* Else add command to commands arr. */
                else
                {
                    strcpy(commands[cmd_idx].command, command);
                    strcpy(commands[cmd_idx].line_content, line_content);
                    commands[cmd_idx].paragraph_num = paragraph_num;
                    ++cmd_idx;
                }
            }

            /* Replace text. */
            else if (strcmp(command, "replace_text") == 0)
            {
                /* Read values. */
                values_read = sscanf(data, "%s%s %80c", command, line_content, replace_txt);

                /* Print error messages. */
                if (strcmp(line_content, "N/A") == 0)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }

                /* Add command to commands arr. */
                else
                {
                    k = 1;
                    p = 0;
                    q = 1;
                    t = 0;
                    while (line_content[k] != '\"')
                    {
                        temp1[p] = line_content[k];
                        ++p;
                        ++k;
                    }
                    temp1[p] = '\0';

                    while (replace_txt[q] != '\"')
                    {
                        temp2[t] = replace_txt[q];
                        ++t;
                        ++q;
                    }
                    temp2[t] = '\0';

                    strcpy(commands[cmd_idx].command, command);
                    strcpy(commands[cmd_idx].line_content, temp1);
                    strcpy(commands[cmd_idx].replace_str, temp2);
                    ++cmd_idx;
                }
            }

            /* Highlight text. */
            else if (strcmp(command, "highlight_text") == 0)
            {
                /* Read values. */
                values_read = sscanf(data, "%s %80c", command, line_content);

                /* Print_error messages. */
                if (strcmp(line_content, "N/A") == 0)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else
                {
                    k = 1;
                    p = 0;
                    while (line_content[k] != '\"')
                    {
                        temp1[p] = line_content[k];
                        ++p;
                        ++k;
                    }

                    temp1[p] = '\0';

                    /* Add command to commands arr. */
                    strcpy(commands[cmd_idx].command, command);
                    strcpy(commands[cmd_idx].line_content, temp1);
                    ++cmd_idx;
                }
            }

            /* Add-paragraph. */
            else if (strcmp(command, "add_paragraph_after") == 0)
            {
                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s%d %80c", command, &paragraph_num, line_content);

                /* Print "Invalid command" if any invalid command is given. */
                if (paragraph_num < 0)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else if (values_read > 2)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;

                    /* Else add the command to commands arr. */
                }
                else
                {
                    strcpy(commands[cmd_idx].command, command);
                    commands[cmd_idx].paragraph_num = paragraph_num;
                    ++cmd_idx;
                }
            }

            /* Add-line. */
            else if (strcmp(command, "add_line_after") == 0)
            {

                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s%d%d %80c", command, &paragraph_num, &line_num, line_content);

                /* Print error messages. */
                if (paragraph_num < 0 || line_num < 0)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else if (line_content[0] != '*')
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }

                 /* Else append command to commands arr. */
                else
                {
                    strcpy(commands[cmd_idx].command, command);
                    strcpy(commands[cmd_idx].line_content, line_content);
                    commands[cmd_idx].paragraph_num = paragraph_num;
                    commands[cmd_idx].line_num = line_num;
                    ++cmd_idx;
                }
            }

            /* Print_document. */
            else if (strcmp(command, "print_document") == 0)
            {

                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s %80c", command, line_content);

                /* Print error message. */
                if (values_read > 1)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else
                {
                    strcpy(commands[cmd_idx].command, command);
                    ++cmd_idx;
                }
            }

            /* Quit. */
            else if (strcmp(command, "quit") == 0)
            {

                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s %80c", command, line_content);

                if (values_read > 1)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else
                {
                    strcpy(commands[cmd_idx].command, command);
                    ++cmd_idx;
                }
            }

            /* Exit. */
            else if (strcmp(command, "exit") == 0)
            {

                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s %80c", command, line_content);

                if (values_read > 1)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else
                {
                    strcpy(commands[cmd_idx].command, command);
                    ++cmd_idx;
                }
            }

            /* Remove_line. */
            else if (strcmp(command, "remove_line") == 0)
            {

                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s%d%d %80c", command, &paragraph_num, &line_num, line_content);

                /* Print error message. */
                if (paragraph_num < 0 || line_num < 0)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else if (values_read > 3)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }

                /* Else add command to commands arr. */
                else
                {
                    strcpy(commands[cmd_idx].command, command);
                    commands[cmd_idx].paragraph_num = paragraph_num;
                    commands[cmd_idx].line_num = line_num;
                    ++cmd_idx;
                }
            } 

            /* Remove_text. */
            else if (strcmp(command, "remove_text") == 0) {
                
                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s %80c", command, line_content);

                /* Print error messages. */
                if (values_read < 2){
                   strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                
                /* Else add command to commands arr. */
                } else {
                    strcpy(commands[cmd_idx].command, command);
                    strcpy(commands[cmd_idx].line_content, line_content);
                    ++cmd_idx;
                }

            } 

            /* Save_document. */
            else if (strcmp(command, "save_document") == 0) {

                /* Read Values. */
                values_read = sscanf(data, "%s%s %c", command, line_content, additional_cont);

                /* Print error messages. */
                if (values_read > 2) {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                } else if (strcmp(line_content, "N/A") == 0){
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                } 

                /* Else add command to commands arr. */
                else {
                strcpy(commands[cmd_idx].command, command);
                strcpy(commands[cmd_idx].line_content, line_content);
                ++cmd_idx;
                }
            } 

            /* Reset_document. */
            else if (strcmp(command, "reset_document") == 0){
                
                /* Read values. */
                values_read = sscanf(data, "%s %c", command, additional_cont);

                /* Print error messages. */
                if (values_read > 1) {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                } else {

                /* Add command to commands arr. */
                strcpy(commands[cmd_idx].command, command);
                ++cmd_idx;
                }
            } 
            
            /* If comments or any non-relevent line is read. */
            else {
                ++ex;
            }
        }

        /* Print arrows marks to indicate how many commands were read. */
        for (i = 0; i < (cmd_idx + ex) - 1; ++i)
        {
            printf("> ");
        }

        /* Process the commands read from the input. */
        process(commands, cmd_idx, load_file_nums, &doc, load_file_idxs);
        printf(">\n");

        /* Close input stream. */
        fclose(input);
        exit(EXIT_SUCCESS);
    }

    /* The code below is a exact copy of the code above and therefore 
    have similar comments. The only difference here is that the input 
    stream is a file instead of stdin. */
    else if (argc == 2)
    {
        /* Open file for reading. */
        input = fopen(argv[1], "r");

        /* Print error message if file cannot be opened. */
        if (input == NULL)
        {
            fprintf(stderr, "%s cannot be opened.\n", argv[1]);
            exit(EX_OSERR);
        }

        /* Else continue reading the lines from the file until 
        end of file is reached. */
        else
        {
            fgets(data, MAX_STR_SIZE + 1, input);
            while (!feof(input))
            {
            
            /* Initilizae all struct variables and local variables to default values. 
            This helps in recognizing which commands are invalid. */
            strcpy(commands[cmd_idx].command, "N/A");
            strcpy(commands[cmd_idx].line_content, "N/A");
            strcpy(commands[cmd_idx].replace_str, "N/A");
            commands[cmd_idx].paragraph_num = -1;
            commands[cmd_idx].line_num = -1;
            commands[cmd_idx].values_in = -1;

            strcpy(command, "N/A");
            strcpy(line_content, "N/A");
            strcpy(replace_txt, "N/A");
            paragraph_num = -1;
            line_num = -1;
            values_read = -1;

            /* Scan the first string to see what is the command. */
            sscanf(data, "%s", command);

            /* The following code checks what the input command is and then
            creates a command variables to be put into the commands array. */

            /* Load file. */
            if (strcmp(command, "load_file") == 0)
            {
                /* Load-file command should only read two values. */
                values_read = sscanf(data, "%s%s %c", command, line_content, additional_cont);

                /* If invalid entries are given or read. */
                if (strcmp(line_content, "N/A") == 0 || values_read > 2)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }

                /* Else copy the input values and the command itself and append
                it to the commands arr. */
                else
                {
                   load_file_idxs[z] = cmd_idx;
                    ++z;
                    ++load_file_nums;
                    strcpy(commands[cmd_idx].command, command);
                    strcpy(commands[cmd_idx].line_content, line_content);
                    ++cmd_idx;
                }
            }

            /* Append line. */
            else if (strcmp(command, "append_line") == 0)
            {
                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s%d %80c", command, &paragraph_num, line_content);

                /* Print "Invalid command" if any invalid command is given. */
                if (paragraph_num < 0)
                {
                   strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else if (line_content[0] != '*')
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                /* Else add command to commands arr. */
                else
                {
                    strcpy(commands[cmd_idx].command, command);
                    strcpy(commands[cmd_idx].line_content, line_content);
                    commands[cmd_idx].paragraph_num = paragraph_num;
                    ++cmd_idx;
                }
            }

            /* Replace text. */
            else if (strcmp(command, "replace_text") == 0)
            {
                /* Read values. */
                values_read = sscanf(data, "%s%s %80c", command, line_content, replace_txt);

                /* Print error messages. */
                if (strcmp(line_content, "N/A") == 0)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }

                /* Add command to commands arr. */
                else
                {
                    k = 1;
                    p = 0;
                    q = 1;
                    t = 0;
                    while (line_content[k] != '\"')
                    {
                        temp1[p] = line_content[k];
                        ++p;
                        ++k;
                    }
                    temp1[p] = '\0';

                    while (replace_txt[q] != '\"')
                    {
                        temp2[t] = replace_txt[q];
                        ++t;
                        ++q;
                    }
                    temp2[t] = '\0';

                    strcpy(commands[cmd_idx].command, command);
                    strcpy(commands[cmd_idx].line_content, temp1);
                    strcpy(commands[cmd_idx].replace_str, temp2);
                    ++cmd_idx;
                }
            }

            /* Highlight text. */
            else if (strcmp(command, "highlight_text") == 0)
            {
                /* Read values. */
                values_read = sscanf(data, "%s %80c", command, line_content);

                /* Print_error messages. */
                if (strcmp(line_content, "N/A") == 0)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else
                {
                    k = 1;
                    p = 0;
                    while (line_content[k] != '\"')
                    {
                        temp1[p] = line_content[k];
                        ++p;
                        ++k;
                    }

                    temp1[p] = '\0';

                    /* Add command to commands arr. */
                    strcpy(commands[cmd_idx].command, command);
                    strcpy(commands[cmd_idx].line_content, temp1);
                    ++cmd_idx;
                }
            }

            /* Add-paragraph. */
            else if (strcmp(command, "add_paragraph_after") == 0)
            {
                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s%d %80c", command, &paragraph_num, line_content);

                /* Print "Invalid command" if any invalid command is given. */
                if (paragraph_num < 0)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else if (values_read > 2)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;

                    /* Else add the command to commands arr. */
                }
                else
                {
                    strcpy(commands[cmd_idx].command, command);
                    commands[cmd_idx].paragraph_num = paragraph_num;
                    ++cmd_idx;
                }
            }

            /* Add-line. */
            else if (strcmp(command, "add_line_after") == 0)
            {

                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s%d%d %80c", command, &paragraph_num, &line_num, line_content);

                /* Print error messages. */
                if (paragraph_num < 0 || line_num < 0)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else if (line_content[0] != '*')
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }

                 /* Else append command to commands arr. */
                else
                {
                    strcpy(commands[cmd_idx].command, command);
                    strcpy(commands[cmd_idx].line_content, line_content);
                    commands[cmd_idx].paragraph_num = paragraph_num;
                    commands[cmd_idx].line_num = line_num;
                    ++cmd_idx;
                }
            }

            /* Print_document. */
            else if (strcmp(command, "print_document") == 0)
            {

                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s %80c", command, line_content);

                /* Print error message. */
                if (values_read > 1)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else
                {
                    strcpy(commands[cmd_idx].command, command);
                    ++cmd_idx;
                }
            }

            /* Quit. */
            else if (strcmp(command, "quit") == 0)
            {

                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s %80c", command, line_content);

                if (values_read > 1)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else
                {
                    strcpy(commands[cmd_idx].command, command);
                    ++cmd_idx;
                }
            }

            /* Exit. */
            else if (strcmp(command, "exit") == 0)
            {

                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s %80c", command, line_content);

                if (values_read > 1)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else
                {
                    strcpy(commands[cmd_idx].command, command);
                    ++cmd_idx;
                }
            }

            /* Remove_line. */
            else if (strcmp(command, "remove_line") == 0)
            {

                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s%d%d %80c", command, &paragraph_num, &line_num, line_content);

                /* Print error message. */
                if (paragraph_num < 0 || line_num < 0)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }
                else if (values_read > 3)
                {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                }

                /* Else add command to commands arr. */
                else
                {
                    strcpy(commands[cmd_idx].command, command);
                    commands[cmd_idx].paragraph_num = paragraph_num;
                    commands[cmd_idx].line_num = line_num;
                    ++cmd_idx;
                }
            } 

            /* Remove_text. */
            else if (strcmp(command, "remove_text") == 0) {
                
                /* Read all values to see if valid data is given or not. */
                values_read = sscanf(data, "%s %80c", command, line_content);

                /* Print error messages. */
                if (values_read < 2){
                   strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                
                /* Else add command to commands arr. */
                } else {
                    strcpy(commands[cmd_idx].command, command);
                    strcpy(commands[cmd_idx].line_content, line_content);
                    ++cmd_idx;
                }

            } 

            /* Save_document. */
            else if (strcmp(command, "save_document") == 0) {

                /* Read Values. */
                values_read = sscanf(data, "%s%s %c", command, line_content, additional_cont);

                /* Print error messages. */
                if (values_read > 2) {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                } else if (strcmp(line_content, "N/A") == 0){
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                } 

                /* Else add command to commands arr. */
                else {
                strcpy(commands[cmd_idx].command, command);
                strcpy(commands[cmd_idx].line_content, line_content);
                ++cmd_idx;
                }
            } 

            /* Reset_document. */
            else if (strcmp(command, "reset_document") == 0){
                
                /* Read values. */
                values_read = sscanf(data, "%s %c", command, additional_cont);

                /* Print error messages. */
                if (values_read > 1) {
                    strcpy(commands[cmd_idx].command, "Invalid Command\n");
                    ++cmd_idx;
                } else {

                /* Add command to commands arr. */
                strcpy(commands[cmd_idx].command, command);
                ++cmd_idx;
                }
            } 
            
                fgets(data, MAX_STR_SIZE + 1, input);
                
            }

            /* Process commands and close input stream. */
            process(commands, cmd_idx, load_file_nums, &doc, load_file_idxs);
            fclose(input);
            exit(EXIT_SUCCESS);
        }
    }

    /* Else print error message if invalid use of command line
    arguments. */
    else
    {
        fprintf(stderr, "user_interface\n");
        fprintf(stderr, "user_interface <filename>\n");
        exit(EX_USAGE);
    }

    return 0;
}

/* Function process each command read from input stream. */
void process(Command *commands, int count, int load_file_nums, Document *doc, int * load_file_idxs)
{

    int i, k, p, j, temp, line_content_size;
    char temp1[MAX_STR_SIZE + 1], final_line[MAX_STR_SIZE + 1] = "";

    temp = 0;

    for (i = 0; i < count; ++i)
    {
        /* Add_paragraph command. */
        if (strcmp(commands[i].command, "add_paragraph_after") == 0)
        {
            add_paragraph_after(doc, commands[i].paragraph_num);
        }

        /* Add_line command. */
        else if (strcmp(commands[i].command, "add_line_after") == 0)
        {
            line_content_size = strlen(commands[i].line_content);

            for (p = 1; p < line_content_size; ++p)
            {
                if (commands[i].line_content[p] != '\n')
                {
                    final_line[p - 1] = commands[i].line_content[p];
                } else {
                    break;
                    p++;
                }
            }
            final_line[p] = '\0';

            add_line_after(doc, commands[i].paragraph_num, commands[i].line_num, final_line);
        }

        /* Append_line command. */
        else if (strcmp(commands[i].command, "append_line") == 0)
        {
            line_content_size = strlen(commands[i].line_content);

            for (p = 1; p < line_content_size; ++p)
            {
                if (commands[i].line_content[p] != '\n')
                {
                    final_line[p - 1] = commands[i].line_content[p];
                }
            }
            final_line[p] = '\0';

            append_line(doc, commands[i].paragraph_num, final_line);
        }

        /* Print_document command. */
        else if (strcmp(commands[i].command, "print_document") == 0)
        {
            print_document(doc);
        }

        /* Quit_document Command. */
        else if (strcmp(commands[i].command, "quit") == 0)
        {
            break;
        }

        /* Exit_document Command. */
        else if (strcmp(commands[i].command, "exit") == 0)
        {
            break;
        }

        /* Remove_line Command. */
        else if (strcmp(commands[i].command, "remove_line") == 0)
        {
            remove_line(doc, commands[i].paragraph_num, commands[i].line_num);
        }

        /* Load_file Command. */
        else if (strcmp(commands[i].command, "load_file") == 0)
        {
            ++temp;
            if (temp == load_file_nums){
                for (j = load_file_nums - 1; j>=0; --j) {
                    load_file(doc, commands[load_file_idxs[j]].line_content);
                    
                }
            }
            
        }

        /* Replace_text Command. */
        else if (strcmp(commands[i].command, "replace_text") == 0)
        {
            replace_text(doc, commands[i].line_content, commands[i].replace_str);
        }

        /* Highlight_text Command. */
        else if (strcmp(commands[i].command, "highlight_text") == 0)
        {
            highlight_text(doc, commands[i].line_content);
        }

        /* Remove_text Command. */
        else if (strcmp(commands[i].command, "remove_text") == 0)
        {
            k = 1;
                    p = 0;
                    while (commands[i].line_content[k] != '\"')
                    {
                        temp1[p] = commands[i].line_content[k];
                        ++p;
                        ++k;
                    }

            temp1[p] = '\0';
            remove_text(doc, temp1);
        }

        /* Save_document. */
        else if (strcmp(commands[i].command, "save_document") == 0) {
            save_document(doc, commands[i].line_content);
        }

        /* Reset-document. */
        else if (strcmp(commands[i].command, "reset_document") == 0) {
            reset_document(doc);
        }

        /* Else print invalid message. */
        else {
            fprintf(stdout, commands[i].command);
        }
    }
}
