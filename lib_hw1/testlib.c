#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "bitmap.h"
#include "debug.h"
#include "hash.h"
#include "hex_dump.h"
#include "limits.h"
#include "list.h"
#include "round.h"

struct bitmap_arr{
    struct bitmap* bitmap_;
    char bitmap_name[10];
    struct bitmap_arr* next;
};

struct hash_arr{
    struct hash* hash_;
    char hash_name[10];
    struct hash_arr* next;
};

struct list_arr{
    struct list* list_;
    char list_name[10];
    struct list_arr* next;
};

struct bitmap_arr* bitmap_arr_head = NULL;
struct hash_arr* hash_arr_head = NULL;
struct list_arr* list_arr_head = NULL;
int bitmap_arr_s = -1;
int bitmap_arr_e = -1;
int hash_arr_s = -1;
int hash_arr_e = -1;
int list_arr_s = -1;
int list_arr_e = -1;

int create(char* buffer);
int dumpdata(char* buffer);
int delete(char* buffer);
struct bitmap* find_bitmap(char* buffer);
struct hash* find_hash(char* buffer);
struct list* find_list(char* buffer);

// FILE *stdin;

//********* 제출전 할 것
//1. stdin대신 stdin로 fscanf 바꿀 것

int main(int argc, char* argv[]){
    
    //stdin = fopen(argv[1], "r");
    // stdin = fopen(".././os_hw1_tester/bitmap_scan_and_flip.in", "r");

    char buffer[30];

    while(fscanf(stdin, "%s", buffer) != EOF){
    
      
        if(strcmp(buffer, "quit") == 0 || strcmp(buffer, "quit\n") == 0){
            break;
        }

        else if(strcmp(buffer,"create") == 0){//create bitmap, hash, list
            fscanf(stdin, "%s", buffer);//create buffer type data structure
            create(buffer);
        }

        else if(strcmp(buffer, "dumpdata") == 0){//dumpdata bitmap, hash, list
            fscanf(stdin, "%s", buffer);
            dumpdata(buffer);
        }

        else if(strcmp(buffer, "bitmap_mark") == 0){//dumpdata bitmap, hash, list
            size_t idx;
            fscanf(stdin, "%s", buffer);
            struct bitmap* target_bitmap = find_bitmap(buffer);
            fscanf(stdin,"%zu", &idx);
            bitmap_mark(target_bitmap, idx);
        }

        else if(strcmp(buffer, "bitmap_all") == 0){
            size_t start, cnt;
            fscanf(stdin, "%s", buffer);
            struct bitmap* target_bitmap = find_bitmap(buffer);
            fscanf(stdin, "%zu %zu", &start, &cnt);
            printf("%s\n", bitmap_all(target_bitmap, start, cnt)?"true":"false");
        }
        
        else if(strcmp(buffer, "bitmap_any") == 0){
            size_t start, cnt;
            fscanf(stdin, "%s", buffer);
            struct bitmap* target_bitmap = find_bitmap(buffer);
            fscanf(stdin, "%zu %zu", &start, &cnt);
            printf("%s\n", bitmap_any(target_bitmap, start, cnt)?"true":"false");
        }

        else if(strcmp(buffer, "bitmap_contains") == 0){
            size_t start, cnt;
            bool contain;
            fscanf(stdin, "%s", buffer);
            struct bitmap* target_bitmap = find_bitmap(buffer);
            fscanf(stdin, "%zu %zu", &start, &cnt);
            fscanf(stdin, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            printf("%s\n", bitmap_contains(target_bitmap, start, cnt, contain)?"true":"false");
        }

        else if(strcmp(buffer, "bitmap_count") == 0){
            size_t start, cnt;
            bool contain;
            fscanf(stdin, "%s", buffer);
            struct bitmap* target_bitmap = find_bitmap(buffer);
            fscanf(stdin, "%zu %zu", &start, &cnt);
            fscanf(stdin, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            printf("%zu\n", bitmap_count(target_bitmap, start, cnt, contain));

        }

        else if(strcmp(buffer, "bitmap_expand") == 0 ){
            size_t size;
            fscanf(stdin, "%s %zu", buffer, &size);
            struct bitmap* target_bitmap = find_bitmap(buffer);
        
            target_bitmap = bitmap_expand(target_bitmap, size);

            struct bitmap_arr* bitmap_arr_temp = bitmap_arr_head;
            bool bitmap_found = false;

            while(bitmap_arr_temp != NULL){//find bitmap named buffer
                if(strcmp(bitmap_arr_temp->bitmap_name, buffer) == 0){
                    bitmap_found = true;
                    break;
                }
                bitmap_arr_temp = bitmap_arr_temp->next;
            }

            if(bitmap_found)
                bitmap_arr_temp->bitmap_ = target_bitmap;
        }

        else if(strcmp(buffer, "bitmap_flip") == 0 ){
            size_t idx;
            fscanf(stdin, "%s %zu", buffer, &idx);
            struct bitmap* target_bitmap = find_bitmap(buffer);  
            bitmap_flip (target_bitmap, idx);
        }

        else if(strcmp(buffer, "bitmap_none") == 0 ){
            size_t start, cnt;
            fscanf(stdin, "%s %zu %zu", buffer, &start, &cnt);
            struct bitmap* target_bitmap = find_bitmap(buffer);  
            printf("%s\n", bitmap_none (target_bitmap,  start,  cnt)?"true":"false");

        }

        else if(strcmp(buffer, "bitmap_reset") == 0 ){
            size_t idx;
            fscanf(stdin, "%s %zu", buffer, &idx);
            struct bitmap* target_bitmap = find_bitmap(buffer);  
            bitmap_reset (target_bitmap,  idx);
        }

        else if(strcmp(buffer, "bitmap_scan_and_flip") == 0 ){
            size_t start, cnt;
            bool contain;

            fscanf(stdin, "%s %zu %zu", buffer, &start, &cnt);
            struct bitmap* target_bitmap = find_bitmap(buffer);  
            fscanf(stdin, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            printf("%s\n", bitmap_scan_and_flip (target_bitmap,  start,  cnt, contain)?"true":"false");

        }

        else if(strcmp(buffer, "bitmap_scan") == 0 ){
            size_t start, cnt;
            bool contain;

            fscanf(stdin, "%s %zu %zu", buffer, &start, &cnt);
            struct bitmap* target_bitmap = find_bitmap(buffer);  
            fscanf(stdin, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            
            printf("%s\n", bitmap_scan (target_bitmap,  start,  cnt, contain)?"true":"false");

        }

        else if(strcmp(buffer, "bitmap_set_all") == 0 ){
            bool contain;

            fscanf(stdin, "%s", buffer);
            struct bitmap* target_bitmap = find_bitmap(buffer);  
            fscanf(stdin, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            bitmap_set_all (target_bitmap, contain); 
        }

        else if(strcmp(buffer, "bitmap_set_multiple") == 0 ){
            size_t start, cnt;
            bool contain;

            fscanf(stdin, "%s %zu %zu", buffer, &start, &cnt);
            struct bitmap* target_bitmap = find_bitmap(buffer);  
            fscanf(stdin, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            bitmap_set_multiple (target_bitmap,  start, cnt, contain) ;
        }

        else if(strcmp(buffer, "bitmap_set") == 0 ){
            size_t idx;
            bool contain;

            fscanf(stdin, "%s %zu", buffer, &idx);
            struct bitmap* target_bitmap = find_bitmap(buffer);  
            fscanf(stdin, "%s", buffer);
            if(strcmp(buffer, "true") == 0){
                contain = true;
            }
            else if(strcmp(buffer, "false") == 0){
                contain = false;
            }
            bitmap_set (target_bitmap, idx,  contain); 

        }

        else if(strcmp(buffer, "bitmap_test") == 0){
            size_t idx;
            fscanf(stdin, "%s %zu", buffer, &idx);
            struct bitmap* target_bitmap = find_bitmap(buffer);  
            printf("%s\n", bitmap_test(target_bitmap, idx)?"true":"false");
        }

        else if(strcmp(buffer, "bitmap_size") == 0 ){
            size_t size;

            fscanf(stdin, "%s %zu", buffer, &size);
            struct bitmap* target_bitmap = find_bitmap(buffer);  
            
            printf("%zu\n", (size_t) bitmap_size (target_bitmap)); 
        }

        else if(strcmp(buffer, "bitmap_dump") == 0){
            fscanf(stdin, "%s", buffer);
            struct bitmap* target_bitmap = find_bitmap(buffer);
            bitmap_dump(target_bitmap);
        }
        else if(strcmp(buffer, "delete") == 0){
            fscanf(stdin, "%s", buffer);
            delete(buffer);
        }

        //*************************
        //2. hash

        else if(strcmp(buffer, "hash_insert") == 0){
            // size_t ele;
            // fscanf(stdin,"%s %zu", buffer, &ele);
            // struct hash* target_hash = find_hash(buffer);
            // hash_insert(target_hash, ele);
        }

        //*************************
        //3. list

        else if(strcmp(buffer, "list_push_front") == 0 ){
            int i;
            fscanf(stdin, "%s %d", buffer, &i);

            struct list* target_list = find_list(buffer);
            struct list_elem *e = (struct list_elem*) malloc(sizeof(struct list_elem));
            e->prev = &target_list->head;
            struct list_item *temp = list_entry(e, struct list_item, elem);
            temp->data = i;
            list_push_front (target_list, e);

        }

        else if(strcmp(buffer, "list_push_back") == 0 ){
            int i;
            fscanf(stdin, "%s %d", buffer, &i);
            struct list* target_list = find_list(buffer);
            struct list_elem *e = (struct list_elem*) malloc(sizeof(struct list_elem));
            e->next = &target_list->tail;
            struct list_item *temp = list_entry(e, struct list_item, elem);
            temp->data = i;
            list_push_back (target_list, e);



        }

        else if(strcmp(buffer, "list_pop_front") == 0 ){
            int i;
            fscanf(stdin, "%s %d", buffer, &i);
            struct list* target_list = find_list(buffer);
            struct list_elem* e = list_pop_front(target_list);

        }

        else if(strcmp(buffer, "list_pop_back") == 0 ){
            int i;
            fscanf(stdin, "%s %d", buffer, &i);
            struct list* target_list = find_list(buffer);
            struct list_elem* e = list_pop_back(target_list);

        }
        else if(strcmp(buffer, "list_front") == 0 ){
            fscanf(stdin, "%s", buffer);
            struct list* target_list = find_list(buffer);
            struct list_elem *e = list_front(target_list);
            struct list_item *temp = list_entry(e, struct list_item, elem);
            printf("%d\n", temp->data);
        }

        else if(strcmp(buffer, "list_back") == 0 ){
            fscanf(stdin, "%s", buffer);
            struct list* target_list = find_list(buffer);
            struct list_elem *e = list_back(target_list);
            struct list_item *temp = list_entry(e, struct list_item, elem);
            printf("%d\n", temp->data);
        }

        else if(strcmp(buffer, "list_insert_ordered") == 0){
            int val;
            fscanf(stdin, "%s %d", buffer, &val);
            struct list* target_list = find_list(buffer);
            struct list_elem* e = (struct list_elem*)malloc(sizeof(struct list_elem));
            list_insert_ordered (target_list, e, NULL, 0);//help
        }

        else if(strcmp(buffer, "list_insert") == 0){
            int i, val;
            fscanf(stdin, "%s %d %d", buffer, &i, &val);
            struct list* target_list = find_list(buffer);
            struct list_elem* e;
            struct list_elem* new = (struct list_elem*)malloc(sizeof(struct list_elem));
            struct list_item* temp = list_entry(new, struct list_item, elem);
            temp->data = val;
            int cnt = 0;
            for (e = list_begin (target_list); cnt < i; e = list_next (e))
            {
                cnt++;
            }
            list_insert (e, new);           
        }

        else if(strcmp(buffer, "list_empty") == 0){
            fscanf(stdin, "%s", buffer);
            struct list* target_list = find_list(buffer);
            printf("%s\n", list_empty(target_list)?"true":"false");      
        }

        else if(strcmp(buffer, "list_size") == 0){
            fscanf(stdin, "%s", buffer);
            struct list* target_list = find_list(buffer);
            printf("%zu\n", list_size(target_list));      
        }

        else if(strcmp(buffer, "list_max") == 0){
            fscanf(stdin, "%s", buffer);
            struct list* target_list = find_list(buffer);
            struct list_elem* e = list_max(target_list, NULL, 0); //help
            struct list_item* temp = list_entry(e, struct list_item, elem);
            printf("%d\n", temp->data);     
        }

        else if(strcmp(buffer, "list_min") == 0){
            fscanf(stdin, "%s", buffer);
            struct list* target_list = find_list(buffer);
            struct list_elem* e = list_min(target_list, NULL, 0); //help
            struct list_item* temp = list_entry(e, struct list_item, elem);
            printf("%d\n", temp->data);     
        }

        else if(strcmp(buffer, "list_remove") == 0){
            int i;
            fscanf(stdin, "%s %d", buffer, &i);
            struct list* target_list = find_list(buffer);

            int cnt = 0;
            struct list_elem* e;
            struct list_item* temp;
            for (e = list_begin (target_list); cnt < i; e = list_next (e))
            {
                cnt++;
            }
            list_remove (e);       
        }

        else if(strcmp(buffer, "list_reverse") == 0){
            fscanf(stdin, "%s", buffer);
            struct list* target_list = find_list(buffer);
            list_reverse(target_list);
        }

        else if(strcmp(buffer, "list_shuffle") == 0){
            fscanf(stdin, "%s", buffer);
            struct list* target_list = find_list(buffer);
            list_shuffle(target_list);
        }

        else if(strcmp(buffer, "list_splice") == 0){
            int bf, s, e;
            fscanf(stdin, "%s %d", buffer, &bf);
            struct list* target_list1 = find_list(buffer);
            fscanf(stdin, "%s %d %d", buffer, &s, &e);
            struct list* target_list2 = find_list(buffer);
            struct list_elem* before, *first, *last, *temp;
            int cnt = 0;
            for (before = list_begin (target_list1); cnt < bf; before = list_next (before))
            {
                cnt++;
            }
            cnt = 0;
            for (temp = list_begin (target_list2); cnt < s; temp = list_next (temp))
            {
                cnt++;
            }
            first = temp;
            cnt = 0;
            for (temp = list_begin (target_list2); cnt < e; temp = list_next (temp))
            {
                cnt++;
            }
            last = temp;
            list_splice (before,first, last);
        }

        else if(strcmp(buffer, "list_swap") == 0){
            int sw1, sw2;
            fscanf(stdin, "%s %d %d", buffer, &sw1, &sw2);
            struct list* target_list = find_list(buffer);
            struct list_elem* temp1, *temp2, *e;
            int cnt = 0;
            for (e = list_begin (target_list); cnt < sw1; e = list_next (e))
            {
                cnt++;
            }
            temp1 = e;
            cnt = 0;
            for (e = list_begin (target_list); cnt < sw2; e = list_next (e))
            {
                cnt++;
            }
            temp2 = e;
            list_swap(temp1, temp2);
        }

        else if(strcmp(buffer, "list_unique") == 0){
            struct list* list1, *list2;
            fscanf(stdin, "%s", buffer);
            list1 = find_list(buffer);
            fscanf(stdin, "%s", buffer);
            list2 = find_list(buffer);
        void list_unique (struct list *, struct list *duplicates,
                  list_less_func *, void *aux);//help
        }
    }
    
    return 0;
}

int create(char* buffer){
    if(strcmp(buffer,"bitmap") == 0){//create bitmap
        size_t bitsize;
        fscanf(stdin, "%s", buffer);//bitmap name
        fscanf(stdin, "%zu", &bitsize);//bitsize
        struct bitmap_arr* curr = bitmap_arr_head;
        struct bitmap_arr* before = bitmap_arr_head;
        bool create = true;

        while(curr != NULL){
            if(strcmp(curr->bitmap_name, buffer) == 0) {
                create = false;
                break;
            }
            before = curr;
            curr = curr->next;
        }

        if(create){
            struct bitmap_arr* new_bitmap = (struct bitmap_arr*)(malloc(sizeof(struct bitmap_arr)));
            new_bitmap->bitmap_ = bitmap_create(bitsize);
            strcpy(new_bitmap->bitmap_name, buffer);

            if(before == NULL){//new_bitmap is the first bitmap
                new_bitmap->next = NULL;
                bitmap_arr_head = new_bitmap;
                bitmap_arr_s = bitmap_arr_e = 0;
            }

            else{
                new_bitmap->next = curr;
                before->next = new_bitmap;                      
                bitmap_arr_e++;
                    
            }
            
        }
    }
    else if(strcmp(buffer, "hashtable") == 0 ){
        fscanf(stdin, "%s", buffer);//hashtable name

        struct hash_arr* curr = hash_arr_head;
        struct hash_arr* before = hash_arr_head;
        bool create = true;

        while(curr != NULL){
            if(strcmp(curr->hash_name, buffer) == 0) {
                create = false;
                break;
            }
            before = curr;
            curr = curr->next;
        }
        if(create){
            // struct hash_arr* new_hash = (struct hash_arr*)(malloc(sizeof(struct hash_arr)));
            // hash_init(new_hash->hash_, hash_int, NULL, NULL);

            // if(before == NULL){//new_bitmap is the first bitmap
            //     new_hash->next = NULL;
            //     hash_arr_head = new_hash;
            //     hash_arr_s = hash_arr_e = 0;
            // }

            // else{
            //     new_hash->next = curr;
            //     before->next = new_hash;                      
            //     hash_arr_e++;                  
            // }
            
        }
    }
    else if(strcmp(buffer, "list") == 0 ){
        fscanf(stdin, "%s", buffer);//list name
        struct list_arr* curr = list_arr_head;
        struct list_arr* before = list_arr_head;
        bool create = true;

        while(curr != NULL){
            if(strcmp(curr->list_name, buffer) == 0) {
                create = false;
                break;
            }
            before = curr;
            curr = curr->next;
        }
        if(create){
            struct list_arr* new_list = (struct list_arr*)(malloc(sizeof(struct list_arr)));
            new_list->list_ = (struct list*)malloc(sizeof(struct list));
            strcpy(new_list->list_name, buffer);
            list_init(new_list->list_);

            if(before == NULL){//new_list is the first list
                
                new_list->next = NULL;
                list_arr_head = new_list;
                list_arr_s = list_arr_e = 0;
                
            }

            else{
                new_list->next = curr;
                before->next = new_list;                      
                list_arr_e++;                  
            }
            
        }
    }    
    else{
        printf("could not create\n");
        return -1;
    }

    return 0;
}


int dumpdata(char* buffer){//find bitmap, hash, list with named buffer. search bitmap, hash, list in order
    struct bitmap* target_bitmap = find_bitmap(buffer);
    if(target_bitmap != NULL){
        size_t size = bitmap_size(target_bitmap);
        for(size_t i = 0; i < size; i++){
            printf("%zu", (size_t) bitmap_test(target_bitmap, i));
        }
        printf("\n");
        return 0;
    }

    //dump hash
    struct hash* target_hash = find_hash(buffer);
    // if(target_hash != NULL){
    //     struct hash_iterator i;
    //     hash_first (&i, target_hash);
    //     while (hash_next (&i))
    //         {
    //         struct hash *f = hash_entry (hash_cur (&i), struct hash_elem, list_elem);
            
    //         }
    //     return 0;
    //}

    //dump list
    
    struct list* target_list = find_list(buffer);

    if(target_list != NULL){
        struct list_elem* e;
        for (e = list_begin (target_list); e != list_end (target_list); e = list_next (e))
        {
            struct list_item *temp = list_entry(e, struct list_item, elem);
            printf("%d ", temp->data);
        }
        printf("\n");

        return 0;
    }
    return -1; //can't find
}


int delete(char* buffer){//find bitmap, hash, list with named buffer. search bitmap, hash, list in order

    struct bitmap_arr* bitmap_arr_curr = bitmap_arr_head;
    struct bitmap_arr* bitmap_arr_prev = bitmap_arr_head;
    bool bitmap_found = false;

    while(bitmap_arr_curr != NULL){//find bitmap named buffer
        if(strcmp(bitmap_arr_curr->bitmap_name, buffer) == 0){
            bitmap_found = true;
            break;
        }
        bitmap_arr_prev = bitmap_arr_curr;
        bitmap_arr_curr = bitmap_arr_curr->next;
    }

    if(bitmap_found){
        if(bitmap_arr_curr == bitmap_arr_prev){//delete first node
            bitmap_arr_head = bitmap_arr_curr->next;
            free(bitmap_arr_curr);
            bitmap_arr_curr = NULL;
        }
        else{//delete from second node
            bitmap_arr_prev->next = bitmap_arr_curr->next;
            //bitmap_destroy(bitmap_arr_curr->bitmap_);
            //free(bitmap_arr_curr->next);
            free(bitmap_arr_curr);
            bitmap_arr_curr = NULL;
        }
        
        return 0;
    }
        

  
    //find hash

    //find list
    struct list_arr* list_arr_curr = list_arr_head;
    struct list_arr* list_arr_prev = list_arr_head;
    bool list_found = false;

    while(list_arr_curr != NULL){//find list named buffer
        if(strcmp(list_arr_curr->list_name, buffer) == 0){
            list_found = true;
            break;
        }
        list_arr_prev = list_arr_curr;
        list_arr_curr = list_arr_curr->next;
    }

    if(list_found){
        if(list_arr_curr == list_arr_prev){//delete first node
            list_arr_head = list_arr_curr->next;
            free(list_arr_curr);
            list_arr_curr = NULL;
        }
        else{//delete from second node
            list_arr_prev->next = list_arr_curr->next;
            //list_destroy(list_arr_curr->list_);
            //free(list_arr_curr->next);
            free(list_arr_curr);
            list_arr_curr = NULL;
        }
        
        return 0;
    }
    return -1; //can't find
}

struct bitmap* find_bitmap(char* buffer){
    struct bitmap_arr* bitmap_arr_temp = bitmap_arr_head;
    bool bitmap_found = false;

    while(bitmap_arr_temp != NULL){//find bitmap named buffer
        if(strcmp(bitmap_arr_temp->bitmap_name, buffer) == 0){
            bitmap_found = true;
            break;
        }
        bitmap_arr_temp = bitmap_arr_temp->next;
    }

    if(bitmap_found)
        return bitmap_arr_temp->bitmap_;
    else
        return NULL;
}

struct hash* find_hash(char* buffer){
    struct hash_arr* hash_arr_temp = hash_arr_head;
    bool hash_found = false;

    while(hash_arr_temp != NULL){//find hash named buffer
        if(strcmp(hash_arr_temp->hash_name, buffer) == 0){
            hash_found = true;
            break;
        }
        hash_arr_temp = hash_arr_temp->next;
    }

    if(hash_found)
        return hash_arr_temp->hash_;
    else
        return NULL;
}


struct list* find_list(char* buffer){
    struct list_arr* list_arr_temp = list_arr_head;
    bool list_found = false;

    while(list_arr_temp != NULL){//find list named buffer
        if(strcmp(list_arr_temp->list_name, buffer) == 0){
            list_found = true;

            break;
        }
        list_arr_temp = list_arr_temp->next;
    }

    if(list_found){
        return list_arr_temp->list_;
    }
        
    else
        return NULL;
}