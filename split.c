//měly by ty fce být char **
// jsou void jen abych nemusel nic vracet

//zatím asi nejnadějnější pokus
void my_split(char line[], char separator[]){
    char token[50]; //toto by melo byt dynamicky, ale nechtělo se mi zatím
    char* rest = line; //toto je asi zbytečný
    printf("here\n");
    for(int i = 0; i < 1; i++){
        printf("ggggg: %s\n", line);
        printf("f: %s\n", rest);
    }
    for (int i = 0, j = 0; rest[i] != '\0'; i++, j++){
        //printf("%c\n", rest[i]);
        if (rest[i] == ' '){
            printf("nenee: %s\n", token); //ten string 'token' si nevymazávám, jen se vždycky kouknu na první pozici jestli se tam hodilo to správný, kdybych ho promazával průběžně tak to asi funguje
            j = -1;
            continue;
        }
        else{
            token[j] = rest[i];
        }
    }
    printf("%s\n", token);
}


//dost podobný jako se strtok (v hlavní větvi), ale taky to timeoutne
void my_split(char line[], char separators[]){
    printf("%s\n", line);
    printf("tady\n");
    //char **com_arr = malloc(sizeof(char*));
    char *temp;
    char *rest = line;
    printf("tady2\n");
    while ((temp = strtok_r(rest, separators, &rest))){
        //com_arr[i] = malloc(strlen(temp));
        printf("%s\n", temp);
        //strcpy(com_arr[i], temp);
        //com_arr = realloc(com_arr, (i + 2) * sizeof(char*));
    }
    //return temp;
}