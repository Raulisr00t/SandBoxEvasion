# Payload Execution Program

This program performs several system checks and, if all conditions are met, executes a payload by injecting it into the `explorer.exe` process. The payload is downloaded from a specified URL.

## Description

The program performs the following checks:

1. **Geolocation Check**: Verifies if the system's public IP address matches a predefined IP address.
2. **Memory Check**: Ensures the system has at least 5 GB of physical memory.
3. **Domain Controller Check**: Determines if the system is a domain controller.

If all checks pass, the program downloads a payload (shellcode) from a specified URL and executes it within the `explorer.exe` process.

## Prerequisites

- Windows operating system.
- Internet connection to access external IP and download the payload.
- Necessary permissions to access and manipulate system processes.

## Dependencies

The program uses the following libraries:

- `<iostream>`: For input and output operations.
- `<cstdio>`: For C-style standard I/O operations.
- `<urlmon.h>`: For URL stream operations.
- `<windows.h>`: For Windows API functions.
- `<NTSecAPI.h>`: For security-related API functions.
- `<lm.h>`: For LAN Manager API functions.

## Compilation

To compile this program, use a C++ compiler with the necessary libraries linked. For example, using `g++`:

```sh
g++ -o payload_executer payload_executer.cpp -lurlmon -lnetapi32
Usage
Run the compiled executable. The program will:

Sleep for 60 seconds if a debugger is detected.
Perform the geolocation check.
Perform the memory check.
Perform the domain controller check.
If all checks are successful, download and execute the payload.
sh
Copy code
./payload_executer
Code Explanation
Geolocation Check
The Geolocation function fetches the public IP address of the system from https://ifconfig.me/ip and compares it with a predefined IP address (192.168.40.226). If they match, it returns TRUE; otherwise, FALSE.

Memory Check
The Memory function checks if the system has at least 5 GB of physical memory. If true, it returns TRUE; otherwise, FALSE.

Domain Controller Check
The isDomainController function checks if the system is a domain controller by calling NetGetDCName. If it is a domain controller, it returns TRUE; otherwise, FALSE.

Payload Execution
The Execution function:

Opens the explorer.exe process with all access permissions.
Downloads the payload (shellcode) from a specified URL (https://your-ip.com/index.raw).
Allocates memory in the explorer.exe process.
Writes the downloaded payload to the allocated memory.
Creates a remote thread in the explorer.exe process to execute the payload.
Main Function
The main function orchestrates the checks and payload execution:

Checks if a debugger is present. If so, sleeps for 60 seconds.
Performs the geolocation, memory, and domain controller checks.
If all checks pass, calls the Execution function to run the payload.
Disclaimer
This program is for educational purposes only. Unauthorized use of this code, including using it to perform unauthorized actions on any system, is illegal and unethical. The author is not responsible for any misuse of this program. Use responsibly and legally.
