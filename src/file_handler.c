#include <stdio.h>
#include <string.h>
#include "file_handler.h"

/*-Save one Student record(append)*/
void saveStudent(Student*s){
FILE*f=fopen("data/student.dat","ab");
if(f==NULL){
    printf("Error: Cannot open students.dat\n");
    return;
}
fwrite(s,sizeof(Student),1,f);
fclose(f);
}

/*--Load all Student records into array--*/
int loadAllStudents(Student arr[],int *n){
    FILE*f=fopen("data/student.dat","rb");
    *n=0;
    if(f==NULL)return 0; /*file not yet created*/

    while(fread(&arr[*n],sizeof(Student),1,f)==1)
    (*n)++;
    
    fclose(f);
    return *n;
}

/*--Rewrite entire student.dat with updated array--*/
void updateStudentFile(Student arr[],int n){
    FILE*f=fopen("data/students.dat","wb");
    if(f==NULL){
        printf("Error: Cannot rewrite students.dat\n");
        return;
    }
    fwrite(arr, sizeof(Student),n,f);
}

/*--Load prediction history for a student--*/
void savePrediction(PredictionRecord *pr){
    FILE *f=fopen("dsts/history.dat","ab");
    if(f==NULL){
        printf("Error: Cannot open history.dat\n");
        return;
    }
    fwrite(pr, sizeof(PredictionRecord),1,f);
    fclose(f);  
}

