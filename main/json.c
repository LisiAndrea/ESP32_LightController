#include <cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"


cJSON *id_obj = NULL;
cJSON *act01 = NULL;
cJSON *tmp_obj = NULL;
cJSON *root = NULL;
cJSON *action = NULL;

int length;
char* ttmp;
char* jString;

bool parse_json(char* tmp){
    root = cJSON_Parse(tmp);
    action = cJSON_GetObjectItem(root, "action");
    if(cJSON_IsFalse(action)){
        cJSON_Delete(root);
        return false;
    }
    cJSON_Delete(root);
    return true;
}

char* build_json(){
    
    act01 = cJSON_CreateObject();

    if (act01 == NULL){
        fprintf(stderr, "Failed to create obj.\n");
        cJSON_Delete(act01);
        return "{}";
    }

    id_obj = cJSON_CreateString("ACT01");
    if (id_obj == NULL){
        fprintf(stderr, "Failed to create string.\n");
        cJSON_Delete(act01);
        return "{}";
    }

    cJSON_AddItemToObject(act01, "sender", id_obj);


    if (rStatus->A){
        tmp_obj =  cJSON_CreateTrue();
    }else{
        tmp_obj =  cJSON_CreateFalse();
    }
    if (tmp_obj == NULL){
        fprintf(stderr, "Failed to create A.\n");
        cJSON_Delete(act01);
        return "{}";
    }
    cJSON_AddItemToObject(act01, "A", tmp_obj);

    if (rStatus->B){
        tmp_obj =  cJSON_CreateTrue();
    }else{
        tmp_obj =  cJSON_CreateFalse();
    }
    if (tmp_obj == NULL){
        fprintf(stderr, "Failed to create B.\n");
        cJSON_Delete(act01);
        return "{}";
    }
    cJSON_AddItemToObject(act01, "B", tmp_obj);

    
    if (rStatus->C){
        tmp_obj =  cJSON_CreateTrue();
    }else{
        tmp_obj =  cJSON_CreateFalse();
    }
    if (tmp_obj == NULL){
        fprintf(stderr, "Failed to create C.\n");
        cJSON_Delete(act01);
        return "{}";
    }
    cJSON_AddItemToObject(act01, "C", tmp_obj);

    
    if (rStatus->D){
        tmp_obj =  cJSON_CreateTrue();
    }else{
        tmp_obj =  cJSON_CreateFalse();
    }
    if (tmp_obj == NULL){
        fprintf(stderr, "Failed to create D.\n");
        cJSON_Delete(act01);
        return "{}";
    }
    cJSON_AddItemToObject(act01, "D", tmp_obj);
    
    tmp_obj =  cJSON_CreateString("");
    if (tmp_obj == NULL){
        fprintf(stderr, "Failed to create details.\n");
        cJSON_Delete(act01);
        return "{}";
    }
    cJSON_AddItemToObject(act01, "details", tmp_obj);


    jString = cJSON_PrintUnformatted(act01);
    cJSON_Delete(act01);
    //free_vars();
    if (jString == NULL)
    {
        fprintf(stderr, "Failed to print json.\n");
        return "{}";
    }else{
        return jString;
    }
   
}
