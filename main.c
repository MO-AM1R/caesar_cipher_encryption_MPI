# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <stdio.h>
# include <mpi.h>

void encryptHelper(char sub[], int size, int encrypt);
int  selectChoice(int* choice, char output[]);
int  readFromFile(char text[]);
void takeInput(char text[]);

int main(int argc, char *argv[]) {
    int processesNumber, rank, size, split, mode, encrypt;
    MPI_Status status;
    char test[100];

    // Initialize the MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &processesNumber);

    // scatter the string among all cores by master core
    if (rank == 0) {
        int check = selectChoice(&mode, "\n1- File input mode\n2- Console input mode\nEnter the mode -> ");
        if (!check) {
            printf("Invalid Input");
            return 0;
        }

        if (mode == 1) {
            if (!readFromFile(test)) return 0;
        }
        else takeInput(test);

        check = selectChoice(&encrypt, "\n1- Encrypt the text\n2- Decrypt the text\nEnter the choice -> ");
        if (!check) {
            printf("Invalid Input");
            return 0;
        }

        size = (int) strlen(test);
        double temp = (double)size / (processesNumber - 1);
        split = temp == (int)temp ? (int)temp : (int)temp + 1;

        for (int i = 1; i < processesNumber; ++i) {
            int start = (i - 1) * split, ind = 0, subSize = split + 1, end = start + split;
            char sub[subSize];
            for (; start < end && start < size; ++start) sub[ind++] = test[start];

            sub[ind] = '\0';

            MPI_Send(&ind, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&encrypt, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&sub, ind, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    }

    if (rank != 0) {
        int subSize;
        MPI_Recv(&subSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&encrypt, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        char sub[subSize];
        MPI_Recv(&sub, subSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        sub[subSize] = '\0';

        encryptHelper(sub, subSize, encrypt);

        MPI_Send(&subSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&sub, subSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    } else {
        int ind = 0;
        char answer[size];
        for (int source = 1; source < processesNumber; ++source) {
            int subSize;
            MPI_Recv(&subSize, 1, MPI_INT, source, 0, MPI_COMM_WORLD, &status);

            char sub[subSize];
            MPI_Recv(&sub, subSize, MPI_CHAR, source, 0, MPI_COMM_WORLD, &status);
            sub[subSize] = '\0';

            for (int i = 0; i < subSize; ++i) answer[ind++] = sub[i];
            answer[ind] = '\0';
        }

        printf("%s\n", answer);
    }

    MPI_Finalize();
    return 0;
}

void encryptHelper(char sub[], int size, int encrypt) {
    int shift = 3;
    if (encrypt != 1) shift = -3;

    for (int i = 0; i < size; ++i)
        sub[i] = (char)((int) sub[i] + shift);
}

int readFromFile(char text[]){
    FILE* filePtr;

    // Open a file in read mode
    filePtr = fopen("text.txt", "r");

    // If the file exist
    if(filePtr == NULL) {
        printf("Not able to open the file.\nPlease check the file is exist");
        fclose(filePtr);
        return 0;
    }

    // Read the content and print it
    fgets(text, 100, filePtr);

    // Close the file
    fclose(filePtr);
    return 1;
}

void takeInput(char text[]){
    enum { INPUT_SIZE = 100};
    printf("Enter the text -> ");
    // To make stdout unbuffered
    fflush(stdout);

    // store the input int input variable
    fgets(text, INPUT_SIZE, stdin);
}

int selectChoice(int* choice, char output[]){
    char input[100];
    char* ptr;

    printf("%s", output);
    // To make stdout unbuffered
    fflush(stdout);

    fgets(input, 100, stdin);
    *choice = (int)strtol(input, &ptr, 10);

    return (*choice == INT_MAX || *choice == INT_MIN || (*choice != 1 && *choice != 2)) ? 0 : 1;
}