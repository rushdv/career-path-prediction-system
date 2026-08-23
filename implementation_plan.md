# Career Path Prediction Feature Implementation Plan

The goal of this plan is to securely add password functionality for students, upgrade the prediction engine to factor in completed CSE major courses, and drastically expand the recommended career roles to cover the modern IT landscape.

## Proposed Changes

### 1. Student Login Security (Password)
#### [MODIFY] [student.h](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/include/student.h)
- Add `char password[50];` to the `Student` struct.

#### [MODIFY] [student.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/student.c)
- Update `createStudent()` to prompt the user to create a password during registration.
- Update `updateStudent()` to allow the user to change their password.

#### [MODIFY] [auth.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/auth.c)
- Update the student login flow to require the password alongside the Student ID.

### 2. Major Courses for Prediction
#### [MODIFY] [assessment.h](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/include/assessment.h)
- Add boolean flags (or integers) to `SkillProfile` to track completed courses:
  - `course_ml_ai`
  - `course_web_engineering`
  - `course_data_science`
  - `course_cybersecurity`
  - `course_cloud_computing`

#### [MODIFY] [assessment.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/assessment.c)
- Update `runAssessment()` to ask 5 simple Yes/No questions regarding the completion of these major courses.
- The answers will be fed directly into the prediction algorithm to heavily boost relevant career scores.

### 3. Upgraded Career Roles & Prediction Engine
#### [NEW/MODIFY] [prediction.h](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/include/prediction.h)
- Define `CareerPath` struct containing required skills for each role.
- Define `NUM_CAREERS` to **10**.

#### [NEW/MODIFY] [prediction.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/prediction.c)
- Implement `getCareers()` to define 10 highly relevant modern IT careers:
  1. Software Engineer (Backend/Frontend)
  2. Data Scientist / Analyst
  3. Cybersecurity Specialist
  4. Cloud Architect (AWS/Azure)
  5. DevOps Engineer
  6. AI/Machine Learning Engineer
  7. Mobile App Developer
  8. Game Developer
  9. Database Administrator (DBA)
  10. Systems Programmer
- Implement `calculateScore()` to calculate a match percentage. This will use a weighted algorithm that considers both the 1-10 skill ratings and the Y/N major course completions. If a student completed the Machine Learning course, the AI/ML Engineer score will receive a massive boost.
- Implement `rankCareers()` to sort and display the top 3 recommended careers with an interactive UI.

### 4. Gap Analysis and Reporting
#### [NEW/MODIFY] [gap_analysis.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/gap_analysis.c)
- Implement `analyzeGap()` to compare the student's skills against their best-matched career and explicitly list which areas they need to improve (e.g., "To become an AI Engineer, you need to improve your Data Analytics score from 4 to 8").

#### [NEW/MODIFY] [report.c](file:///run/media/aevum7/Lab/Dev%20Stack/My%20Projects/career-path-prediction-system/src/report.c)
- Implement `generateReport()` to output a beautifully formatted final summary.

## User Review Required
> [!WARNING]
> Because we are adding a password to the `Student` structure, the size of the structure in memory will change. This means your old `students.dat` binary file will no longer be compatible. Once these changes are implemented, **you must delete the old `data/students.dat` file** and register your students again. Is this acceptable?

> [!TIP]
> Please review the 10 career roles listed above. If you want to add or change any specific job roles, let me know now! If everything looks good, just approve this plan and I'll get to work.
