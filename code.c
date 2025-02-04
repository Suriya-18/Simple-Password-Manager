#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// Structure to store password details
struct Password {
    char website[50];
    char username[50];
    char password[50]; // Storing encrypted passwords
};

// Function prototypes
void addPassword();
void viewPasswords();
void searchPassword();
void deletePassword();
void encrypt(char *str);
void decrypt(char *str);

int main() {
    int choice;
    while (1) {
        printf("\n==== Password Manager ====\n");
        printf("1. Add Password\n");
        printf("2. View Passwords\n");
        printf("3. Search Password\n");
        printf("4. Delete Password\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear input buffer

        switch (choice) {
            case 1: addPassword(); break;
            case 2: viewPasswords(); break;
            case 3: searchPassword(); break;
            case 4: deletePassword(); break;
            case 5: printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

// Function to encrypt a password (simple XOR cipher)
void encrypt(char *str) {
    while (*str) {
        *str = *str ^ 0xAA; // XOR encryption key
        str++;
    }
}

// Function to decrypt a password
void decrypt(char *str) {
    while (*str) {
        *str = *str ^ 0xAA; // XOR decryption key (same as encryption)
        str++;
    }
}

// Function to add a new password entry
void addPassword() {
    FILE *file = fopen("passwords.dat", "ab");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    struct Password p;
    printf("Enter website: ");
    fgets(p.website, sizeof(p.website), stdin);
    p.website[strcspn(p.website, "\n")] = '\0'; // Remove newline

    printf("Enter username: ");
    fgets(p.username, sizeof(p.username), stdin);
    p.username[strcspn(p.username, "\n")] = '\0';

    printf("Enter password: ");
    fgets(p.password, sizeof(p.password), stdin);
    p.password[strcspn(p.password, "\n")] = '\0';

    encrypt(p.password); // Encrypt password before storing
    fwrite(&p, sizeof(struct Password), 1, file);
    fclose(file);
    printf("Password saved successfully!\n");
}

// Function to view all stored passwords
void viewPasswords() {
    FILE *file = fopen("passwords.dat", "rb");
    if (file == NULL) {
        printf("No records found!\n");
        return;
    }

    struct Password p;
    printf("\n--- Saved Passwords ---\n");
    while (fread(&p, sizeof(struct Password), 1, file)) {
        decrypt(p.password); // Decrypt before displaying
        printf("Website: %s\nUsername: %s\nPassword: %s\n", p.website, p.username, p.password);
        encrypt(p.password); // Re-encrypt after displaying
        printf("-------------------------\n");
    }
    fclose(file);
}

// Function to search for a password by website
void searchPassword() {
    FILE *file = fopen("passwords.dat", "rb");
    if (file == NULL) {
        printf("No records found!\n");
        return;
    }

    char searchWebsite[50];
    struct Password p;
    int found = 0;

    printf("Enter website to search: ");
    fgets(searchWebsite, sizeof(searchWebsite), stdin);
    searchWebsite[strcspn(searchWebsite, "\n")] = '\0';

    while (fread(&p, sizeof(struct Password), 1, file)) {
        if (strcmp(p.website, searchWebsite) == 0) {
            decrypt(p.password);
            printf("Record Found!\nWebsite: %s\nUsername: %s\nPassword: %s\n", p.website, p.username, p.password);
            encrypt(p.password);
            found = 1;
            break;
        }
    }
    if (!found) printf("Website not found!\n");
    fclose(file);
}

// Function to delete a password entry
void deletePassword() {
    FILE *file = fopen("passwords.dat", "rb");
    if (file == NULL) {
        printf("No records found!\n");
        return;
    }

    FILE *tempFile = fopen("temp.dat", "wb");
    if (tempFile == NULL) {
        printf("Error opening temporary file!\n");
        fclose(file);
        return;
    }

    char deleteWebsite[50];
    struct Password p;
    int found = 0;

    printf("Enter website to delete: ");
    fgets(deleteWebsite, sizeof(deleteWebsite), stdin);
    deleteWebsite[strcspn(deleteWebsite, "\n")] = '\0';

    while (fread(&p, sizeof(struct Password), 1, file)) {
        if (strcmp(p.website, deleteWebsite) != 0) {
            fwrite(&p, sizeof(struct Password), 1, tempFile);
        } else {
            found = 1;
        }
    }
    
    fclose(file);
    fclose(tempFile);
    remove("passwords.dat");
    rename("temp.dat", "passwords.dat");

    if (found) {
        printf("Password entry deleted successfully!\n");
    } else {
        printf("Website not found!\n");
    }
}
