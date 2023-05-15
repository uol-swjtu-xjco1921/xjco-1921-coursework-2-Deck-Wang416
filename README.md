# XJCO 1921 Coursework 2

# Requirements

## Linux and Development Environment
Your code must compile and run on a standard Linux Distribution (such as Ubuntu).

If you're using a Windows operating system, you can still achieve this by installing the Windows Subsystem for Linux (WSL) and selecting Ubuntu as your Linux distribution.

To install WSL on Windows 10/11, follow the instructions provided at: https://learn.microsoft.com/zh-cn/windows/wsl/install. Once you have WSL installed, you can install the necessary development tools, such as gcc, gdb, and make, on Ubuntu by running the following commands in your Ubuntu terminal:

```
sudo apt update
sudo apt upgrade

sudo apt-get install -y build-essential
```

These commands will update the Ubuntu package list and install the build-essential package, which includes the development tools you'll need to compile and run your code on the Ubuntu Linux distribution.

## SDL (Simple DirectMedia Layer) and SDL_ttf Library

If you are a Windows system, you will need to go to the SDL official website (https://www.libsdl.org/) to download the SDL development libraries. Make sure you download the correct version for your development environment (such as MinGW, Visual Studio, etc).

If you are a Linux system, you can use the package manager to install SDL and SDL_ttf.  
In Debian-based systems, like Ubuntu, you can use the apt-get command:

```
sudo apt-get update
sudo apt-get install libsdl2-dev

sudo apt-get update
sudo apt-get install libsdl2-ttf-dev

```

In Fedora-based systems, you can use the dnf command:

```
sudo dnf install SDL2-devel

sudo dnf install SDL2_ttf-devel
```

In Arch-based systems, you can use the pacman command:

```
sudo pacman -S sdl2

sudo pacman -S sdl2_ttf
```

# Operation methods

You can compile the provided files by using:

```
make all
```

You run the test suite using:

```
bash ./test.sh
```

You run the program using:

```
./main inputFile.map
```

After entering the program:

```
Choose An Option from Shortest Path(A), Fastest Path(B), Constraint Route(C) and Edit Attributes(D): 
```

Under The Shortest Path(A) Mode :

```
Enter The Start Node ID: 
Enter The End Node ID: 
```
NB: They are all integers and cannot have extra characters. They must exist in the file and cannot be the same.

Under The Shortest Path(B) Mode :

```
Enter The Start Node ID: 
Enter The End Node ID: 
```

If there are no speed attribute values in the original file:

```
Usage: Add Speed Limits to The Links
Enter An Link ID: 
Enter Speed of The Link: 
Enter 'Y' to Continue, Enter Any Other Key to Finish: 
```
NB: 
Link ID is an integer, cannot has extra characters and must exist in the file.
Speed of the link is a positive value and cannot has extra characters.

Under The Constraint Route(C) Mode :

```
Enter The Start Node ID: 
Enter The End Node ID: 
Choose An Option from Passing A Given Location(L) and POI(P):
```

Under The Location(L) Mode:

```
Enter An Intermediate Node ID: 
Enter 'Y' to Continue, Enter Any Other Key to Finish: 
```

Under The Location(P) Mode:

```
Enter An POI ID: 
Enter 'Y' to Continue, Enter Any Other Key to Finish: 
```
NB: POI ID is a Non-Negative Integer, cannot has extra characters and must exist in the file.

Under The Edit Attributes(D): 

```
Choose A Type of Element to Modify from Link(1), Node(2) and POI(3): 
```

Under The Link(1): 

```
Enter The Link ID:
Link Attributes: length, veg, arch, land, speed
Enter The Attribute to Modify:
Enter The New Value: 
```
NB: length, veg, arch, land and speed are all positive values and cannot have extra characters.

Under The Node(2): 

```
Enter The Node ID:
Node Attributes: lat, lon
Enter The Attribute to Modify: 
Enter The New Value: 
```
NB: They are all numeric values, cannot have extra characters and must be within the bounding range.

Under The POI(3): 

```
Enter The Link ID:
Enter The New POI ID:
Enter The New Value: Enter 'Y' to Continue, Enter Any Other Key to Finish: 
```
NB: POI can be added multiple values, but only be supported to start from scratch and cannot be added after existing ones.

# Testing script

Users are provided with a shell script to test the program. This script includes two types of testing, unit testing and integration testing. Unit testing tests specific return values and error statements. The integration test tests the correctness of specific error prompts and output routes in interactive mode, including the shortest route, the fastest route, the shortest route passing through multiple designated positions, and the shortest route passing through multiple designated POIs.

The messages and return codes which I have used are:

| Value  | String | Condition |
| ------------- | ------------- | ------------- |
| 0  | Usage: ./executablefile inputFile.map | Program only given main parameter|
| 1  | ERROR: Bad Argument Count | Program given wrong amount of arguments |
| 2 | ERROR: Bad File Name (fname) | Program fails to open file |
| 3 | ERROR: Map Malloc Failed (fname) | Malloc failed to allocate memory |
| 4 | ERROR: Initialize SDL Failed | Program failed to visualize using SDL |
| 5 | ERROR: Bad Attributes (fname) | Program has incomplete attributes |
| 6 | ERROR: Bad Labels (fname) | Program has incomplete labels |

The prompts I have used are:

| String | Condition |
| ------------- | ------------- |
| Usage: ./executablefile inputFile.map | Program only given main parameter |
| Usage: Enter An Integer Value | Incorrect input type |
| Usage: Enter An Integer with No Extra Characters | Input exists extra characters |
| Usage: Usage: Enter An Existing # ID | The entered data does not exist |
| Usage: Enter A Valid Value between minLat and maxLat | Value out of range |
| Usage: Enter Either A, B, C or D | Option out of range |
| Usage: Enter An End Node ID Different from The Start Node ID | Same starting and ending nodes |
| Usage: Add Speed Limits to The Links | Missing speed attribute |
| Usage: The Link Already Has The Most POIs | Number of POIs has reached the limit |

The standard success messages I have used are:

| Value | String | Program | Meaning |
| ------------- | ------------- | ------------- | ------------- |
| 0 | READ AND STORED | readMap | Program successfully read and stored data |
| 0 | FOUND | main | Program successfully found various routes |
| 0 | EDITED | main | Program successfully edited attributes |
| 0 | VISUALIZED | visualization | Program successfully visualized using SDL |

# Test data

I have provide some test data for user to start with. These map files are in the test_data directory, which has the following structure:

```
test_data/bad_attributes: containing some input map data. These include some incomplete attributes to test error handling in this situation.
test_data/bad_labels: containing some input map data. These include some incomplete labels to test error handling in this situation.
test_data/edit_attributes: containing complete input map data, for users to test the function of editing attributes.
test_data/fastest_path: containing complete input map data, including speed attributes, for users to test the function of calculating the fastest route.
test_data/shortest_path: containing complete input map data, for users to test the function of calculating the shortest route.
test_data/path_with_locations: containing complete input map data, for users to test the function of calculating the shortest route that meets the constraint of passing multiple given locations.
test_data/path_with_pois: containing complete input map data, for users to test the function of calculating the shortest route that meets the constraint of passing multiple given POIs.
```