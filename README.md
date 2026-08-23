# 🚀 Career Path Prediction System

[![Language: C99](https://img.shields.io/badge/Language-C99-blue.svg)](https://en.wikipedia.org/wiki/C99)
[![Database: SQLite3](https://img.shields.io/badge/Database-SQLite3-003B57.svg?logo=sqlite&logoColor=white)](https://www.sqlite.org/)
[![Security: OpenSSL](https://img.shields.io/badge/Security-OpenSSL-721412.svg)](https://www.openssl.org/)
[![Build: Makefile%20%7C%20CMake](https://img.shields.io/badge/Build-Makefile%20%7C%20CMake-brightgreen.svg)](https://cmake.org/)

An advanced, menu-driven C-based **Career Path Prediction & Skill Analytics System**. The system evaluates computer science students' skill profiles, academic course completions, and performance metrics to intelligently score, rank, and recommend the best-suited modern IT career pathways—complete with gap analysis and downloadable report generation.

---

## ✨ Key Features

- 🔐 **Secure Authentication System:** User signup/login with password security using SHA-256 via OpenSSL.
- 👨‍🎓 **Student Profile Management:** Create, view, update, and manage student demographic & academic information.
- 🎯 **Comprehensive Skill Assessment:** Interactive 1-10 rating across core competencies (Problem Solving, Data Structures, Web, Networking, Data Analytics, Security, Cloud, AI/ML) plus CSE major course tracking.
- 🧠 **Intelligent Career Matching Engine:** Weighted scoring algorithm predicting compatibility with 10 modern IT career roles:
  1. **Software Engineer** (Backend/Frontend)
  2. **Data Scientist / Analyst**
  3. **Cybersecurity Specialist**
  4. **Cloud Architect**
  5. **DevOps Engineer**
  6. **AI/Machine Learning Engineer**
  7. **Mobile App Developer**
  8. **Game Developer**
  9. **Database Administrator (DBA)**
  10. **Systems Programmer**
- 📊 **Skill Gap Analysis:** Highlights exact skill gaps needed to reach target career roles.
- 📈 **Performance & System Analytics:** Statistical insights and career distribution metrics.
- 💾 **SQLite Database Persistence:** High-reliability SQL database layer using `libsqlite3`.
- 📄 **Report Generation:** Generates formatted text/summary reports for students.
- 🎨 **Rich Terminal UI:** Enhanced user experience with ANSI colors, dynamic borders, and clean menu navigation.

---

## 👥 Team Work Plan & File Distribution

| Week | Task | Deliverable | Responsible |
| :---: | :--- | :--- | :--- |
| **1** | Requirement analysis & Structure design | Design document | All members |
| **2** | Student profile & Skill assessment modules | `student.c`, `assessment.c` | **Shihab Shahriar** |
| **3** | Scoring, ranking, and gap analysis logic | `prediction.c` | **Sajib Saha** |
| **4** | File handling for history & report generation | `file_handler.c` / `db_handler.c` | **Abrar Mueed** |
| **5** | Menu-driven interface & Integration | `main.c` | All members |
| **6** | Testing, debugging, documentation & final presentation prep | Final build + report | All members |

### 📂 Detailed Code Distribution

#### 👤 **Shihab Shahriar** — *Student Profile, Assessment & Authentication*
- [student.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/student.c) / [student.h](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/include/student.h) — Student profile creation, profile updates, and structural definitions.
- [assessment.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/assessment.c) / [assessment.h](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/include/assessment.h) — Interactive skill assessment & CSE major course tracking.
- [auth.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/auth.c) / [auth.h](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/include/auth.h) — Authentication workflow and login/signup validation.
- [input_handler.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/input_handler.c) / [input_handler.h](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/include/input_handler.h) — Safe terminal input parsing and sanitization.

#### 👤 **Sajib Saha** — *Career Prediction Engine & Analytics*
- [prediction.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/prediction.c) / [prediction.h](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/include/prediction.h) — Weighted scoring engine, career ranking, and path matching algorithms.
- [gap_analysis.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/gap_analysis.c) / [gap_analysis.h](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/include/gap_analysis.h) — Skill deficiency detection & targeted improvement suggestions.
- [analytics.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/analytics.c) / [analytics.h](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/include/analytics.h) — Performance analytics and aggregate statistical reporting.

#### 👤 **Abrar Mueed** — *Database Management & Report Generation*
- [db_handler.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/db_handler.c) / [db_handler.h](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/include/db_handler.h) — SQLite database initialization, queries, and data persistence.
- [report.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/report.c) / [report.h](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/include/report.h) — Report compiler and export mechanisms.

#### 👥 **All Members** — *Integration, UI & Build System*
- [main.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/main.c) — Application entry point, controller, and main program loop.
- [ui.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/ui.c) / [ui.h](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/include/ui.h) — Layout rendering, header formatting, and terminal screens.
- [colors.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/colors.c) / [colors.h](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/include/colors.h) — ANSI color theme engine and text styling.
- [Makefile](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/Makefile) & [CMakeLists.txt](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/CMakeLists.txt) — Project compilation scripts and dependencies.

---

## 🛠️ Project Structure

```
career-path-prediction-system/
├── CMakeLists.txt         # CMake build configuration
├── Makefile               # GNU Make automation script
├── README.md              # Project documentation
├── career.db              # SQLite3 production database
├── include/               # C Header files
│   ├── analytics.h
│   ├── assessment.h
│   ├── auth.h
│   ├── colors.h
│   ├── db_handler.h
│   ├── gap_analysis.h
│   ├── input_handler.h
│   ├── prediction.h
│   ├── report.h
│   ├── student.h
│   └── ui.h
└── src/                   # C Source implementations
    ├── analytics.c
    ├── assessment.c
    ├── auth.c
    ├── colors.c
    ├── db_handler.c
    ├── gap_analysis.c
    ├── input_handler.c
    ├── main.c
    ├── prediction.c
    ├── report.c
    ├── student.c
    └── ui.c
```

---

## ⚙️ Prerequisites

Before building the project, ensure you have the following dependencies installed on your system:

- **C Compiler:** `gcc` (supporting C99 or later)
- **Build System:** `make` or `cmake` (v3.10+)
- **Database Library:** SQLite3 (`libsqlite3-dev`)
- **Crypto Library:** OpenSSL (`libssl-dev`)

### Installation on Ubuntu/Debian Linux:
```bash
sudo apt update
sudo apt install build-essential cmake libsqlite3-dev libssl-dev -y
```

---

## 🚀 Building and Running

### Option 1: Using `Makefile` (Recommended)

1. **Compile the project:**
   ```bash
   make
   ```
2. **Run the executable:**
   ```bash
   ./career_system
   ```
3. **Clean build artifacts:**
   ```bash
   make clean
   ```

### Option 2: Using `CMake`

1. **Create build directory & generate build files:**
   ```bash
   cmake -B build
   ```
2. **Build the project:**
   ```bash
   cmake --build build
   ```
3. **Run the executable:**
   ```bash
   ./career_system
   ```

---

## 📜 License

This project is open-source and developed for academic and educational purposes.
