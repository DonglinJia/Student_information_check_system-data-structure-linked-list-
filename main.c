//
//  main.c
//  Student_login
//
//  Created by Leo Jia on 2018-05-18.
//  Copyright © 2018 Leo Jia. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


// build structrue for students
struct student {
    char* coop;
    char* name;
    char* major;
    char* current_level;
    char* login;
    char* addr;
    char* email;
    char* cell;
    char* number;
    int height;
    int weight;
    int current_gpa;
    struct student *next;
};


// build structure for school and create a pointer point to student
struct school {
    char* name;
    char* authority_number;
    struct student *stdd;
};


// inputstr() use doulbe strategy to input the information
// malloc certain amount of memory based on the maxlen
// realloc the memory based on the actual length to save memory
// effect: read input
char *inputstr() {
    char c;
    if (scanf(" %c", &c) != 1) return NULL;
    int maxlen = 1;
    int len = 1;
    char* str = malloc(maxlen * sizeof(char));
    str[0] = c;
    while (1) {
        if (scanf("%c", &c) != 1) break;
        if (c == '\n') break;
        if (len == maxlen) {
            maxlen *= 2;
            str = realloc(str, maxlen * sizeof(char));
        }
        ++len;
        str[len-1] = c;
    }
    str = realloc(str, (len+1) * sizeof(char));
    str[len] = '\0';
    return str;
}

// build school() initialize struct school, malloc certain amount of memory for structure
// input all information about structure school
// effect: initialize structure
struct school *build_school() {
    struct school *s = malloc(sizeof(struct school));
    printf("Please enter school's name: ");
    s->name = inputstr();
    printf("Please enter the school pin code: ");
    s->authority_number = inputstr();
    s->stdd = NULL;
    return s;
}


// destroy_student (*std) free all the memory that a node(student) consume
// effect: free the memory that one node consume
void destroy_student(struct student *std) {
    free(std->number);
    free(std->name);
    free(std->addr);
    free(std->major);
    free(std->cell);
    free(std->email);
    free(std->current_level);
    free(std->login);
    free(std->coop);
    free(std);
}


// destroy_school(s) free all the memory that used for school(the whole list)
// effect: free all the memory
void destroy_school(struct school *s) {
    free(s->name);
    free(s->authority_number);
    struct student *std = s->stdd;
    struct student *cur  = NULL;
    while (std) {
        cur = std;
        std = std->next;
        destroy_student(cur);
    }
    
    free(s);
}


// check whether enter a legal password; a legal password must have a length of 8 with at least one upper case
// letter and a special char like: !@#$%^&*() etc
// effect: check the string and produce the feedback
bool check_password(char* password) {
    int upper = 0;
    int special = 0;
    long int len = strlen(password);
    for (int i = 0; i < len; i++){
        if (password[i] <= 'A' && password[i] <= 'Z') {
            upper++;
        }
        if (password[i] < 'A' || (password[i] > 'Z' && password[i] < 'a')
            || password[i] > 'z') {
            special++;
        }
    }
    return upper && special && (len >= 8);
}

// make_student(s) passing a pointer to structure; change the content of the elements in structure
// effect: change the content of student structure  
struct student *make_student(struct student *s){
    printf("Please enter student's name: ");
    s->name = inputstr();
    printf("Please enter student's major: ");
    s->major = inputstr();
    printf("Please enter student's current_year: ");
    s->current_level = inputstr();
    printf("Please enter student's login password: ");
    char* pas = inputstr();
    while (!check_password(pas)) {
        printf("Please enter password constains at least one upper and one special char with length of 8!");
        pas = inputstr();
    }
    s->login = pas;
    printf("Please enter student's home address: ");
    s->addr = inputstr();
    printf("Please enter student's number: ");
    s->number = inputstr();
    printf("Please enter student's cell number: ");
    s->cell = inputstr();
    printf("Please enter student's email: ");
    s->email = inputstr();
    printf("Please enter student weight: ");
    int wei = 0;
    scanf("%d", &wei);
    s->weight = wei;
    printf("Please enter student height: ");
    int hei = 0;
    scanf("%d", &hei);
    s->height = hei;
    printf("Please enter student's GPA: ");
    int gp = 0;
    scanf("%d", &gp);
    s->current_gpa = gp;
    printf("Is this student in coop? ");
    s->coop = inputstr();
    s->next = NULL;
    return s;
}


// enrol_student(s) by passing a structure school to the function, initialize the new
// structure for student, malloc certain amount of memory from heap; add the node to the
// proper position by using index (student number) that user provide
// effect: add node to the linked list
void enrol_student(struct school *s) {
    struct student *new = malloc(sizeof(struct student));
    new = make_student(new);
    if (s->stdd == NULL) {
        s->stdd = new;
    } else {
        struct student *cur = s->stdd;
        //struct student *pre = NULL;
        while (cur) {
            if (!cur->next || (strcmp(cur->number, new->number) < 1 && strcmp(cur->next->number, new->number) > 1)) break;
            cur = cur->next;
        }
        if (cur->next == NULL) cur->next = new;
        else {
            new->next = cur->next->next;
            cur->next = new;
        }
    }
}


// search_student(s, number) using student number to be an index to search the whole linked list
// effect: produce output
struct student *search_student(struct school *s, char* number) {
    struct student *std = s->stdd;
    while (std) {
        if (!strcmp(std->number , number)) return std;
        std = std->next;
    }
    printf("There is no such student! ");
    return NULL;
}

// kick_out(s) remove the certain node by passing the structure school and user's input
// user's input can be an index to search node
// effect: delete a node and make change to the list
void kick_out(struct school *s) {
    printf("Please enter the student number: ");
    char *number = inputstr();
    struct student *std = s->stdd;
    struct student *prev = NULL;
    while (std) {
        if (!strcmp(std->number, number)) {
            break;
        }
        prev = std;
        std = std->next;
    }
    if (prev == NULL) {
        struct student *new = std->next;
        destroy_student(std);
        s->stdd = new;
    } else {
        prev->next = std->next;
        destroy_student(std);
    }
}


// display(s) need to use password or school pin code to check the student information for security
// there is a count down only three times only
// effect: produce output
void display(struct school *s) {
    printf("Please enter the student number: ");
    char* number = inputstr();
    struct student *std = search_student(s, number);
    if (std == NULL) return;
    printf("Please enter student login password or school pin code: ");
    char* input = inputstr();
    int check = 3;
    while (check > 0) {
        if (!strcmp(input, s->authority_number) || !strcmp(input, std->login)) {
            printf("School name: %s", s->name);
            printf("Student name: %s \n", std->name);
            printf("Student number: %s \n", std->number);
            printf("Student major: %s \n", std->major);
            printf("Student current_year: %s \n", std->current_level);
            printf("Student name: %d \n", std->current_gpa);
            printf("Student's height: %d \n", std->height);
            printf("Student's weight: %d \n", std->weight);
            printf("Student email: %s \n", std->email);
            printf("Student home address: %s \n", std->addr);
            printf("Student cell number: %s \n", std->cell);
            printf("Is this student in coop? %s \n", std->coop);
            return;
        }
        printf("Enter again: ");
        input = inputstr();
        check--;
    }
    printf("You have used all your chance to enter! You are not allowed to look through everthing! ");
}


int main() {
    struct school *s = build_school();
    printf("Enter the number of students that you want to enroll!");
    int number = 0;
    scanf("%d", &number);
    for (int i = 0; i < number; i++) {
        enrol_student(s);
    }
    char c;
    while (1) {
        printf("Do you want to see student file? y/n");
        scanf("%c", &c);
        if (c == 'n') break;
        display(s);
    }
    destroy_school(s);
    return 0;
}




























