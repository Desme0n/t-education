/* Файл sem_send.c – программа-клиент */
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "pair.h"
int main()
{
     int shmid; /* ID сегмента разделяемой памяти */
     char *str; /* Передаваемая строка */
     int semid; /* ID набора семафоров */
     struct sembuf semoper[1]; /* Используется semop() */
     /* Получение доступа к разделяемой памяти */
     if ( (shmid = shmget(SEG_KEY, MAXSTRLEN+1, 0600)) < 0) {
           printf("Ошибка shmget()!\n"); return 1;
     }
     /* Подключение сегмента разделяемой памяти */
     if((str=(char *)shmat(shmid,NULL,0))==(char *)(-1)) {
           printf("shmat() error\n"); return 1;
     }
     /* Получение доступа к набору семафоров */
     if ( (semid = semget(SEM_KEY, 1, 0600)) < 0 ) {
           printf("Ошибка semget()!\n");
           return 2;
     }
     /* Получение монопольного доступа к разделяемой памяти */
     /* P-операция */
     semoper[0].sem_num = 0; /* Номер семафора в наборе */
     semoper[0].sem_op = -1; /* операция */
     semoper[0].sem_flg = 0;
     semop(semid, semoper, 1); /* Выполнение операции */
     /* Ввод строки с клавиатуры */
     printf("Введите строку: ");
     fgets(str, MAXSTRLEN, stdin);
     /* Освобождаем ресурс (разделяемую память) */
     /* V-операция */
     semoper[0].sem_num = 0; /* Номер семафора в наборе */
     semoper[0].sem_op = 1; /* операция */
     semoper[0].sem_flg = 0;
     semop(semid, semoper, 1); /* Выполнить операцию */
     /* Отключение сегмента разделяемой памяти */
     shmdt(str);
     return 0;
}
