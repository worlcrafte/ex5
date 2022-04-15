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

dayname dayname_day (int day){
    dayname d; 
    d=day;
    errno=0;
    switch (d)
    {
    case MON:
        return 1;
    case TUE :
        return 2;
    case WED: 
        return 3;
    case THU :
        return  4;
    case FRI :
        return 5;
    case SAT:
        return 6;
    case SUN:
        return 7;
    default:
        return THU;
    }
}

char* monthname_str (monthname month){
    monthname m;
    m=month;
    switch (m)
    {
    case JAN:
        return "January";
        break;
    case FEB:
        return "February";
        break;
    case MAR:
        return "March";
        break;
    case APR:
        return "April";
        break;
    case MAY:
        return "May";
        break;
    case JUN:
        return "June";
        break;
    case JUL:
        return "July";
        break;
    case AUG:
        return "August";
        break;
    case SEP:
        return "September";
        break;
    case OCT:
        return "October";
        break;
    case NOV:
        return "November";
        break;
    case DEC:
        return "December";
        break;
    
    default:
    return "error";
        break;
    }
}

dayname weekday (time_t when){
    int tmpNonB =31536000;
    int tmpB = 31622400;
    int bissextile =2;
    int jourAtrouver = 4; //Je premier janvier 1970 était un jeudi.
    int tmpSemaine =604800;
    int tmpJour =86400;
    while (when > tmpB){
        if (bissextile == 4){
            when-=tmpB;
            bissextile = 1;
        }else{
            when-=tmpNonB;
            bissextile++;
        }
    } while (when>tmpSemaine)
    {
        when-=tmpSemaine;
    }while (when>tmpJour){
        if (jourAtrouver==7){
            jourAtrouver=1;
        }else{
            jourAtrouver++;
        }
        when-=tmpJour;
    }
    dayname a= dayname_day(jourAtrouver);
    //printf ("%i",a);
    return a;
}

int leapyear (unsigned int year){
    if (((year % 4 == 0) && (year % 100 != 0)) || (year%400 == 0)){
        return 1;
    }else {
        return 0;
    }
}

date month (time_t w, int bissextile, date d){
    time_t when =w;
    d.month=1;
    int nbJ30 = 2592000;
    int nbj31 = 2678400;
    int nbJ28 = 2419200;
    int nbJ29 = 2505600;
    int fin =1;
    int tmpJour =86400;
    if (bissextile == 4){
        while (fin==1){
            if (d.month == 1 || d.month == 3 || d.month == 5 || d.month==7 || d.month==8 ||d.month == 10 || d.month==12){
                if (when>nbj31)
                {
                    when-=nbj31;
                    d.month++;
                }else{
                    fin=0;
                }
            }else if (d.month==2){
                if (when>nbJ29)
                {
                    when-=nbJ29;
                    d.month++;
                }else{
                    fin=0;
                }
            }else{
                if (when>nbJ30)
                {
                    when-=nbJ30;
                    d.month++;
                }else{
                    fin=0;
                }
            }
            
            //printf("%li\n",when);
        }
    }
    else{
        while (fin==1){
            if (d.month == 1 || d.month == 3 || d.month == 5 || d.month==7 || d.month==8 ||d.month == 10 || d.month==12){
                if (when>nbj31)
                {
                    when-=nbj31;
                    d.month++;
                }else{
                    fin=0;
                }
            }else if (d.month==2){
                if (when>nbJ28)
                {
                    when-=nbJ28;
                    d.month++;
                }else{
                    fin=0;
                }
            }else{
                if (when>nbJ30)
                {
                    when-=nbJ30;
                    d.month++;
                }else{
                    fin=0;
                }
            }
            //printf("%li\n",when);
        }
    }
    while (when>tmpJour){
        if (d.weekday==7){
            d.weekday=1;
        }else{
            d.weekday++;
        }
        when-=tmpJour;
        d.day++;
        // printf("%i\n",d.day);
    }
    //printf("%i",d.month);
    return d;
}

date from_time (time_t when){
    date d;
    d.day=1;
    d.month =1;
    d.year = 1970;
    d.weekday=4;
    int tmpNonB =31536000;
    int tmpB = 31622400;
    int bissextile =2;
    // int tmpSemaine =604800;
    // int tmpJour =86400;
    while (when > tmpB){
        if (bissextile == 4){
            when-=tmpB;
            bissextile = 1;
            if (d.weekday==7){
                d.weekday=2;
            }else if (d.weekday==6){
                d.weekday=1;
            }else{
                d.weekday+=2;
            }
        }else{
            when-=tmpNonB;
            bissextile++;
            if (d.weekday==7){
                d.weekday=1;
            }else{
                d.weekday+=1;
            }
        }
        // printf("%i\n",d.weekday);
        d.year +=1;
    } 
    d=month (when,bissextile,d);
    // while (when>tmpSemaine)
    // {
    //     when-=tmpSemaine;
    // }
    
    return d;
}


int main (void){
    date a=from_time(1658945568);
    // printf("%s\n",dayname_str(a.weekday));
    // printf("%s\n",monthname_str(a.month));
    // printf("%i\n",a.day);
    // printf("%i",a.year);
    printf("%s %i %s %i \n",dayname_str(a.weekday), a.day, monthname_str(a.month), a.year);
    // printf("%i",weekday(1920541143));
    // leapyear(1980);
    // return 0;
}
