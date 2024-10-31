Student Management System
This repository contains a Student Management System implemented in C, designed for efficiently handling student records. The project includes functionalities for adding, retrieving, and managing student data using an indexed approach for fast access.

Features
Student Record Management: Add, search, and retrieve student records.
Indexing System: Utilizes an index for quick lookup of student records, reducing the time complexity of search operations.
Temporary Buffering: Implements a temporary block to optimize disk writes, improving performance by minimizing the number of file operations.
File Handling: Efficient management of student data through structured files for storage and retrieval.
Directory Structure
.vscode/: Contains settings for Visual Studio Code.
etudiant.c: The main implementation file for student management functions.
etudiant.h: Header file defining the Etudiant structure and related functions.
global.h: Contains external variable declarations for global access.
index.c: Implementation of indexing functionalities for fast data access.
index.h: Header file defining index-related structures and functions.
main.c: Entry point of the application.
struct.h: Defines various data structures used in the system.
Usage
To compile the project, use a C compiler, and execute the generated executable to interact with the student management system.

License
This project is licensed under the MIT License - see the LICENSE file for details.
