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



int main(){

    struct list* newlist = (struct list*)malloc(sizeof(struct list));
    list_init(newlist);

    return 0;
}