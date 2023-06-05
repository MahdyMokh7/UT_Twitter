#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FALSE 0
#define TRUE 1
#define MIN_ARRAY_LEN 2


/*...................
Author : Mahdy Mokhtari
Student's ID : 810101515
file : CA3
Date : 1401 / 10 / 11
....................*/


typedef struct content content;
typedef struct toti_user toti_user;

struct content{
    int post_id;
    int like;
    char* post;
    //char** who_liked;
    content* next;
};

struct toti_user {
    content* my_content;
    char* user_name;
    char* password;
    int num_of_posts;
    toti_user* next;
};


char* command_getter(int*);
char* part2_getter(int*);
char* part3_getter(int*);
int decoder (int*, int, char*, char*, char*, toti_user*, toti_user**, toti_user**);
toti_user* signup(char*, char*, toti_user**, toti_user**, toti_user*);
toti_user* login(char*, char*, toti_user**, toti_user**, toti_user*);
toti_user* post(char*, char*, toti_user**, toti_user**, toti_user*);
toti_user* like(char*, char*, toti_user**, toti_user**, toti_user*);
toti_user* logout(toti_user**, toti_user**, toti_user*);
toti_user* delete(char*, toti_user**, toti_user**, toti_user*);
toti_user* info(toti_user**, toti_user**, toti_user*);
toti_user* find_user(char*, toti_user**, toti_user**, toti_user*);
void design(void);
void print_in_file(toti_user*);

int main() {
    int flag_login = 0, counter = 0;
    design();//////kamel kon
    toti_user** tail_users;
    tail_users = (toti_user**)malloc(sizeof(toti_user*));
    *tail_users = (toti_user*)malloc(sizeof(toti_user));
    if(tail_users == NULL) {
        printf("coulnt malloc in main\n");
    }
    toti_user** node_cur_users;
    node_cur_users = (toti_user**)malloc(sizeof(toti_user*));
    *node_cur_users = *tail_users;
    if(node_cur_users == NULL) {
        printf("coulnt malloc in main\n");
    }        
    toti_user* head_users;
    head_users = *tail_users;
    head_users->next = NULL;
    char *command, *part2, *part3;
    while(TRUE) {
        int num_last_part = 0, check_decoder = -1;
        command = command_getter(&num_last_part); /*getting the command*/
        if(num_last_part == 0) {  /*if we have more than one word we will put the second word in part2*/
            part2 = part2_getter(&num_last_part);
            if(num_last_part == 0) {  /*if we have more than 2 words we will get all the rest as a string in part3*/
                part3 = part3_getter(&num_last_part);                
            }
        }
        printf("command:%s  part2:%s  part3:%s  num_last_part:%d\n", command, part2, part3, num_last_part);
        check_decoder = decoder(&flag_login, num_last_part, command, part2, part3, head_users, node_cur_users, tail_users);
        if(check_decoder == FALSE) {
            printf("not a correct command\n");
        }
        fflush(stdin);
        if(counter != 0) {
            if(num_last_part == 3) {
                free(command), free(part2), free(part3);
            }
            else if(num_last_part == 2) {
                free(command), free(part2);
            }
            else {
                free(command);
            }
        }
        counter++;
        print_in_file(head_users);
    }
}

void design(void) {
    system("cls");
    printf(".........................Hi, Wellcome to UT_TOTI...........................\n\nwrite your command...\n");
}

char* command_getter(int* num_last_part) {
    char* command = (char*)malloc(sizeof(char) * MIN_ARRAY_LEN);
    if(command == NULL) {
        printf("couldnt malloc comand\n");
        exit(0);
    }
    char temp;
    int malloc_size = MIN_ARRAY_LEN; 
    while(((temp = getchar()) != ' ') && (temp != '\n')) {
        command = realloc(command, sizeof(char) * malloc_size);
        if(command == NULL) {
            printf("couldnt realloc part3\n");
            exit(0);
        }
        command[malloc_size - 2] = temp;
        malloc_size++;
    }
    if(temp == '\n') {
        (*num_last_part) = 1;
    }
    command[malloc_size - 2] = '\0';
    return command;
}

char* part2_getter(int* num_last_part) {
    char* part2 = (char*)malloc(sizeof(char) * MIN_ARRAY_LEN);
    char temp;
    int malloc_size = MIN_ARRAY_LEN;
    if(part2 == NULL) {
        printf("couldnt malloc part2\n");
        exit(0);
    }
    while(((temp = getchar()) != ' ') && (temp != '\n')) {
        part2 = realloc(part2, sizeof(char) * malloc_size);
        if(part2 == NULL) {
            printf("couldnt realloc part3\n");
            exit(0);
        }
        part2[malloc_size - 2] = temp;
        malloc_size++;
    }
    if(temp == '\n') {
        (*num_last_part) = 2;
    }
    part2[malloc_size - 2] = '\0';
    return part2;
}

char* part3_getter(int* num_last_part) {
    char* part3 = (char*)malloc(sizeof(char) * MIN_ARRAY_LEN);
    char temp;
    int malloc_size = MIN_ARRAY_LEN;
    if(part3 == NULL) {
        printf("couldnt malloc part3\n");
        exit(0);
    }
    while((temp = getchar()) != '\n') {
        part3 = realloc(part3, sizeof(char) * malloc_size);
        if(part3 == NULL) {
            printf("couldnt realloc part3\n");
            exit(0);
        }
        part3[malloc_size - 2] = temp;
        malloc_size++;
    }
    if(temp == '\n') {
        (*num_last_part) = 3;
    }
    part3[malloc_size - 2] = '\0';
    fflush(stdin);
    return part3;
}

int decoder(int* flag_login, int num_last_part, char* command, char* part2, char* part3, toti_user* head_users, toti_user** node_cur_users, toti_user** tail_users) { /*finding witch command the user gave us and doing it*/
    if(strcmp("signup", command) == 0) { 
        if(*flag_login == 1 || num_last_part != 3) {
            return FALSE;
        }
        else {
            toti_user* check = head_users->next;
            while (check != NULL) {
                if(strcmp(check->user_name, part2) == 0) {
                    printf("The username you picked is used\n");
                    return FALSE;
                }
                check = check->next;
            }
            *flag_login = 0;
            *node_cur_users = signup(part2, part3, tail_users, node_cur_users, head_users);
        }
    }
    else if(strcmp("login", command) == 0) {
        if(*flag_login == 1 || num_last_part != 3) {
            return FALSE;
        }
        else {
            *node_cur_users = login(part2, part3, tail_users, node_cur_users, head_users);
            if(*node_cur_users == head_users) {
                printf("The username or password is incorrect\n");
                return FALSE;
            }
            else {
                *flag_login = 1;
            }
        }
    }
    else if(strcmp("post", command) == 0) {
        if((*flag_login) == 0 || num_last_part < 2) {
            return FALSE;
        }
        else {
            *flag_login = 1;
            *node_cur_users = post(part2, part3, tail_users, node_cur_users, head_users);
        }
    }
    else if(strcmp("info", command) == 0) {
        if( num_last_part != 1) {
            return FALSE;
        }
        else {
            *flag_login = 1;
            *node_cur_users = info(tail_users, node_cur_users, head_users);
        }
    }
    else if(strcmp("like", command) == 0) {
        if(*flag_login == 0 || num_last_part != 3) {
            return FALSE;
        }
        else {
            *flag_login = 1;
            *node_cur_users = like(part2, part3, tail_users, node_cur_users, head_users);
        }
    }
    else if(strcmp("logout", command) == 0) {
        if(*flag_login == 0 || num_last_part != 1) {
            return FALSE;
        }
        else {
            *flag_login = 0;
            *node_cur_users = logout(tail_users, node_cur_users, head_users);
        }
    }
    else if(strcmp("delete", command) == 0) {
        if(*flag_login == 0 || num_last_part != 2) {
            return FALSE;
        }
        else {
            *flag_login = 1;
            *node_cur_users = delete(part2, tail_users, node_cur_users, head_users);
        }        
    }
    else if(strcmp("find_user", command) == 0) {
        if(*flag_login == 0 || num_last_part != 2) {
            return FALSE;
        }
        else {
            *flag_login = 1;
            *node_cur_users = find_user(part2, tail_users, node_cur_users, head_users);
        }
    }
    else {
        return FALSE;
    }
    return TRUE;
}  

/*the return of all the bellow functions is the current node that we are logged in to or if we are not its the head of nodes*/

toti_user* signup(char* part2, char* part3, toti_user** tail_users, toti_user** node_cur_users, toti_user* head_users) { 
    int part2_size, part3_size;
    (**tail_users).next = (toti_user*)malloc(sizeof(toti_user));
    if((**tail_users).next == NULL) {
        printf("couldnt malloc signup\n");
    }/*writing the users information and saving it to signup*/
    *tail_users = (**tail_users).next;
    (**tail_users).next = NULL;
    part2_size = strlen(part2);
    part3_size = strlen(part3);
    (**tail_users).user_name = (char*)malloc(sizeof(char) * part2_size);
    (**tail_users).password = (char*)malloc(sizeof(char) * part3_size);
    if((**tail_users).password == NULL || (**tail_users).user_name == NULL) {
        printf("couldnt malloc in user or pass in signup\n");
        exit(0);
    }
    strcpy((**tail_users).user_name, part2); 
    strcpy((**tail_users).password, part3);
    (**tail_users).num_of_posts = 0;
    (**tail_users).my_content = (content*)malloc(sizeof(content)); 
    ((**tail_users).my_content)->next = NULL;
    if((**tail_users).my_content == NULL) {
        printf("couldnt malloc my_content or who_liked in signup\n");
        exit(0);
    }
    printf("signed up successefully\n");
    return head_users;
}

toti_user* login(char* part2, char* part3, toti_user** tail_users, toti_user** node_cur_users, toti_user* head_users) {
    toti_user *cur = head_users;
    toti_user * prev = NULL;

    while(cur != NULL) {/*finding the account that we want to signup in*/
        if(strcmp(cur->user_name, part2) == 0 && strcmp(cur->password, part3) == 0) {
            *node_cur_users = cur;
            printf("logged in to: %s\n", (**node_cur_users).user_name);
            return *node_cur_users;
        }
        prev = cur;
        cur = cur->next;
    }
    return head_users;
}

toti_user* post(char* part2, char* part3, toti_user** tail_users, toti_user** node_cur_users, toti_user* head_users) {
    content* cur = (**node_cur_users).my_content;      
    content* prev = NULL;
    int flag = 0;
    if(cur->next == NULL) {
        flag = 1;
    }
    while (cur != NULL) {  //finding the first empty node in the posts linked list
        prev = cur;
        cur = cur->next;
    }/*adding the post to our content list*/
    (**node_cur_users).num_of_posts++;
    prev->next = (content*)malloc(sizeof(content));
    (prev->next)->next = NULL;
    (prev->next)->like = 0;
    if (flag == 1) { //first post the it posted
        (prev->next)->post_id = 1;
    }
    else {
        (prev->next)->post_id = ((prev->post_id) + 1);
    }
    (prev->next)->post = (char*)malloc(sizeof(char) * (strlen(part2) + strlen(part3) + 1)); 
    strcpy((prev->next)->post, part2);
    strcat((prev->next)->post, " ");
    strcat((prev->next)->post, part3);
    return *node_cur_users;
}

toti_user* like(char* part2, char* part3, toti_user** tail_users, toti_user** node_cur_users, toti_user* head_users) {
    toti_user* cur_user = head_users;
    toti_user* prev_user = NULL;
    int flag = 0; // this flag if it gets 1 means that we liked if 0 we didnt
    while(cur_user != NULL) { // finding the user that has the post we want to like
        if(strcmp(cur_user->user_name, part2) == 0) {
            content* cur_content = cur_user->my_content;
            content* prev_content = NULL;
            while (cur_content != NULL) { // finding the post that we want to like
                if(cur_content->post_id == atoi(part3)) {
                    cur_content->like++;   //we havent strengthened repetitious likes
                    flag = 1;
                    break;
                }
                prev_content = cur_content;
                cur_content = cur_content->next;
            }
            break;
        }
        prev_user = cur_user;
        cur_user = cur_user->next;
    }
    if(flag == 1) {
        printf("post liked successfuly\n");
    }
    else {
        printf("post didnt got liked\n");
    }
    return *node_cur_users;
}

toti_user* info(toti_user** tail_users, toti_user** node_cur_users, toti_user* head_users) {
    int flag = 0;
    content* cur = ((**node_cur_users).my_content)->next;
    content* prev = (**node_cur_users).my_content;
    printf("Username: %s\n",(**node_cur_users).user_name);
    printf("password: %s\n", (**node_cur_users).password);
    while(cur != NULL) {
        printf("post: %s\n", cur->post);
        printf("post_id: %d\n", cur->post_id);
        printf("like: %d\n", cur->like);
        prev = cur;
        cur = cur->next;
        printf("\n");
        flag = 1;
    }
    if(flag == 0) {
        printf("you have no posts yet\n");
    }
    return *node_cur_users;
}

toti_user* logout(toti_user** tail, toti_user** node_cur_users, toti_user* head_users) {
    *node_cur_users = head_users;
    printf("You have logged out successfuly\n");
    return *node_cur_users;
}

toti_user* delete(char* part2, toti_user** tail_users, toti_user** node_cur_users, toti_user* head_users) {
    content* cur = (**node_cur_users).my_content;
    content* prev = NULL;
    int flag = 0;
    while(cur != NULL) { 
        if(cur->post_id == atoi(part2)) { //the post_id we want is found and deleted
            flag = 1;
            prev->next = cur->next;
            free(cur->post);
            free(cur);
            break;
        }
        prev = cur;
        cur = cur->next;
    }
    if(flag == 0) {
        printf("You dont have that post_id\n");
    }
    else {
        (**node_cur_users).num_of_posts--;
        printf("post deleted successfuly\n");
    }
    return *node_cur_users;
}

toti_user* find_user(char* part2, toti_user** tail_users, toti_user** node_cur_users, toti_user* head_users) {
    toti_user* cur_user = head_users;
    toti_user* prev_user = NULL;
    while(cur_user != NULL) {
        if(strcmp(cur_user->user_name, part2) == 0) { //the username got matched and now we are printing its info
            printf("Username: %s\n", cur_user->user_name);
            int flag = 0;
            content* cur = (cur_user->my_content)->next;
            content* prev = cur_user->my_content;
            while(cur != NULL) {
                printf("post: %s\n", cur->post);
                printf("post_id: %d\n", cur->post_id);
                printf("like: %d\n", cur->like);
                prev = cur;
                cur = cur->next;
                printf("\n");
                flag = 1;
            }
            if(flag == 0) {
            printf("you have no posts yet\n");
            }
            break;
        }
        prev_user = cur_user;
        cur_user = cur_user->next;
    }
    return *node_cur_users;
}

void print_in_file(toti_user* head_users) {
    FILE* account = fopen("C:\\Users\\MahdyBeast\\OneDrive\\Desktop\\programing\\C1\\.vscode\\CA3\\account.txt", "w");
    FILE* post = fopen("C:\\Users\\MahdyBeast\\OneDrive\\Desktop\\programing\\C1\\.vscode\\CA3\\post.txt", "w");
    if(post == NULL || account == NULL) {
        printf("couldnt fopen\n");
        exit(0);
    }
    toti_user* cur_user = head_users->next;
    while(cur_user != NULL) {
        int check_1 = fprintf(account, "%s %s %d\n", cur_user->user_name, cur_user->password, cur_user->num_of_posts);
        if(check_1 < 0) {
            printf("couldnt fprintf account.\n");
        }
        content* cur_content = (cur_user->my_content)->next; 
        while(cur_content != NULL) {
            int check_2 = fprintf(post, "%s %s %d\n", cur_content->post, cur_user->user_name, cur_content->like);
            if(check_2 < 0) {
                printf("couldnt fprintf account.\n");
            }
            cur_content = cur_content->next;
        }
        cur_user = cur_user->next;
    }
    fclose(account);
    fclose(post);
}