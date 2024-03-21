# include <stdio.h>
# include <mpi.h>
# include <string.h>

void encryptHelper(char sub[], int size);
void decryptHelper(char sub[], int size);
int readFromFile(char text[]);
void takeInput(char text[]);

int main(int argc, char *argv[]) {
    int processesNumber, rank, size, split, mode = 0, encrypt = 0;
    MPI_Status status;
    char test[100];

    // Initialize the MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &processesNumber);

    // scatter the string among all cores by master core
    if (rank == 0) {
        int check = 1;
        if (mode) check = readFromFile(test);
        else takeInput(test);

        if (!check) return 0;

        size = (int) strlen(test);
        double temp = (double)size / (processesNumber - 1);
        split = temp == (int)temp ? (int)temp : (int)temp + 1;

        for (int i = 1; i < processesNumber; ++i) {
            int start = (i - 1) * split, ind = 0, subSize = split + 1, end = start + split;
            char sub[subSize];
            for (; start < end && start < size; ++start) sub[ind++] = test[start];

            sub[ind] = '\0';

            MPI_Send(&ind, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&sub, ind, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    }

    if (rank != 0) {
        int subSize;
        MPI_Recv(&subSize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        char sub[subSize];
        MPI_Recv(&sub, subSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        sub[subSize] = '\0';

        if (encrypt) encryptHelper(sub, subSize);
        else decryptHelper(sub, subSize);

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

void decryptHelper(char sub[], int size) {
    for (int i = 0; i < size; ++i)
        sub[i] = (char)((int) sub[i] - 3);

}

void encryptHelper(char sub[], int size) {
    for (int i = 0; i < size; ++i)
        sub[i] = (char)((int) sub[i] + 3);
}

int readFromFile(char text[]){
    FILE *fptr;

    // Open a file in read mode
    fptr = fopen("text.txt", "r");

    // If the file exist
    if(fptr == NULL) {
        printf("Not able to open the file.\nPlease check the file is exist");
        fclose(fptr);
        return 0;
    }

    // Read the content and print it
    fgets(text, 100, fptr);

    // Close the file
    fclose(fptr);
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