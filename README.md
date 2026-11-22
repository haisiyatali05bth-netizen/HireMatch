# HireMatch – Smart Job Matching System

A C++17 + Qt Widgets desktop application demonstrating core Data Structures and Algorithms.

## Features
- Candidate profile registration and local text-file persistence
- Job listing creation and display
- Skill-based job matching with percentage scores
- Custom Merge Sort to rank recommendations
- HashSet-based average O(1) skill lookup
- Stack-based undo for the latest application in the current session
- Queue-based FIFO processing of pending applications
- Binary Search Tree indexing/search for job IDs
- Application history and status tracking

## Requirements
- C++17 compiler
- CMake 3.16+
- Qt 6 with the Widgets component
- Qt Creator (recommended) or VS Code

## Build with Qt Creator
1. Install Qt 6 (including a compiler kit) and CMake.
2. Open `CMakeLists.txt` in Qt Creator.
3. Select a desktop kit that matches your installed compiler.
4. Configure the project and click Run.

## Build from terminal
Run these commands from the project root:

```bash
cmake -S . -B build
cmake --build build
```

Run the generated executable from the build output directory. The application creates a `data` folder in its working directory on first launch.

## Data files
- `data/candidates.txt`: id|name|education|experience|comma-separated-skills
- `data/jobs.txt`: id|company|title|location|experience|comma-separated-skills
- `data/applications.txt`: id|job-id|status

Do not use the `|` character in names, company names, job titles, or locations because it is the field separator.

## Demo workflow
1. Open Candidate Profile and save a candidate with skills: `C++, Java, DSA, SQL, HTML`.
2. Open Job Management and add:
   - Infosys | Software Developer | Chandigarh | 0 | C++, DSA, SQL
   - TCS | Java Developer | Pune | 0 | Java, DSA
   - Startup X | Backend Developer | Remote | 1 | Java, SQL, Python, Spring Boot
3. Open Smart Matching and click Find Matching Jobs.
4. Select a job row and apply from the Applications tab.
5. Process the next pending application, then try Undo Last Application on a newly submitted application.

## DSA complexity notes
- HashSet lookup: average O(1), worst-case O(n)
- Merge Sort: O(n log n) time, O(n) auxiliary space
- Stack push/pop: O(1)
- Queue enqueue/dequeue: O(1)
- BST search/insert: O(h), where h is tree height; worst case O(n)

## Important scope notes
- The project is a local academic demo, not a live recruitment platform.
- Matching is based on skill overlap only; experience and education are displayed/stored but are not included in the initial score.
- The undo stack is in memory, so undo history is available only during the current running session.
- The pending queue is demonstrated by processing the oldest application with Pending status.
