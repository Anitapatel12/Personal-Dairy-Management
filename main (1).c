
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PASSWORD_LENGTH 20

// Structure for a diary entry
typedef struct DiaryEntry {
    char date[20];
    char entry[200];
    struct DiaryEntry* next;
} DiaryEntry;

// Global variables for password protection
char password[MAX_PASSWORD_LENGTH] = "password123";
bool authenticated = false;

// Function prototypes
void display_menu();
DiaryEntry* create_entry(const char* date, const char* entry);
void add_entry(DiaryEntry** head);
void view_entries(const DiaryEntry* head);
void search_entries(const DiaryEntry* head, const char* date);
void delete_entry(DiaryEntry** head, const char* date);
void free_entries(DiaryEntry* head);
void authenticate_user();
void change_password();

int main() {
    DiaryEntry* diary = NULL;
    int choice;
    char date[20];

    // Authenticate user before accessing the diary
    authenticate_user();

    do {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character left in the input buffer

        switch (choice) {
            case 1:
                add_entry(&diary);
                break;
            case 2:
                view_entries(diary);
                break;
            case 3:
                printf("Enter date to search (DD-MM-YYYY): ");
                fgets(date, sizeof(date), stdin);
                date[strcspn(date, "\n")] = '\0'; // Remove newline character
                search_entries(diary, date);
                break;
            case 4:
                printf("Enter date to delete (DD-MM-YYYY): ");
                fgets(date, sizeof(date), stdin);
                date[strcspn(date, "\n")] = '\0'; // Remove newline character
                delete_entry(&diary, date);
                break;
            case 5:
                change_password();
                break;
            case 6:
                // Exit the program
                printf("Exiting the program.\n");
                free_entries(diary);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 6);

    return 0;
}

// Function to display the menu
void display_menu() {
    printf("\n--- Personal Diary Management System ---\n");
    printf("1. Add Entry\n");
    printf("2. View All Entries\n");
    printf("3. Search Entry\n");
    printf("4. Delete Entry\n");
    printf("5. Change Password\n");
    printf("6. Exit\n");
}

// Function to create a new diary entry
DiaryEntry* create_entry(const char* date, const char* entry) {
    DiaryEntry* new_entry = (DiaryEntry*)malloc(sizeof(DiaryEntry));
    if (new_entry == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(new_entry->date, date);
    strcpy(new_entry->entry, entry);
    new_entry->next = NULL;
    return new_entry;
}

// Function to add a new entry to the diary
void add_entry(DiaryEntry** head) {
    char date[20];
    char entry[200];

    printf("Enter date (DD-MM-YYYY): ");
    fgets(date, sizeof(date), stdin);
    date[strcspn(date, "\n")] = '\0'; // Remove newline character

    printf("Enter diary entry: ");
    fgets(entry, sizeof(entry), stdin);
    entry[strcspn(entry, "\n")] = '\0'; // Remove newline character

    DiaryEntry* new_entry = create_entry(date, entry);

    if (*head == NULL) {
        *head = new_entry;
    } else {
        DiaryEntry* current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_entry;
    }
    printf("Entry added successfully.\n");
}

// Function to view all diary entries
void view_entries(const DiaryEntry* head) {
    if (head == NULL) {
        printf("No entries found.\n");
        return;
    }
    printf("Diary Entries:\n");
    int count = 1;
    while (head != NULL) {
        printf("%d. Date: %s\n   Entry: %s\n", count, head->date, head->entry);
        head = head->next;
        count++;
    }
}

// Function to search for a diary entry by date
void search_entries(const DiaryEntry* head, const char* date) {
    if (head == NULL) {
        printf("No entries found.\n");
        return;
    }
    int count = 1;
    while (head != NULL) {
        if (strcmp(head->date, date) == 0) {
            printf("Entry found at position %d:\n", count);
            printf("Date: %s\nEntry: %s\n", head->date, head->entry);
            return;
        }
        head = head->next;
        count++;
    }
    printf("No entry found for the given date.\n");
}

// Function to delete a diary entry by date
void delete_entry(DiaryEntry** head, const char* date) {
    if (*head == NULL) {
        printf("No entries found.\n");
        return;
    }
    DiaryEntry* current = *head;
    DiaryEntry* prev = NULL;
    while (current != NULL) {
        if (strcmp(current->date, date) == 0) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            printf("Entry deleted successfully.\n");
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("No entry found for the given date.\n");
}

// Function to free memory allocated for diary entries
void free_entries(DiaryEntry* head) {
    DiaryEntry* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// Function to authenticate the user
void authenticate_user() {
    char input_password[MAX_PASSWORD_LENGTH];
    printf("Enter password to access the diary: ");
    fgets(input_password, sizeof(input_password), stdin);
    input_password[strcspn(input_password, "\n")] = '\0'; // Remove newline character
    if (strcmp(input_password, password) == 0) {
        printf("Authentication successful. Access granted.\n");
        authenticated = true;
    } else {
        printf("Authentication failed. Access denied.\n");
        exit(1);
    }
}

// Function to change the password
// Function to change the password
void change_password() {
    if (!authenticated) {
        printf("You need to authenticate first to change the password.\n");
        return;
    }

    char new_password[MAX_PASSWORD_LENGTH];
    printf("Enter new password: ");
    fgets(new_password, sizeof(new_password), stdin);
    new_password[strcspn(new_password, "\n")] = '\0'; // Remove newline character
    strcpy(password, new_password);

    printf("Password changed successfully.\n");
    authenticated = false; // Reset authenticated flag after password change
     // Re-authenticate the user with the new password
      authenticate_user();
}

