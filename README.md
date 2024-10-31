# Student Management System

This repository contains the implementation of a student management system, which includes functionalities for adding, searching, and managing student records using an indexed approach for efficient data retrieval.

## Features

- **Student Records**: Store and manage student information, including their identification and other relevant details.
- **Indexing System**: Utilize a dense index to facilitate quick lookups of student records based on unique identifiers.
- **Data Storage**: Implement methods for saving and loading student records from disk to ensure persistence.
- **Temporary Block Management**: Use a temporary block to minimize disk writes, enhancing performance when adding or modifying records.

## File Structure

- `etudiant.c`: Contains the main implementation of student management functionalities.
- `etudiant.h`: Header file for the student structure and function declarations.
- `index.c`: Implementation of indexing functions.
- `index.h`: Header file for index-related structures and declarations.
- `global.h`: Contains global variable declarations.
- `main.c`: Entry point of the application.
- `.vscode`: Directory for Visual Studio Code settings and configurations.

## Usage

To use this system, compile the source files and execute the generated binary. The application provides a command-line interface for interacting with student records.

## License

This project is licensed under the MIT License.
