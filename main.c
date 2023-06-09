#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Student scores are read in the following order:
//MATH, ENGLISH, SCIENCE, SOCIAL STUDIES, BDT, RME, ICT, FRENCH, GHANAIAN LANGUAGE.

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
    char school_name[20];
    char course[20];
};

struct placement {
    char school[15];
    char course[20];
};

struct student {
    char name[15];
    long int index_number;
    int core_subjects[3];
    int electives[6];
    int grade;
    int core_subjects_grades[4];
    int electives_grades[5];
    struct school_choice school_choices[3];
    struct placement placement;
};


int grade_score(int score) {
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
    return (*(int*)a - *(int*)b);
}

int sum_largest_two(int arr[], int n) {
    qsort(arr, n, sizeof(int), compare);
    int sum = 0;
    for (int i = 0; i < 2; i++) {
        sum += arr[i];
    }
    return sum;
}


int main() {
    FILE* students_file = fopen("students.txt", "r");
    FILE* schools_file = fopen("schools.txt", "r");
    FILE* cutoffs_file = fopen("cutoffs.txt", "r");

    //Check if files are found.
    if(students_file == NULL || schools_file == NULL || cutoffs_file == NULL) {
        printf("Error! File not found!");
        return 0;
    }
    //printf("Success! File is found\n");

    //Read the students file.
    struct student students[5];
    int has_ended=17;
    int students_count = 0;
    while(has_ended==17) {
        has_ended = fscanf(students_file, "%s %d %d %d %d %d %d %d %d %d %d %s %s %s %s %s %s",
                           &students[students_count].name, &students[students_count].index_number,
                           &students[students_count].core_subjects[0], &students[students_count].core_subjects[1],
                           &students[students_count].core_subjects[2], &students[students_count].core_subjects[3],
                           &students[students_count].electives[0], &students[students_count].electives[1],
                           &students[students_count].electives[2], &students[students_count].electives[3],
                           &students[students_count].electives[4], &students[students_count].school_choices[0].school_name,
                           &students[students_count].school_choices[1].school_name, &students[students_count].school_choices[2].school_name,
                           &students[students_count].school_choices[0].course, &students[students_count].school_choices[1].course,
                           &students[students_count].school_choices[2].course
                          );

        if(has_ended==17) {
            students_count++;
        }
    };
    fclose(students_file);

    //Read the schools file.
    int scanned=8;
    struct school schools[4];
    int schools_counter = 0;
    while(scanned==8) {
        scanned = fscanf(schools_file, "%s %s %s %s %s %s %s %s", schools[schools_counter].name,
                         schools[schools_counter].courses[0].name, schools[schools_counter].courses[1].name, schools[schools_counter].courses[2].name,
                         schools[schools_counter].courses[3].name, schools[schools_counter].courses[4].name, schools[schools_counter].courses[5].name,
                         schools[schools_counter].courses[6].name);

        if(scanned==8) {
            schools_counter++;
        }
    };
    fclose(schools_file);

    //Read the cutoffs file
    for(int i=0; i<schools_counter; i++) {
        fscanf(cutoffs_file, "%d %d %d %d %d %d %d", &schools[i].courses[0].cutoff, &schools[i].courses[1].cutoff,
               &schools[i].courses[2].cutoff, &schools[i].courses[3].cutoff, &schools[i].courses[4].cutoff,
               &schools[i].courses[5].cutoff, &schools[i].courses[6].cutoff);
    }


    //Calculate the grades of the students
    for(int i=0; i<students_count; i++) {
        int grade = 0;

        for (int subject=0; subject<4; subject++) {
            students[i].core_subjects_grades[subject] = grade_score(students[i].core_subjects[subject]);
        }
        for (int subject=0; subject<5; subject++) {
            students[i].electives_grades[subject] = grade_score(students[i].electives[subject]);
        }
        grade += array_sum(students[i].core_subjects_grades, 4);
        grade += sum_largest_two(students[i].electives_grades, 5);
        students[i].grade = grade;
    }



    //Compare the cutoffs and place the students based on their choices
    printf("\n");
    for(int student=0; student<students_count; student++) {
        int placed=0;
        int student_grade = students[student].grade;
        char *student_school_choices[3] = {students[student].school_choices[0].school_name,
                                           students[student].school_choices[1].school_name,
                                           students[student].school_choices[2].school_name
                                          };
        //printf("%s\n", students[student].name);

        struct school last_choice;

        for(int i=0; i<3; i++) {
            int index_found;
            int matched;
            char choice[20];

            //Look through each school.
            struct school school_found;
//            printf("Searching for school name that matches with %s (for %s)\n",
//                   students[student].school_choices[i].school_name,
//                   students[student].name
//                  );

            for(int school=0; school<schools_counter; school++) {
                char school_name[20];
                strcpy(choice, student_school_choices[i]);
                strcpy(school_name, schools[school].name);
                //printf("\tSearching %s if it matches with %s...\n", schools[school].name, choice);
                matched = strcmp(choice, school_name);
                //printf("\t%s\n", (matched==0) ? "true" : "false");
                if(matched == 0) {
                    school_found = schools[school];
                    index_found = i;
                    break;
                }
            }

            //Compare the cutoffs for the school found with the student
//            printf("\t\tThe school found that matched with the choice %s is %s\n", choice, school_found.name);
//            printf("Comparing grades of %s...\n", school_found.name);
//            printf("School Choice #%d\n", index_found);
            int current_settle = 54;
            char current_settle_name[20];

            for(int c=0; c<7; c++) {
//                printf("%d (%s)(c is %d)\t", school_found.courses[c].cutoff, school_found.courses[c].name, c);
//                printf("The student's grade is %d\n", students[student].grade);
                if(students[student].grade<=school_found.courses[c].cutoff) {

                    //Check if it's their choice after they have qualified.
                    //However, if this is their last choice school, they should settle for
                    //the course they didn't choose.
//                    printf("The student has qualified for %s, checking if in their choices...\n",
//                           school_found.courses[c].name);
//                    printf("But first let's check if it's their last choice school\n");
                    //If they have a choice...
                    if(index_found<2) {
                        //printf("It's not their last choice school so let's proceed to check if in their choices\n");
                        char chc[20];
                        char comp[20];
                        strcpy(chc, students[student].school_choices[index_found].course);
                        strcpy(comp, school_found.courses[c].name);
                        if(strcmp(chc, comp)==0) {
//                            printf("%s has qualified to study %s at %s which is his choice (school #%d) ----------------------\n",
//                                   students[student].name, school_found.courses[c].name,
//                                   school_found.name, index_found);
                            placed = 1;
                            break;
                        } else {
//                            printf("%s has qualified to study %s at %s which is NOT his choice (school #%d) (her choice is %s)\n",
//                                   students[student].name, school_found.courses[c].name,
//                                   school_found.name, index_found, students[student].school_choices[i].course);
                        }
                    }
                    //If this is their last choice school, they have to settle for that course.
                    else {
//                        printf("It's their last choice school...");
//                        printf("%s has qualified for %s which is not his choice at %s "
//                               "but he has to settle for it now (school #%d)\n",
//                               students[student].name,
//                               school_found.courses[c].name,
//                               school_found.name,
//                               index_found
//                              );
                        if(school_found.courses[c].cutoff < current_settle) {
                            strcpy(current_settle_name, school_found.courses[c].name);
                            current_settle = school_found.courses[c].cutoff;
                        }

//                        printf("The last course settled for: %s (%d) (c is %d)\n",
//                               current_settle_name,
//                               current_settle, c);

                        if(c+1==7) {
                            strcpy(students[student].placement.school, school_found.name);
                            strcpy(students[student].placement.course, school_found.courses[c].name);
//                            printf("%s has been placed in %s for %s\n", students[student].name, school_found.name, school_found.courses[c].name);
//                            printf("Finally settled for : %s (%d) (c is %d)\n",
//                                   current_settle_name,
//                                   current_settle, c);
                            placed = 1;
                            break;
                        }
                    }
                }
                //The student didn't qualify, but let's check if that's their choice
                //so that we can break the loop early
                else {
                    char chc[20];
                    char comp[20];
                    strcpy(chc, students[student].school_choices[index_found].course);
                    strcpy(comp, school_found.courses[c].name);
                    if(strcmp(chc, comp)==0) {
                        //But let's check if it's their last choice
                        if(index_found<2) {
                            //printf("This is their choice, sadly. THE PERSON CHOP BOUNCE EARLY!");
                            break;
                        }
                    }
                }
            }
            if(matched==0 && placed == 1)
                break;
            else if(matched==0 && placed == 0)
                //printf("THE PERSON CHOP BOUNCE BE THAT LET'S MOVE TO THE NEXT SCHOOL IN THEIR CHOICE LIST\n");
            last_choice = school_found;
        }

        //printf("placed: %d\n", placed);

        //Worst case scenario: Not qualified for any choice
        if(placed==0) {
            //printf("He wasn't able to get any school of his choice. In this case, let's give him...\n\n");
            //Find the closest course qualified for.
            int how_close = 54;
            char how_close_course[20];
            for(int z=0; z<7; z++) {
//                printf("checking how close he is to %s.. (%d) The closest so far is %d which is for %s\n",
//                       last_choice.courses[z].name,
//                       students[student].grade-last_choice.courses[z].cutoff, how_close, how_close_course);
                if (students[student].grade-last_choice.courses[z].cutoff <= how_close) {
                    how_close = students[student].grade-last_choice.courses[z].cutoff;
                    strcpy(how_close_course, last_choice.courses[z].name);
                }
            }

            strcpy(students[student].placement.school, last_choice.name);
            strcpy(students[student].placement.course, how_close_course);
            placed = 1;
            //printf("%s has been placed in %s for %s\n", students[student].name, last_choice.name, how_close_course);
        }
    }



//THE FRONTEND PART OF THINGS
main:
auth:
    printf("=================================\n");
    printf("=======-WELCOME TO THE-==========\n");
    printf("== SCHOOL PLACEMENT APPLICATION =\n");
    printf("=================================\n");
    printf("\nEnter your index number to login and check your placement: ");
    int index_number;
    scanf("%d", &index_number);

    //Validate Index Number
    int found = 0;
    struct student student;
    for(int i=0; i<5; i++) {
        if(students[i].index_number==index_number) {
            student = students[i];
            found = 1;
            break;
        }
    }
    if(found==0) {
        printf("Invalid Index Number!\n");
        system("cls");
        goto auth;
    }

    printf("\n---Student Details---\n");
    printf("Name: %s\n", student.name);
    printf("Index Number: %d\n", student.index_number);

    printf("\n==(Results)==\n");
    printf("\tMath: %d\n", student.core_subjects[0]);
    printf("\tEnglish: %d\n", student.core_subjects[1]);
    printf("\tScience: %d\n", student.core_subjects[2]);
    printf("\tSocial Studies: %d\n", student.core_subjects[3]);
    printf("\tBDT: %d\n", student.electives[0]);
    printf("\tRME: %d\n", student.electives[1]);
    printf("\tICT: %d\n", student.electives[2]);
    printf("\tFrench: %d\n", student.electives[3]);
    printf("\tGhanaian Language: %d\n\n", student.electives[4]);

    printf("Aggregate: %d\n", student.grade);
    printf("\nFirst Choice School: %s (course: %s)\n", student.school_choices[0].school_name, student.school_choices[0].course);
    printf("Second Choice School: %s (course: %s)\n", student.school_choices[1].school_name, student.school_choices[1].course);
    printf("Third Choice School: %s (course: %s)\n\n", student.school_choices[2].school_name, student.school_choices[2].course);

    printf("\n---Placement Details--- \n");
    printf("School Placed: %s\n", student.placement.school);
    printf("Course: %s\n", student.placement.course);

end: ;
    char response[1];
    printf("\nLogin Again?[y/n] ");
    scanf("%s", &response);

    if(strcmp(response, "y")==0) {
        system("cls");
        goto main;
    } else if(strcmp(response, "n")==0)
        return 0;
    else {
        system("cls");
        goto end;
    }


    return 0;
}
