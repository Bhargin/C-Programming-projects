#include <stdio.h>
#include "document.h"
#include <string.h>
#include <stdlib.h>

/* Method initializes a document with name. */
int init_document(Document *doc, const char *name)
{

    /* Length of Doc Name. */
    int name_length = strlen(name);

    /* Check if any parameter input is invalid. If true then return FAILURE. */
    if (doc == NULL || name == NULL || name_length > MAX_STR_SIZE)
    {
        return FAILURE;
    }

    /* If parameter inputs are valid then continure intitialization. 
     Reutrn SUCCESS */
    else
    {
        strcpy(doc->name, name);
        doc->name[name_length] = '\0';
        doc->number_of_paragraphs = 0;
        return SUCCESS;
    }
}

/* Method resets a document. */
int reset_document(Document *doc)
{
    /* Reutrn FAILURE if document is null. */
    if (doc == NULL)
    {
        return FAILURE;
    }
    /* Else return SUCCESS. */
    else
    {
        doc->number_of_paragraphs = 0;
        return FAILURE;
    }
}

/* Mehtod prints a document. */
int print_document(Document *doc)
{
    /* Int blank keep tracks of empty lines in paragraph. */
    int i, p, blank = 0;

    /* Return FAILURE if document is null. */
    if (doc == NULL)
    {
        return FAILURE;
    }

    /* Else return SUCCESS. */
    else
    {
        printf("Document name: \"%s\"\n", doc->name);
        printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

        /* For-Loop iterates over each element of documents paragraph array. */
        for (i = 0; i < doc->number_of_paragraphs; ++i)
        {
            blank = 0;

            /* For-Loop iterates over each paragraph line and prints it. */
            for (p = 0; p <= doc->paragraphs[i].number_of_lines; ++p)
            {
                if (strcmp(doc->paragraphs[i].lines[p], "") != 0)
                {
                    printf("%s\n", doc->paragraphs[i].lines[p]);
                }
                else
                {
                    blank += 1;
                }
            }

            /* Enter another blankspace when starting a new paragraph. */
            if (i < doc->number_of_paragraphs - 1)
            {
                printf("\n");
            }
        }
        return SUCCESS;
    }
}

/* Method adds paragraph to the document. */
int add_paragraph_after(Document *doc, int paragraph_number)
{

    /* The conditonal statements below check whether any parameter input is invalid.
        If there is a invalid parameter than FAILURE will be returned. */
    if (doc == NULL)
    {
        fprintf(stdout, "add_paragraph_after failed\n");
        return FAILURE;
    }
    else if (doc->number_of_paragraphs >= MAX_PARAGRAPHS || paragraph_number >= MAX_PARAGRAPHS)
    {
        fprintf(stdout, "add_paragraph_after failed\n");
        return FAILURE;
    }
    else if (paragraph_number > doc->number_of_paragraphs)
    {
        fprintf(stdout, "add_paragraph_after failed\n");
        return FAILURE;

        /* This conditonal statment will be executed if all inputs are valid. 
    SUCCESS will be returned. */
    }
    else
    {
        doc->number_of_paragraphs += 1;
        doc->paragraphs[paragraph_number].number_of_lines = 0;
        return SUCCESS;
    }
}

/* Method adds lines to a specified paragraph in document. */
int add_line_after(Document *doc, int paragraph_number, int line_number, const char *new_line)
{

    /* Check if any parameters are null or invalid. Reutrn FAILURE if true. */
    if (doc == NULL || new_line == NULL)
    {
        fprintf(stdout, "add_line_after failed\n");
        return FAILURE;
    }
    else if (doc->number_of_paragraphs >= MAX_PARAGRAPHS ||
             doc->paragraphs[paragraph_number - 1].number_of_lines >= MAX_PARAGRAPH_LINES)
    {
        fprintf(stdout, "add_line_after failed\n");
        return FAILURE;
    }
    else if (line_number > doc->paragraphs[paragraph_number - 1].number_of_lines || paragraph_number > doc->number_of_paragraphs)
    {
        fprintf(stdout, "add_line_after failed\n");
        return FAILURE;
        /* This conditonal statment will be executed if all inputs are valid. 
    SUCCESS will be returned. */
    }
    else
    {
        strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number], new_line);
        doc->paragraphs[paragraph_number - 1].number_of_lines += 1;
        return SUCCESS;
    }
}

/* Method returns number of lines in a paragraph. */
int get_number_lines_paragraph(Document *doc, int paragraph_number, int *number_of_lines)
{
    if (doc == NULL || number_of_lines == NULL)
    {
        return FAILURE;
    }
    else if (paragraph_number > MAX_PARAGRAPHS || paragraph_number <= 0 || paragraph_number > doc->number_of_paragraphs)
    {
        return FAILURE;
    }
    else
    {
        *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
        return SUCCESS;
    }
}

/* Method appends line to paragraph. */
int append_line(Document *doc, int paragraph_number, const char *new_line)
{
    /* Check if any parameters are null or invalid. Reutrn FAILURE if true. */
    if (doc == NULL)
    {
        fprintf(stdout, "append_line failed\n");
        return FAILURE;
    }
    else if (doc->number_of_paragraphs >= MAX_PARAGRAPHS ||
             doc->paragraphs[paragraph_number - 1].number_of_lines >= MAX_PARAGRAPH_LINES)
    {
        fprintf(stdout, "append_line failed\n");
        return FAILURE;
    }
    else if (paragraph_number > doc->number_of_paragraphs)
    {
        fprintf(stdout, "append_line failed\n");
        return FAILURE;
    }

    /* This conditonal statment will be executed if all inputs are valid. 
    SUCCESS will be returned. */
    else
    {
        /* Copy new_line to the end of the paragraph. */
        strcpy(doc->paragraphs[paragraph_number - 1]
                   .lines[doc->paragraphs[paragraph_number - 1].number_of_lines],
               new_line);
        doc->paragraphs[paragraph_number - 1].number_of_lines += 1;
        return SUCCESS;
    }
}

/* Method removes a line from the specified paragraph. */
int remove_line(Document *doc, int paragraph_number, int line_number)
{
    /* Return FAILURE if any parameters are invalid. */
    if (doc == NULL)
    {
        fprintf(stdout, "remove_line failed\n");
        return FAILURE;
    }
    else if (paragraph_number > doc->number_of_paragraphs || line_number > doc->paragraphs[paragraph_number - 1].number_of_lines)
    {
        fprintf(stdout, "remove_line failed\n");
        return FAILURE;
    }
    else if (doc->number_of_paragraphs >= MAX_PARAGRAPHS || doc->paragraphs[paragraph_number - 1].number_of_lines >= MAX_PARAGRAPH_LINES)
    {
        fprintf(stdout, "remove_line failed\n");
        return FAILURE;
    }

    /* Else return SUCCESS. */
    else
    {
        /* Remove line at line_number by copying a empty string to the line_number. */
        strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number - 1], "");
        doc->paragraphs[paragraph_number - 1].number_of_lines -= 1;
        return SUCCESS;
    }
}

/* Method loads data to the document. */
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines)
{
    /* Int index and line keep track of the current paragraph number
    and the current line number. */
    int p, index, line = 0;
    index = 0;

    /* Return FAILURE if any parameters are invalid. */
    if (doc == NULL || data == NULL)
    {
        return FAILURE;
    }
    else if (data_lines == 0)
    {
        return FAILURE;
    }

    /* Else return SUCCESS. */
    else
    {
        /* Initialize first paragraph and line. */
        doc->number_of_paragraphs += 1;
        doc->paragraphs[0].number_of_lines = 0;

        /* Loop-over the data arrary until desired data_line is reached. */
        for (p = 0; p < data_lines; ++p)
        {
            /* If there is a empty line then start a new paragraph. */
            if (strcmp(data[p], "") == 0)
            {
                /* Re-initialize variables as if a new paragraph was intialized. */
                ++index;
                doc->number_of_paragraphs += 1;
                doc->paragraphs[index].number_of_lines = 0;
                line = 0;
            }

            /* Else copy the current data line to doc. */
            else
            {
                strcpy(doc->paragraphs[index].lines[line], data[p]);
                ++line;
                doc->paragraphs[index].number_of_lines += 1;
            }
        }
        return SUCCESS;
    }
}

/* Method replaces text in doc. */
int replace_text(Document *doc, const char *target, const char *replacement)
{
    /* All int variables either are used for loops or to store length of target
    , replacement, or array indexes. The names explains the purpose of
    variable. */
    int num_of_paragraphs, num_of_lines, i, p, j, k, l, target_length, rep_length,
        line_length, start_idx, end_idx, range_arr_idx, rep_arr_idx;

    /* These string arrays copy the original lines from the document
    and store the final replaced string lines so that the doc isn't altered 
    accidently. */
    char copy[MAX_STR_SIZE + 1], range_arr[MAX_STR_SIZE + 1], final[MAX_STR_SIZE + 1];

    target_length = strlen(target);

    /* Return FAILURE if any parameter is invalid. */
    if (doc == NULL || target == NULL || replacement == NULL)
    {
        fprintf(stdout, "replace_text failed\n");
        return FAILURE;
    }

    /* Else return SUCCESS. */
    else
    {
        num_of_paragraphs = doc->number_of_paragraphs;
        rep_length = strlen(replacement);

        /* Loop over each paragraph of the doc. */
        for (i = 0; i < num_of_paragraphs; ++i)
        {
            num_of_lines = doc->paragraphs[i].number_of_lines;

            /* For-Each line in paragraph. */
            for (p = 0; p < num_of_lines; ++p)
            {
                /* Copy current line to the copy array for further processing. */
                strcpy(copy, doc->paragraphs[i].lines[p]);
                line_length = strlen(copy);

                /* start_idx and end_idx represent the number of character we need to search for
                (target lenght). */
                start_idx = 0;
                end_idx = start_idx + (target_length - 1);

                /* Iterate until the end of line is reached. */
                while (end_idx < line_length)
                {
                    range_arr_idx = 0;

                    /* Copy target letters into range_arr to be compared to target string. */
                    for (j = start_idx; j <= end_idx; ++j)
                    {
                        range_arr[range_arr_idx] = copy[j];
                        ++range_arr_idx;
                    }
                    range_arr[range_arr_idx] = '\0';

                    /* Compare range_arr to target string to see if they match. */
                    if (strcmp(range_arr, target) == 0)
                    {
                        /* rep_arr_idx is used to copy replacement characters to the final line. 
                        This variable refers to the indexes of the replacement array.*/
                        rep_arr_idx = 0;
                        l = start_idx + rep_length;

                        /* Start the replacing procedure. Copy the origianl line up till the target word. */
                        for (k = 0; k < start_idx; ++k)
                        {
                            final[k] = copy[k];
                        }

                        /* Replace the target word with the replacement word. */
                        for (k = start_idx; k <= start_idx + (rep_length - 1); ++k)
                        {
                            final[k] = replacement[rep_arr_idx];
                            ++rep_arr_idx;
                        }

                        /* Copy the original line as it was after the target word on the final line. */
                        for (k = end_idx + 1; k < line_length; ++k)
                        {
                            final[l] = copy[k];
                            ++l;
                        }

                        final[l] = '\0';

                        /* Copy final line to the doc and to the copy array so that the new line can be 
                        looped over again. This aids in finding multiple target instances on the line. */
                        strcpy(doc->paragraphs[i].lines[p], final);
                        strcpy(copy, final);
                    }

                    /* If target is not found then move forward by one letter and 
                    re-compare the new word agaisnt target. */
                    ++start_idx;
                    ++end_idx;
                }
            }
        }

        return SUCCESS;
    }
}

/* Method highlights target text in doc. */
int highlight_text(Document *doc, const char *target)
{

    /* All int variables either are used for loops or to store length of target
    , replacement, or array indexes. The names explains the purpose of
    variable. */
    int num_of_paragraphs, num_of_lines, i, p, j, k, l, target_length, line_length,
        start_idx, end_idx, range_arr_idx;

    /* These string arrays copy the original lines from the document
    and store the final replaced string lines so that the doc isn't altered 
    accidently. */
    char copy[MAX_STR_SIZE + 1], range_arr[MAX_STR_SIZE + 1], final[MAX_STR_SIZE + 1];

    target_length = strlen(target);

    /* Return FAILURE if parameters are null. */
    if (doc == NULL || target == NULL)
    {
        return FAILURE;
    }

    /* Else return SUCCESS. */
    else
    {
        num_of_paragraphs = doc->number_of_paragraphs;

        /* Loop over each paragraph in doc. */
        for (i = 0; i < num_of_paragraphs; ++i)
        {
            num_of_lines = doc->paragraphs[i].number_of_lines;

            /* For-Each line in paragraph. */
            for (p = 0; p < num_of_lines; ++p)
            {
                /* Copy current line to the copy array for further processing. */
                strcpy(copy, doc->paragraphs[i].lines[p]);
                line_length = strlen(copy);

                /* start_idx and end_idx represent the number of character we need to search for
                (target lenght). */
                start_idx = 0;
                end_idx = start_idx + (target_length - 1);
                l = start_idx + (target_length - 1);

                /* Iterate until the end of line is reached. */
                while (end_idx < line_length)
                {
                    range_arr_idx = 0;

                    /* Copy target letters into range_arr to be compared to target string. */
                    for (j = start_idx; j <= end_idx; ++j)
                    {
                        range_arr[range_arr_idx] = copy[j];
                        ++range_arr_idx;
                    }
                    range_arr[range_arr_idx] = '\0';

                    /* Compare range_arr to target string to see if they match. */
                    if (strcmp(range_arr, target) == 0)
                    {

                        /* Start the replacing procedure. Copy the origianl line in to the final
                         array up till the target word. */
                        for (k = 0; k < start_idx; ++k)
                        {
                            final[k] = copy[k];
                        }

                        /* Add hightlight mark at the beginning of the target word. */
                        final[start_idx] = *HIGHLIGHT_START_STR;

                        /* Continue copying the original string after the highlight mark 
                        until the end of target word is not reached. */
                        for (k = start_idx + 1; k <= (start_idx + 1) + (target_length - 1); ++k)
                        {
                            final[k] = copy[k - 1];
                        }

                        /* Add highlight mark at the end of the target word. */
                        final[start_idx + 1 + target_length] = *HIGHLIGHT_END_STR;

                        /* Continue coying the rest the origianl string as it was. */
                        for (k = (start_idx + target_length + 2); k <= line_length + 2; ++k)
                        {
                            final[k] = copy[++l];
                        }

                        final[line_length + 3] = '\0';

                        /* Copy final line to the doc and to the copy array so that the new line can be 
                        looped over again. This aids in finding multiple target instances on the line. */
                        strcpy(doc->paragraphs[i].lines[p], final);
                        strcpy(copy, final);

                        /* Don't loop over from the beginning. Instead start re-checking new line
                        after the end highlight mark.  */
                        start_idx = start_idx + target_length + 2;
                        end_idx = start_idx + (target_length - 1);
                        l = start_idx + (target_length - 1);
                        line_length = strlen(copy);
                    }

                    /* If target is not found then move forward by one letter and 
                    re-check the new word against target. */
                    else
                    {
                        ++start_idx;
                        ++end_idx;
                        ++l;
                    }
                }
            }
        }
        return SUCCESS;
    }
}

/* Remove text from doc. */
int remove_text(Document *doc, const char *target)
{
    /* All int variables either are used for loops or to store length of target
    , replacement, or array indexes. The names explains the purpose of
    variable. */
    int num_of_paragraphs, num_of_lines, i, p, j, k, l, target_length, line_length,
        start_idx, end_idx, range_arr_idx;

    /* These string arrays copy the original lines from the document
    and store the final replaced string lines so that the doc isn't altered 
    accidently. */
    char copy[MAX_STR_SIZE + 1], range_arr[MAX_STR_SIZE + 1], final[MAX_STR_SIZE + 1];

    target_length = strlen(target);

    /* Return FAILURE if any parameter is invailid. */
    if (doc == NULL || target == NULL)
    {
        return FAILURE;
    }

    /* Else return SUCCESS. */
    else
    {
        num_of_paragraphs = doc->number_of_paragraphs;

        /* For-Each paragraph in doc. */
        for (i = 0; i < num_of_paragraphs; ++i)
        {
            num_of_lines = doc->paragraphs[i].number_of_lines;

            /* For-Each line in paragraph. */
            for (p = 0; p < num_of_lines; ++p)
            {
                /* Copy current line to the copy array for further processing. */
                strcpy(copy, doc->paragraphs[i].lines[p]);
                line_length = strlen(copy);

                /* start_idx and end_idx represent the number of character we need to search for
                (target lenght). */
                start_idx = 0;
                end_idx = start_idx + (target_length - 1);
                l = 0;

                /* Iterate until the end of line is reached. */
                while (end_idx < line_length)
                {
                    range_arr_idx = 0;

                    /* Copy target letters into range_arr to be compared to target string. */
                    for (j = start_idx; j <= end_idx; ++j)
                    {
                        range_arr[range_arr_idx] = copy[j];
                        ++range_arr_idx;
                    }
                    range_arr[range_arr_idx] = '\0';

                    /* Compare range_arr to target string to see if they match. */
                    if (strcmp(range_arr, target) == 0)
                    {

                        /* Copy the original string into the final array until target word is 
                        reached. */
                        for (k = 0; k < start_idx; ++k)
                        {
                            final[k] = copy[k];
                            ++l;
                        }

                        /* Resume copying the original string into the final array but skip the target word.
                        Start at the end of the target word. */
                        for (k = end_idx + 1; k < line_length; ++k)
                        {
                            final[l] = copy[k];
                            ++l;
                        }

                        final[l] = '\0';

                        /* Copy final line to the doc and to the copy array so that the new line can be 
                        looped over again. This aids in finding multiple target instances on the line. */
                        strcpy(doc->paragraphs[i].lines[p], final);
                        strcpy(copy, final);
                        l = 0;
                        line_length = strlen(copy);
                    }

                    /* If target is not found then move forward by one letter and 
                    re-check the new word against target. */
                    else
                    {
                        ++start_idx;
                        ++end_idx;
                    }
                }
            }
        }
        return SUCCESS;
    }
}

/* Load file from file with filename. */
int load_file(Document *doc, const char *filename)
{
    FILE *input;
    char line[MAX_STR_SIZE + 1], final_line[MAX_STR_SIZE + 1];

    /* Int index and line keep track of the current paragraph number
    and the current line number. */
    int p, index, line_content_size, line_num;

    /* Return FAILURE if any parameters are invalid. */
    if (doc == NULL || filename == NULL)
    {
        fprintf(stdout, "load_file failed\n");
        return FAILURE;
    }
    else if ((input = fopen(filename, "r")) == NULL)
    {
        fprintf(stdout, "load_file failed\n");
        return FAILURE;
    }

    /* Else return SUCCESS. */
    else
    {
        /* Initialize first paragraph and line if doc is uninitialzied. */
        if (doc->number_of_paragraphs == 0)
        {
            doc->number_of_paragraphs += 1;
            doc->paragraphs[0].number_of_lines = 0;
            index = 0;
            line_num = 0;
        }

        /* Else continue adding data at the end of doc is document was
        already loaded with data. */
        else
        {
            index = doc->number_of_paragraphs;
            doc->paragraphs[doc->number_of_paragraphs].number_of_lines = 0;
            doc->number_of_paragraphs += 1;
            line_num = 0;
        }

        /* Read input from file until end of file is reached. */
        fgets(line, MAX_STR_SIZE + 1, input);
        while (!feof(input))
        {

            /* If there is a empty line then start a new paragraph. */
            if (line[0] == '\n' || line[0] == ' ')
            {
                /* Re-initialize variables as if a new paragraph was intialized. */
                ++index;
                doc->number_of_paragraphs += 1;
                doc->paragraphs[index].number_of_lines = 0;
                line_num = 0;
            }

            /* Else copy the current data line to doc. */
            else
            {
                line_content_size = strlen(line);

                for (p = 0; p < line_content_size - 1; ++p)
                {

                    final_line[p] = line[p];
                }
                final_line[p] = '\0';
                strcpy(doc->paragraphs[index].lines[line_num], final_line);
                ++line_num;
                doc->paragraphs[index].number_of_lines += 1;
            }

            fgets(line, MAX_STR_SIZE + 1, input);
        }
        fclose(input);
        return SUCCESS;
    }
}

/* Save document to file. */
int save_document(Document *doc, const char *filename)
{

    int i, p;
    FILE *output_stream;
    output_stream = fopen(filename, "w");

    /* Return FAILURE if document is null. */
    if (doc == NULL || filename == NULL)
    {
        fprintf(stdout, "save_document failed\n");
        return FAILURE;
    }
    else if (output_stream == NULL)
    {
        fprintf(stdout, "save_document failed\n");
        return FAILURE;
    }

    /* Else return SUCCESS. */
    else
    {
        /* For-Loop iterates over each element of documents paragraph array. */
        for (i = 0; i < doc->number_of_paragraphs; ++i)
        {
            /* For-Loop iterates over each paragraph line and prints it. */
            for (p = 0; p <= doc->paragraphs[i].number_of_lines; ++p)
            {
                fputs(doc->paragraphs[i].lines[p], output_stream);
                fputs("\n", output_stream);
            }
        }
        fclose(output_stream);
        return SUCCESS;
    }
}
