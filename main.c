#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100

void create_record(const char *file_name) {
    char student_id[MAX_LENGTH];
    char name[MAX_LENGTH];
    char department[MAX_LENGTH];
    FILE *file;

    file = fopen(file_name, "a");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter ID: ");
    fgets(student_id, sizeof(student_id), stdin);
    student_id[strcspn(student_id, "\n")] = '\0'; // Remove newline character

    printf("Enter Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0'; // Remove newline character

    printf("Enter Department: ");
    fgets(department, sizeof(department), stdin);
    department[strcspn(department, "\n")] = '\0'; // Remove newline character

    fprintf(file, "%s, %s, %s\n", student_id, name, department);

    fclose(file);
}

void read_record(const char *file_name, const char *option) {
    char line[MAX_LENGTH];
    char search_id[MAX_LENGTH];
    char temp_line[MAX_LENGTH];
    FILE *file;

    file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    if (strcmp(option, "2.1\n") == 0) { // Adjusted to include newline character
        while (fgets(line, sizeof(line), file) != NULL) {
            printf("%s", line);
        }
    } else if (strcmp(option, "2.2\n") == 0) { // Adjusted to include newline character
        printf("Enter ID to search: ");
        fgets(search_id, sizeof(search_id), stdin);
        search_id[strcspn(search_id, "\n")] = '\0'; // Remove newline character
        int found = 0;
        while (fgets(line, sizeof(line), file) != NULL) {
            strcpy(temp_line, line); //Since it as changing the line character, created one more variable to for checking
            char *token = strtok(temp_line, ",");
            if (strcmp(token, search_id) == 0) {
                printf("%s", line);
                found = 1;
                break; // Exit loop after finding the first match
            }
        }
        if (!found) {
            printf("Record with ID %s not found.\n", search_id);
        }
    } else {
        printf("Invalid option.\n");
    }

    fclose(file);
}

void update_record(const char *file_name) {
    char line[MAX_LENGTH];
    char search_id[MAX_LENGTH];
    char new_name[MAX_LENGTH];
    char new_department[MAX_LENGTH];
    FILE *file, *temp_file;

    printf("Enter ID to update: ");
    fgets(search_id, sizeof(search_id), stdin);
    search_id[strcspn(search_id, "\n")] = '\0'; // Remove newline character

    file = fopen(file_name, "r");
    temp_file = fopen("temp.txt", "w");
    if (file == NULL || temp_file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    int found = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, search_id) != NULL) {
            char *token = strtok(line, ",");
            char prevName[100];
            strcpy(prevName, strtok(NULL, ", "));
            printf("Enter updated Name (Press Enter to keep '%s'): ", prevName);
            fgets(new_name, sizeof(new_name), stdin);
            new_name[strcspn(new_name, "\n")] = '\0'; // Remove newline character
            char prevDept[100];
            strcpy(prevDept, strtok(NULL, ", "));
            prevDept[strcspn(prevDept, "\n")] = '\0';
            printf("Enter updated Department (Press Enter to keep '%s'): ", prevDept);
            fgets(new_department, sizeof(new_department), stdin);
            new_department[strcspn(new_department, "\n")] = '\0'; // Remove newline character

            // Check if new_name or new_department are empty strings, use original values if so
            char *updated_name = strcspn(new_name, " \n") == 0 ? strtok(NULL, ",") : new_name;
            // Check if new_name or new_department are empty strings (including spaces and newline)
            char *updated_department = strcspn(new_department, " \n") == 0 ? strtok(NULL, ",") : new_department;
            if (updated_name == NULL && updated_department == NULL) {
                fprintf(temp_file, "%s, %s, %s\n", search_id, prevName, prevDept);
            } else if(updated_name != NULL && updated_department == NULL) {
                fprintf(temp_file, "%s, %s, %s\n", search_id, updated_name, prevDept);
            } else if(updated_name == NULL && updated_department != NULL){
                fprintf(temp_file, "%s, %s, %s\n", search_id, prevName, updated_department);
            } else {
                fprintf(temp_file, "%s, %s, %s\n", search_id, updated_name, updated_department);
            }
            // fprintf(temp_file, "%s, %s, %s\n", search_id, updated_name, updated_department);
            found = 1;
        } else {
            fprintf(temp_file, "%s", line);
        }
    }

    if (!found) {
        printf("Record with ID %s not found.\n", search_id);
    }

    fclose(file);
    fclose(temp_file);
    remove(file_name);
    rename("temp.txt", file_name);
}


void delete_record(const char *file_name) {
    char line[MAX_LENGTH];
    char search_id[MAX_LENGTH];
    FILE *file, *temp_file;

    printf("Enter ID to delete: ");
    fgets(search_id, sizeof(search_id), stdin);
    search_id[strcspn(search_id, "\n")] = '\0'; // Remove newline character

    file = fopen(file_name, "r");
    temp_file = fopen("temp.txt", "w");
    if (file == NULL || temp_file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    int found = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, search_id) == NULL) {
            fprintf(temp_file, "%s", line);
        } else {
            found = 1;
        }
    }

    if (!found) {
        printf("Record with ID %s not found.\n", search_id);
    }

    fclose(file);
    fclose(temp_file);
    remove(file_name);
    rename("temp.txt", file_name);
}

int main() {
    char file_name[] = "records.txt";
    char choice[MAX_LENGTH];

    while (1) {
        system("clear");

        printf("\nMenu:\n");
        printf("1. Create Record\n");
        printf("2. Read Record\n");
        printf("3. Update Record\n");
        printf("4. Delete Record\n");
        printf("5. Exit\n");

        printf("Enter your choice: ");
        fgets(choice, sizeof(choice), stdin);

        switch (choice[0]) {
            case '1':
                create_record(file_name);
                break;
            case '2':
                printf("2.1: Show all data\n2.2: Display data based on ID\nEnter option: ");
                fgets(choice, sizeof(choice), stdin);
                read_record(file_name, choice);
                printf("Please Press Enter to continue\n");
                getchar(); // Clear the newline character from the input buffer
                break;
            case '3':
                update_record(file_name);
                break;
            case '4':
                delete_record(file_name);
                break;
            case '5':
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice, please try again.\n");
        }
    }

    return 0;
}
