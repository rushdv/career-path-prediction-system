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