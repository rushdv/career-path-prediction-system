#include <stdio.h>
#include <string.h>
#include <time.h>
#include "prediction.h"
#include "colors.h"
#include "file_handler.h"
/* Career path definitions
   Skill order: [programming, networking, design, analytics, communication, security] */
static CareerPath careers[NUM_CAREERS] = {
    {
        "Software Developer",
        {0.35f, 0.10f, 0.15f, 0.20f, 0.10f, 0.10f},
        {6.0f,  3.0f,  3.0f,  4.0f,  3.0f,  2.0f}
    },
      {
        "Network Engineer",
        {0.15f, 0.40f, 0.05f, 0.15f, 0.10f, 0.15f},
        {3.0f,  7.0f,  2.0f,  3.0f,  3.0f,  4.0f}
    },
    {
        "UI/UX Designer",
        {0.15f, 0.05f, 0.40f, 0.15f, 0.20f, 0.05f},
        {3.0f,  1.0f,  7.0f,  3.0f,  5.0f,  1.0f}
    },
    {
        "Data Analyst",
        {0.20f, 0.05f, 0.10f, 0.40f, 0.15f, 0.10f},
        {4.0f,  2.0f,  2.0f,  7.0f,  4.0f,  2.0f}
    },
    {
        "IT Project Manager",
        {0.15f, 0.10f, 0.10f, 0.20f, 0.35f, 0.10f},
        {3.0f,  3.0f,  3.0f,  4.0f,  7.0f,  2.0f}
    },