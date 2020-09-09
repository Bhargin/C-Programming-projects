/* 115983922 */

#include <stdio.h>
#include <string.h>
#include "command.h"
#include "executor.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>
#include <unistd.h>
#include <sysexits.h>
#include <stdlib.h>
#include <fcntl.h>

/* static void print_tree(struct tree *t); */
static void dup_func(struct tree *t);

/* An auxiliary function to dup input and output redirection.*/
static void dup_func(struct tree *t)
{
   int file_desc;

   /* Check if any input and output redirection entry is NULL. */
   if (t->input != NULL)
   {
      /* Open t->input for reading. Move forward if open is successful. */
      if ((file_desc = open(t->input, O_RDONLY)) < 0)
      {
         /* Error message if open is not successful. */
         perror("Cannot open file");
         exit(EX_OSERR);
      }

      /* Perform duping. */
      if (dup2(file_desc, STDIN_FILENO) < 0)
      {
         /* Error message if duping is not successful. */
         perror("dup2 (read) error");
         exit(EX_OSERR);
      }

      /* Close file directory. */
      close(file_desc);
   }

   /* Check if any input and output redirection entry is NULL. 
      The body of the following if() is the same as the one above 
      and therefore it will have similar comments. */
   if (t->output != NULL)
   {

      if ((file_desc = open(t->output, O_WRONLY | O_EXCL | O_CREAT, 0664)) < 0)
      {
         perror("Cannot open file for write.");
         exit(EX_OSERR);
      }
      if (dup2(file_desc, STDOUT_FILENO) < 0)
      {
         perror("dup2 (write) error");
         exit(EX_OSERR);
      }
      close(file_desc);
   }
}

/* Main execute method that traverses the tree for reading 
   commands. */
int execute(struct tree *t)
{
   int cdir, success, pipe_fd[2], ret_value;
   pid_t result, result2;

   /* Only traverse the tree is the root is not NULL (tree is not empty). */
   if (t != NULL)
   {
      /* NONE Conjunction. */
      if (t->conjunction == NONE)
      {

         /* Check for cd command first and execute it without 
            forking. */
         if (strcmp(t->argv[0], "cd") == 0)
         {
            /* cd to the directory at argv[1]. */
            if (t->argv[1] != NULL)
            {
               cdir = chdir(t->argv[1]);

               /* If cd command failed than print error message. */
               if (cdir == -1)
               {
                  perror("Cannot change directory");
                  return EX_USAGE;
               }
               return 0;
            }

            /* Else cd to home directory. */
            else
            {
               cdir = chdir(getenv("HOME"));
               return 0;
            }
         }

         /* Check for exit command first and execute it without 
            forking. */
         if (strcmp(t->argv[0], "exit") == 0)
         {
            exit(0);
         }

         /* Fork and perform any other command. */
         else
         {
            /* Fork process. */
            if ((result = fork()) < 0)
            {
               /* Print error message if forking fails. */
               perror("fork");
               exit(EX_OSERR);
            }

            /* Parent code. */
            if (result != 0)
            {
               /* Wait to reap child. Use int success to check if child completed it's 
                  task sucessfully. */
               wait(&success);

               /* If child completed sucessfully return 0 else return 1. */
               ret_value = (WEXITSTATUS(success) == 0 ?  0:1);
               return ret_value;
            }

            /* Child code. */
            else
            {
               /* Perform duping for input and output redirecton.*/
               dup_func(t);

               /* Perfrom task using execvp. */
               execvp(t->argv[0], t->argv);

               /* If any error occured we need to print to stderr and exit. */
               fprintf(stderr, "Failed to execute %s\n", t->argv[0]);
               fflush(stdout);
               exit(EX_OSERR);
            }
         }
      }

      /* PIPE Code. */
      if (t->conjunction == PIPE)
      {
         /* Create a Pipe first so that process can share it. */
         if (pipe(pipe_fd) < 0)
         {
            perror("Pipe Failed");
         }

         /* Fork process. */
         if ((result2 = fork()) < 0)
         {
            /* Print error message if forking fails. */
            perror("fork");
            exit(EX_OSERR);
         }

         /* Parent Code. Performs reading operation and therefore does not need 
            the write side of the pipe. */
         if (result2 != 0)
         {
            
            /* If there is no output redirection then exit process. */
            if (t->left->output != NULL)
            {
               printf("Ambiguous output redirect.\n");
            }

            /* If the process is not exited then go ahead and initiate the
               the parent to only perform readind through the pipe. 
               close(pipe_fd[1]) ensures that the parent only performs
               reading. */
            close(pipe_fd[1]);

            /* Perform duping so that the parent can read from input. */
            if (dup2(pipe_fd[0], STDIN_FILENO) < 0)
            {
               /* Error message. */
               perror("Failed to dup pipe.");
               exit(EX_OSERR);
            }

            /* Once the parent has finished reading, close the reading end. */
            close(pipe_fd[0]);

            /* Now excute the right node which stores the output redirection. 
               This step performs the writing bit through the pipe. This writing process
               will now go to the child. */
            execute(t->right);

            /* Reaping child and returning the status of right side. */
             wait(&success);

            /* If child completed sucessfully return 0 else return 1. */
            ret_value = (WEXITSTATUS(success) == 0 ?  0:1);
            return ret_value;
         }

         /* Child Code. Reponsible for writing from input/pipe. */
         else
         {
            /* If there is no input redirection then exit the process. */
            if (t->right->input != NULL)
            {
               /* Error message. */
               printf("Ambiguous input redirect.\n");
            }

            /* close the read end of the pipe so that processes don't 
               access the wrong side of the pipe. This process is only meant to write. */
            close(pipe_fd[0]);

            /* Perform duping for output redirection. */
            if (dup2(pipe_fd[1], STDOUT_FILENO) < 0)
            {
               /* Error message. */
               perror("Failed to dup pipe.");
               exit(EX_OSERR);
            }

            /* Close pipe end after finished using and passing data through it. */
            close(pipe_fd[1]);

            /* Execute the functionality required by the input before terminating process. */
            execute(t->left);
            exit(1);
         }
      } 

      /* AND Code. */
      if (t->conjunction == AND)
      {
         /* Perform duping for input and output redirection. */
         dup_func(t);

         /* Since the conjuction is AND, we always have to make sure the left 
            node/tree is true before processing the right node/tree. The if statement
            below will recursively check the t->left. We check against 0 because 0
            is returned if WEXITED(&success) was successful in the parent process code 
            for t->conjunction == NONE. See above. */
         if (execute(t->left) == 0)
         {
            /* This statement will recursively excute t->right
               if t->left is true and satisfies AND operator. */
            execute(t->right);
         }
      }
 
      /* SUBSHELL Code. The SUBSHELL method is very simple.
         It just initiates a process and uses the child to 
         excute the t->left or t->right. */
      if (t->conjunction == SUBSHELL)
      {

         /* Fork process. */
         if ((result = fork()) < 0)
         {
            /* Print error message if forking fails. */
            perror("fork");
            exit(EX_OSERR);
         }

         /* Parent code. */
         if (result != 0)
         {
            /* Wait to reap child. Use int success to check if child completed it's 
                  task sucessfully. */
            wait(&success);

            /* If child completed sucessfully return 0 else return 1. */
            ret_value = (WEXITSTATUS(success) == 0 ?  0:1);
            return ret_value;
         }

         /* Child code. */
         else
         {
            /* Perform duping for input and output redirecton.*/
            dup_func(t);

            /* Perform recursion/task and exit process. */
            execute(t->left);
            exit(0);
         }
      }
   }

   return 0;
}

/* static void print_tree(struct tree *t)
{
   if (t != NULL)
   {
      print_tree(t->left);

      if (t->conjunction == NONE)
      {
         printf("NONE: %s, ", t->argv[0]);
      }
      else
      {
         printf("%s, ", conj[t->conjunction]);
      }
      printf("IR: %s, ", t->input);
      printf("OR: %s\n", t->output);

      print_tree(t->right);
   }
} */
