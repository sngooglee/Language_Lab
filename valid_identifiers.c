#include<stdio.h>
#include<conio.h>

int main(){
    char a[50];
    int i, count=0;
    clrscr();
    printf("Enter Identifier");
    scanf("%s", &a);
    for(i=0;a[i]!='\0';i++){
        count++;
    }
    if(count>16){
        printf("Invalid Identifiers");
    }else{
        if((a[0]=='_')||(('a' <= a[0] && a[0]<='z')||('A'<=a[0] && a[0]<='Z') )){
            printf("valid identifiers");
        }else{
            printf("Not valid identifiers");
        }
    }
    getch();
    return 0;
}
