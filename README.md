# Caesar Cipher Encryption with MPI

This repository contains a parallel C program for implementing the Caesar Cipher Encryption algorithm using the Message Passing Interface (MPI).

## Description

The Caesar Cipher Encryption algorithm is a simple substitution cipher where each character in the message is replaced by the character three positions down the alphabet. This program allows users to either encrypt or decrypt a given string based on their choice.

## Modes of Operation

The program operates in two modes:

1. **Console Input Mode:**
   - The user provides input directly from the console.
   - After encryption or decryption, the result is printed on the console.

2. **File Input Mode:**
   - The program reads input from a file.
   - After encryption or decryption, the result is written to an output file.

## Input

- **String:** The input string to be encrypted or decrypted.

## Output

- **Encrypted or Decrypted String:** The result of the encryption or decryption process.

## Usage

1. **Clone the Repository:**
   ```
   git clone <repository-url>
   ```

2. **Compile the Program:**
   ```
   mpicc main.c -o caesar_cipher_mpi
   ```

3. **Execute the Program:**
   - **Console Input Mode:**
     ```
     mpirun -n <num-processes> ./caesar_cipher_mpi
     ```

   - **File Input Mode:**
     ```
     mpirun -n <num-processes> ./caesar_cipher_mpi input.txt output.txt
     ```
     Replace `input.txt` with the path to the input file and `output.txt` with the desired output file name.

4. **Follow the On-Screen Instructions:**
   - Choose whether to encrypt or decrypt the input string.
   - Enter the string to be encrypted or decrypted.
   - View the encrypted or decrypted result.

## Parallelization Scenario

- **Master Process:**
  - Reads the string from the user or file.
  - Splits the string and scatters it among the processes.
  - Gathers the encrypted or decrypted string portions from all processes.
  - Prints the final result.

- **Slave Processes:**
  - Receive their respective portions of the string from the master process.
  - Encrypt or decrypt their string portion.
  - Send the encrypted or decrypted portion back to the master process.

## Example

### Input (Console Input Mode)
```
Choose Operation:
1. Encrypt
2. Decrypt
Enter your choice: 1
Enter the string to encrypt: tutorial
```

### Output
```
Encrypted String: wxwruldo
```

---

This program provides a parallelized implementation of the Caesar Cipher Encryption algorithm, allowing for faster processing of large strings through parallel execution using MPI.

Feel free to explore the code, experiment with different input strings, and contribute improvements to enhance its functionality further.

For any questions or suggestions, please contact the repository owner. Enjoy encrypting and decrypting with MPI!

