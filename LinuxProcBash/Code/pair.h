 /* Файл sem_defs.h */
/* Ключ разделяемой памяти */
#define SEG_KEY 0x12345678
/* Ключ набора семафоров */
#define SEM_KEY 0x12345678

/* Максимальная длина передаваемой строки */
#define MAXSTRLEN 100
/* union для вызова semctl() */
union semun {
      int val;
};
