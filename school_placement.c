#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct course {
    char name[20];
    int cutoff;
};

struct school {
    char name[10];
    int cutoff_list[7];
    struct course courses[7];
};

struct school_choice {
    char school_name[10];
};

struct placement {
    char school;
    char course;
};

struct student {
    char name[15];
    long int index_number;
    int core_subjects[3];
    int electives[6];
    int grade;
    int core_subjects_grades[3];
    int electives_grades[6];
    struct school_choice school_choices[3];
    struct placement placement;
    };


int grade_score(int score){
    if(score>=75 && score<=100)
        return 1;
    else if(score>=70)
        return 2;
    else if(score>=65)
        return 3;
    else if(score>=60)
        return 4;
    else if(score>=55)
        return 5;
    else if(score>=50)
        return 6;
    else if(score>=45)
        return 7;
    else if(score>=40)
        return 8;
    else if(score<40)
        return 9;
}

int array_sum(int arr[], int size) {
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

int compare(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

int sum_largest_three(int arr[], int n) {
    qsort(arr, n, sizeof(int), compare);
    int sum = 0;
    for (int i = 0; i < 3; i++) {
        sum += arr[i];
    }
    return sum;
}


int main()
{
    FILE* students_file = fopen("students.txt", "r");
    FILE* schools_file = fopen("schools.txt", "r");
    FILE* cutoffs_file = fopen("cutoffs.txt", "r");

    //Check if files are found.
     if(students_file == NULL || schools_file == NULL || cutoffs_file == NULL){
        printf("Error! File not found!");
        return 0;
    }
    printf("Success! File is found\n");

    //Read the student file.
    struct student students[5];
    int has_ended=14;
    int students_count = 0;
    while(has_ended==14){
       has_ended = fscanf(students_file, "%s %d %d %d %d %d %d %d %d %d %d %s %s %s", &students[students_count].name, &students[students_count].index_number,
               &students[students_count].core_subjects[0], &students[students_count].core_subjects[1], &students[students_count].core_subjects[2],
               &students[students_count].electives[0], &students[students_count].electives[1], &students[students_count].electives[2],
               &students[students_count].electives[3], &students[students_count].electives[4], &students[students_count].electives[5],
               &students[students_count].school_choices[0].school_name, &students[students_count].school_choices[1].school_name, &students[students_count].school_choices[2].school_name
               );

        if(has_ended==14){
            /*printf("%s %d %d %d %d %d %d %d %d %d %d\n", students[i].name, students[i].index_number,
               students[i].core_subjects[0], students[i].core_subjects[1], students[i].core_subjects[2],
               students[i].electives[0], students[i].electives[1], students[i].electives[2],
               students[i].electives[3], students[i].electives[4], students[i].electives[5]
               );*/
               students_count++;
        }
    };
    fclose(students_file);

    //Read the schools file.
    int scanned=8;
    struct school schools[4];
    int schools_counter = 0;
    while(scanned==8){
        scanned = fscanf(schools_file, "%s %s %s %s %s %s %s %s", schools[schools_counter].name,
        schools[schools_counter].courses[0].name, schools[schools_counter].courses[1].name, schools[schools_counter].courses[2].name,
        schools[schools_counter].courses[3].name, schools[schools_counter].courses[4].name, schools[schools_counter].courses[5].name,
        schools[schools_counter].courses[6].name);

        if(scanned==8){
            /*printf("%s %s %s %s %s %s %s %s\n", schools[schools_counter].name,
                schools[schools_counter].courses[0].name, schools[schools_counter].courses[1].name, schools[schools_counter].courses[2].name,
                schools[schools_counter].courses[3].name, schools[schools_counter].courses[4].name, schools[schools_counter].courses[5].name,
                schools[schools_counter].courses[6].name);*/
            schools_counter++;
        }
    };
    fclose(schools_file);

    //Read the cutoffs file
    for(int i=0;i<schools_counter;i++){
            fscanf(cutoffs_file, "%d %d %d %d %d %d %d", &schools[i].courses[0].cutoff, &schools[i].courses[1].cutoff,
               &schools[i].courses[2].cutoff, &schools[i].courses[3].cutoff, &schools[i].courses[4].cutoff,
               &schools[i].courses[5].cutoff, &schools[i].courses[6].cutoff);
  }


    //Calculate the grades of the students
    printf("The number of students found: %d\n", students_count);
    for(int i=0;i<students_count;i++){
            int grade = 0;
            //struct student student = students[i];
            //printf("Student: %s\n", student.name);

        for (int subject=0;subject<3;subject++){
            students[i].core_subjects_grades[subject] = grade_score(students[i].core_subjects[subject]);
            //printf("%d\t", students[i].core_subjects_grades[subject]);
        }
        for (int subject=0;subject<6;subject++){
            students[i].electives_grades[subject] = grade_score(students[i].electives[subject]);
            //printf("%d\t", students[i].electives_grades[subject]);
        }
        grade += array_sum(students[i].core_subjects_grades, 3);
        grade += sum_largest_three(students[i].electives_grades, 6);
        students[i].grade = grade;
        printf("The student's grade is: %d\n", students[i].grade);
    }



    //Compare the cutoffs and place the students based on their choices
    printf("\n");
    for(int student=0;student<students_count;student++){
        int student_grade = students[student].grade;
        char *student_school_choices[3] = {students[student].school_choices[0].school_name,
                                        students[student].school_choices[1].school_name,
                                        students[student].school_choices[2].school_name};
        //printf("%s\n", student_school_choices[0]);
        for(int i=0;i<3;i++){
            int matched;
            char choice[10];
            //Look through each school.
            struct school school_found;
            //printf("Searching for school name that matches with %s\n", student_school_choices[i]);
            for(int school=0;school<schools_counter;school++){

                char school_name[10];
                strcpy(choice, student_school_choices[i]);
                strcpy(school_name, schools[school].name);
//                char choice[] = "here";
//                char school_name[] = "here";
                //printf("\tSearching %s...\n", school_name);
                matched = strcmp(choice, school_name);
                //printf("\t%s\n", (matched==0) ? "true" : "false");
                if(matched == 0)
                {
                    school_found = schools[school];
                    break;
                }
            }

            //Compare the cutoffs for the school found with the student
            //printf("\t\tThe school found that matched with the choice %s is %s\n", choice, school_found.name);
            //printf("Comparing grades of %s...\n", school_found.name);
            for(int c=0;c<3;c++){
                //printf("%d\t", school_found.courses[c].cutoff);
                //printf("The student's grade is %d\n", students[student].grade);
                if(students[student].grade<=school_found.courses[c].cutoff){
                    printf("%s has qualified to study %s at %s\n", students[student].name, school_found.courses[c].name,
                           school_found.name);
                    students[student].placement.school = school_found.name;
                    students[student].placement.course = school_found.courses[c].name;
                    break;
                }
            }
            if(matched==0)
                break;
        }
    }








    return 0;
}
