 /* Файл sem_main.c - программа-"сервер" */
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include "pair.h"
int main()
{
      int shmid; /* ID сегмента разделяемой памяти */
      char *str; /* Указатель на принимаемую строку */
      int semid; /* ID набора семафоров */
      int retcode = 1; /* Код возврата из программы */
      union semun semcd; /* Используется semctl() */
      struct sembuf semoper[1]; /* Используется semop() */
      int exitflag=1; /* Флаг выхода из цикла приема строк */
      /* Создание сегмента разделяемой памяти */
      if ( (shmid = shmget(SEG_KEY, MAXSTRLEN+1,
                      IPC_CREAT | IPC_EXCL | 0600)) < 0) {
            printf("Ошибка shmget()!\n"); return 1;
      }
      /* Подключение сегмента разделяемой памяти */
      if((str=(char *)shmat(shmid,NULL,0))==(char *)(-1)) {
            printf("shmat() error\n"); goto destroy_segment;
      }
      /* Создание набора семафоров */
      if ( (semid = semget(SEM_KEY, 1,
                               IPC_CREAT | IPC_EXCL | 0600)) < 0 ) {
            printf("Ошибка semget()!\n");
            goto destroy_segment;
      }
      /* Установка начального значения семафора, равного 1 */
      semcd.val = 1;
      if (semctl(semid, 0, SETVAL, semcd) < 0) {
            printf("Ошибка semctl()!\n");

          goto destroy_semaphore;
     }
     /* Цикл приема строк через разделяемую память */
     while(exitflag) {
          /* Обеспечиваем монопольный доступ к ресурсу
          ( разделяемому сегменту памяти ) */
          /* Р-операция */
          semoper[0].sem_num = 0; /* Номер семафора в наборе */
          semoper[0].sem_op = -1; /* Операция */
          semoper[0].sem_flg = 0;
          semop(semid, semoper, 1); /* Выполнение операции */
          if (str[0]) { /* Если строка имеется ... */
               printf("Принята строка: %s\n", str);
               /* Проверка: передана команда выхода? */
               if (strncmp(str, "EXIT", 4) == 0) {
                      exitflag = 0;
                      printf("ВЫХОД.\n");
               }
               /* Сбрасываем признак наличия строки */
               str[0] = 0;
          }
          /* Освобождаем ресурс */
          /* V-операция */
          semoper[0].sem_num = 0; /* Номер семафора в наборе */
          semoper[0].sem_op = 1; /* Операция */
          semoper[0].sem_flg = 0;
          semop(semid, semoper, 1); /* Выполнение операции */
     } /* Конец цикла приема строк */
     /* Нормальное завершение */
     /* Отсоединение сегмента */
     shmdt(str);
     retcode = 0;
destroy_semaphore:
     /* Уничтожение набора семафоров */
     semctl(semid, IPC_RMID, 0);
destroy_segment:
     /* Уничтожение сегмента разделяемой памяти */
     shmctl(shmid, IPC_RMID, NULL);
     return retcode;
}
