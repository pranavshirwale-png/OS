/*
* Address Book using File Handling (C)
* Options: a) Create b) View c) Insert d) Delete e) Modify f) Exit
* Records are stored in a binary file "addressbook.dat".
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILENAME "addressbook.dat"
typedef struct {
 int id;
 char name[40];
 char phone[15];
 char email[40];
} Record;
/* read a line safely, strip newline */
void readLine(const char *prompt, char *buf, int size) {
 printf("%s", prompt);
 if (fgets(buf, size, stdin)) buf[strcspn(buf, "\n")] = '\0';
}
int readInt(const char *prompt) {
 char tmp[32];
 readLine(prompt, tmp, sizeof tmp);
 return atoi(tmp);
}
/* a) Create address book (creates/empties the file) */
void createBook(void) {
 FILE *fp = fopen(FILENAME, "wb");
 if (!fp) { printf("Error: cannot create file.\n"); return; }
 fclose(fp);
 printf("Address book created successfully.\n");
}
/* b) View address book */
void viewBook(void) {
 FILE *fp = fopen(FILENAME, "rb");
 Record r;
 int count = 0;
 if (!fp) { printf("Address book not found. Create it first (option a).\n"); return; }
 printf("\n%-5s %-20s %-14s %-25s\n", "ID", "Name", "Phone", "Email");
 printf("------------------------------------------------------------------\n");
 while (fread(&r, sizeof r, 1, fp) == 1) {
 printf("%-5d %-20s %-14s %-25s\n", r.id, r.name, r.phone,
r.email);
 count++;
 }
 if (count == 0) printf("(no records)\n");
 fclose(fp);
}
/* c) Insert a record */
void insertRecord(void) {
 FILE *fp = fopen(FILENAME, "ab+");
 Record r, t;
 if (!fp) { printf("Address book not found. Create it first (option a).\n"); return; }
 r.id = readInt("Enter ID: ");
 rewind(fp);
 while (fread(&t, sizeof t, 1, fp) == 1) {
 if (t.id == r.id) {
 printf("A record with ID %d already exists.\n", r.id);
 fclose(fp);
 return;
 }
 }
 readLine("Enter Name : ", r.name, sizeof r.name);
 readLine("Enter Phone: ", r.phone, sizeof r.phone);
 readLine("Enter Email: ", r.email, sizeof r.email);
 fseek(fp, 0, SEEK_END);
 fwrite(&r, sizeof r, 1, fp);
 fclose(fp);
 printf("Record inserted.\n");
}
/* d) Delete a record */
void deleteRecord(void) {
 FILE *fp = fopen(FILENAME, "rb");
 FILE *tmp;
 Record r;
 int id, found = 0;
 if (!fp) { printf("Address book not found. Create it first (option a).\n"); return; }
 id = readInt("Enter ID to delete: ");
 tmp = fopen("temp.dat", "wb");
 while (fread(&r, sizeof r, 1, fp) == 1) {
 if (r.id == id) found = 1;
 else fwrite(&r, sizeof r, 1, tmp);
 }
 fclose(fp);
 fclose(tmp);
 remove(FILENAME);
 rename("temp.dat", FILENAME);
 printf(found ? "Record deleted.\n" : "Record not found.\n");
}
/* e) Modify a record */
void modifyRecord(void) {
 FILE *fp = fopen(FILENAME, "rb+");
 Record r;
 int id, found = 0;
 if (!fp) { printf("Address book not found. Create it first (option a).\n"); return; }
 id = readInt("Enter ID to modify: ");
 while (fread(&r, sizeof r, 1, fp) == 1) {
 if (r.id == id) {
 found = 1;
 printf("Enter new details (ID stays %d)\n", id);
 readLine("New Name : ", r.name, sizeof r.name);
 readLine("New Phone: ", r.phone, sizeof r.phone);
 readLine("New Email: ", r.email, sizeof r.email);
 fseek(fp, -(long)sizeof r, SEEK_CUR);
 fwrite(&r, sizeof r, 1, fp);
 break;
 }
 }
 fclose(fp);
 printf(found ? "Record modified.\n" : "Record not found.\n");
}
int main(void) {
 char choice[8];
 for (;;) {
 printf("\n===== ADDRESS BOOK =====\n");
 printf("a) Create address book\n");
 printf("b) View address book\n");
 printf("c) Insert a record\n");
 printf("d) Delete a record\n");
 printf("e) Modify a record\n");
 printf("f) Exit\n");
 readLine("Enter your choice: ", choice, sizeof choice);
 switch (choice[0]) {
 case 'a': case 'A': createBook(); break;
 case 'b': case 'B': viewBook(); break;
 case 'c': case 'C': insertRecord(); break;
 case 'd': case 'D': deleteRecord(); break;
 case 'e': case 'E': modifyRecord(); break;
 case 'f': case 'F': printf("Goodbye!\n"); return 0;
 default: printf("Invalid choice, try again.\n");
 }
 }
}

/*
OUTPUT :
Enter your choice: b
Address book not found. Create it first (option a).
Enter your choice: a
Address book created successfully.
Enter your choice: c
Enter ID: 1
Enter Name : Amit Sharma
Enter Phone: 9876543210
Enter Email: amit@example.com
Record inserted.
(ID 2 Priya Patil and ID 3 Rahul Verma inserted the same way)
Enter your choice: b
ID Name Phone Email
------------------------------------------------------------------
1 Amit Sharma 9876543210 amit@example.com
2 Priya Patil 9123456780 priya@example.com
3 Rahul Verma 9988776655 rahul@example.com
Enter your choice: e
Enter ID to modify: 2
Enter new details (ID stays 2)
New Name : Priya Deshmukh
New Phone: 9123400000
New Email: priya.d@example.com
Record modified.
Enter your choice: b
ID Name Phone Email
------------------------------------------------------------------
1 Amit Sharma 9876543210 amit@example.com
2 Priya Deshmukh 9123400000 priya.d@example.com
3 Rahul Verma 9988776655 rahul@example.com
Enter your choice: d
Enter ID to delete: 1
Record deleted.
Enter your choice: b
ID Name Phone Email
------------------------------------------------------------------
2 Priya Deshmukh 9123400000 priya.d@example.com
3 Rahul Verma 9988776655 rahul@example.com
Enter your choice: d
Enter ID to delete: 99
Record not found.
Enter your choice: f
Goodbye!
*/


"Sir/Ma'am, this is an Address Book program using file handling in C. I have used a structure to store ID, name, phone number, and email. 
 The records are stored permanently in a binary file. Using a menu-driven switch statement, we can create, view, insert, delete, and modify records.
 I have used fread() and fwrite() for reading and writing records."


Q1. What is file handling?

File handling is used to store and retrieve data permanently in files.

Q2. What is a structure in C?

A structure is a user-defined data type that groups different types of variables together.

Q3. Why do we use a binary file?

A binary file stores data in binary format and allows efficient reading and writing of structures.

Q4. Why is temp.dat used?

It temporarily stores all records except the record that needs to be deleted.

Q5. What is the difference between fread() and fwrite()?
- fread() reads data from a file.
- fwrite() writes data into a file.
Q6. What is the use of fseek()?

It moves the file pointer to a specific position in the file.

Q7. Why is typedef struct used?

It creates an easy-to-use name Record for the structure.

Q8. What happens if we insert an existing ID?

The program displays a message that the ID already exists and does not insert the duplicate record.
