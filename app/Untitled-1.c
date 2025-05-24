


void process_redirection(char *str) {

    char *cmd_part = strtok(str, ">");
    char *file_part = strtok(NULL, ">");
	int fd_num = file_Descriptor(str);

    if (file_part == NULL) {
        fprintf(stderr, "Syntax error: Missing file after `>`\n");
        return;
    }

    cmd_part = remove_extra_spaces(cmd_part);
    file_part = remove_extra_spaces(file_part);


    // Split command and arguments
    char *args[10];
    int argc = 0;
    char *token = strtok(cmd_part, " ");
    while (token != NULL && argc < 9) {
        args[argc++] = token;
        token = strtok(NULL, " ");
    }
    args[argc] = NULL;

    // Fork process
    pid_t pid = fork();
    if (pid == 0) { // Child process
        int fd = open(file_part, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror("Error opening file");
            exit(1);
        }

        dup2(fd, fd_num); // Redirect stdout to file
        close(fd);

        execvp(args[0], args);
        perror("exec failed");
        exit(1);
    } else if (pid > 0) {
        wait(NULL); // Parent waits for child process
    } else {
        perror("fork failed");
    }

}
