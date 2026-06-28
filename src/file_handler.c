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

/*--Save one PredictionRecord(append)--*/
void savePrediction(PredictionRecord *pr){
    FILE *f=fopen("dsts/history.dat","ab");
    if(f==NULL){
        printf("Error: Cannot open history.dat\n");
        return;
    }
    fwrite(pr, sizeof(PredictionRecord),1,f);
    fclose(f);  
}

/*-- Load prediction history for a student--*/
int loadHistory(int studentRef,PredictionRecord arr[],int *n){
    FILE *f=fopen("data/history.dat","rb");
    *n=0;
    if(f==NULL) return 0; 
    
    PredictionRecord temp;
    while(fread(&temp,sizeof(PredictionRecord),1,f)==1){
        if(temp.studentID == studentRef){
            arr[(*n)++]= temp;
        }
     }
     fclose(f);
    return *n;
 }

 /*--Search student by name(partical match)--*/
 void searchByName(const char*name){
    Student arr[100];
    int n,i,found = 0;
    loadAllStudents(arr,&n);
    printf("\n Search results for: \"%s\"\n", name);
    printf("%-5s %-25s %-15s %-6s\n","ID","Name","Student ID","CGPA");
    printf("%-5s %-25s %-15s %-6s\n","--","----","----------","----");
    for(i=0;i<n;i++){
        if(arr[i].isActive == 1&& strstr(arr[i].name,name)!=NULL){
            printf("%-5d %-25s %-15s %-6.2f\n",arr[i].id,arr[i].name,arr[i].studentID,arr[i].cgpa);
            found = 1;
        }
 }
    if(!found){
        printf("No students found with that name.\n");
    }
}