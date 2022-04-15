#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

typedef enum {
    MON = 1,
    TUE = 2,
    WED = 3,
    THU = 4,
    FRI = 5,
    SAT = 6,
    SUN = 7,
}dayname;

typedef enum {
    JAN =1,
    FEB =2,
    MAR =3,
    APR =4,
    MAY =5,
    JUN =6,
    JUL =7,
    AUG =8,
    SEP =9,
    OCT =10,
    NOV =11,
    DEC =12
}monthname;

typedef struct {
    dayname weekday :3;
    unsigned int day : 5; // n'acceptre pas les valeurs n'égatives du au unsigned int 
    monthname month : 4;
    int year :20;
}date;


char* dayname_str (dayname day){
    dayname d; 
    d=day;
    errno=0;
    switch (d)
    {
    case MON:
        return "Monday";
        break;
    case TUE :
        return "Tuesday";
        break;
    case WED: 
        return "Wednesday";
        break;
    case THU :
        return  "Thursday";
        break;
    case FRI :
        return "Friday";
        break;
    case SAT:
        return "Saturday";
        break;
    case SUN:
        return "Sunday";
        break;
    default:
    return "error";
        break;
    }
}


// dayname dayname_day (int day){
//     dayname d; 
//     d=day;
//     errno=0;
//     switch (d)
//     {
//     case MON:
//         return 1;
//     case TUE :
//         return 2;
//     case WED: 
//         return 3;
//     case THU :
//         return  4;
//     case FRI :
//         return 5;
//     case SAT:
//         return 6;
//     case SUN:
//         return 7;
//     default:
//         return THU;
//     }
// }


char* monthname_str (monthname month){
    monthname m;
    m=month;
    switch (m)
    {
    case JAN:
        return "January";
    case FEB:
        return "February";
    case MAR:
        return "March";
    case APR:
        return "April";
    case MAY:
        return "May";
    case JUN:
        return "June";
    case JUL:
        return "July";
    case AUG:
        return "August";
    case SEP:
        return "September";
    case OCT:
        return "October";
    case NOV:
        return "November";
    case DEC:
        return "December";   
    default:
    return "error";
    }
}


int leapyear (unsigned int year){
    if (((year % 4 == 0) && (year % 100 != 0)) || (year%400 == 0)){
        return 1;
    }else {
        return 0;
    }
}

//fonction qui calcule le moi l'année et le jour.
date monthAndDay (time_t when, date d){
    int tmpY = 60 * 60 * 24 * 365.25; //temps d'une année en seconde
    int moisNonB[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Les mois non bissextile
    int moisB[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // ceux qu'ils le sont
    int tmpj=86400;//le temps d'un jour en seconde
    d.year = 1970+when/tmpY;//en when/tmpY on trouve la différence d'année entre 1970 et la date recherché
    when-=when/tmpY*tmpY;//Je retire ce le temps en secondes de toutes les années trouvées.
    if (leapyear(d.year)){//On vérifie si on est sur une nnée bissextile ou non
        while (when>moisB[d.month]*tmpj){// si oui on utilisera la table des mois bissextile
            when-=moisB[d.month]*tmpj; //et la condition d'arret est si when est inferieur au nombre de jour en seconde dans le mois (si il y est inf c'est qu'il y appartient forcément)
            d.month++;
        }
    }
    else{ //sinon
        while (when>moisNonB[d.month]*tmpj){
            when-=moisNonB[d.month]*tmpj;
            d.month++;
        }
    }
    d.month++;//les mois commence à 0 il faut donc rajouté 1 pour s'accorder à notre type enum
    while(when>tmpj){ //on chercher le num du jour.
        d.day++;
        when-=tmpj;
    }
    return d;
}

//Permet de caculer le jour (compris entre 1 et 7 en accord avec notre structure)
dayname weekday (time_t when){
    // int tmpJ = 86400;
    int jour =  4+when/86400%7;
    /*while (when>tmpJ) {
        if (jour==7){
            jour=1;
        }else
        {
            jour++;
        }
        when-=tmpJ;
    }
    */// d=annee(when,d);
    if (jour>7){
        jour-=7;
    }
    return jour;
}

date from_time (time_t when){
    date d;
    d.day=1;//placé à 1 car l'époque commence le premier janvier 
    d.month =0;//initialise à 0 pour notre tableau des mois
    d.year = 1970; // en accord avec l'époque
    d.weekday=4;//le premier jour de l'époque était un jeudi donc 4.
    d=monthAndDay(when,d);//nous permet de calculer année mois num jour
    d.weekday=weekday(when);//le nom du jour
    return d; // on retourne notre structure 
}

int main (void){
    date a = from_time(time(NULL));//time(NULL) noous donne le temps en secode d'ajd à partir de l'époque
    printf("%s %i %s %i \n",dayname_str(a.weekday), a.day, monthname_str(a.month), a.year);
    return 0;
}