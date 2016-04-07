#include "main.h"

#include <stdarg.h>
#include <stdio.h>

static     FIL *ret;

/*
 * parse string params to a flags byte to be passed to f_open
 */
uint8_t fopenparams(const char* modes)
{
    uint8_t i = 0, done = 0;
    uint8_t flags = 0;

    while (!done) {
        switch (modes[i]) {
            case 'r':
                flags |= FA_READ;
                break;
            case 'w':
                flags |= FA_WRITE | FA_CREATE_ALWAYS | FA_OPEN_ALWAYS;
                break;
            case 'a':
                flags |= FA_READ | FA_WRITE | FA_APPEND | FA_OPEN_ALWAYS; //ToDo: Add implementation for FA_APPEND
                break;
            case 'b':
                break;
            case '+':
                flags |= FA_READ | FA_WRITE | FA_OPEN_ALWAYS;
                break;
            default:
                done = 1;
                break;
        }
        i++;
    }
    return flags;
}

int m_strcmp(const char* s1, const  char* s2)
{
    uint8_t counter = 0;
    if (s1 == NULL || s2 == NULL)
        return -1;
     while ((uint8_t)*s1 != 0 && (uint8_t)*s1 == (uint8_t)*s2)
    {
      s1++;
      s2++;
      if (counter++ > 30) //ToDo: This is a hack, I need to look into the step
                          //by step execution of this and see if I really need this
        break;
    }

    return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}

FIL* m_fopen(const char* __filename, const char* __modes)
{
    FRESULT fres;
    ret = malloc(sizeof(FIL));
    if ((fres = f_open(ret, __filename,  fopenparams(__modes))) != FR_OK) { 
        return NULL;
    }
    return ret;
}

int m_fread(void *buf, size_t size, size_t count, FIL* fp)
{
    /* copy count * size bytes from fp into buf */
    FRESULT fres;
    uint32_t br;
    if (fres = f_read (fp, buf, size*count, &br) != FR_OK) {
        return 0;
    } else {
        return br;
    }
}
/* Sample call :
 * m_fscanf(file, " %4d > %7s %f", int*, char*, float*);
 * The idea is :
 * handle each argument in the format string and va_list,
 * and each time preserve the position of last arg in input string
 */
#define READ_SIZE 20
#define MIN_SIZE  3

enum {
    NO_VAR = 0,
    VOID_VAR,
    INT_VAR,
    STR_VAR,
    CHR_VAR,
    UINT_VAR,
    FLOAT_VAR
};

char* get_next_str_fmt( const char* fmt, char* tmp_fmt, uint32_t *valid_arg)
{
    *valid_arg = NO_VAR;
    do {
        *tmp_fmt = *fmt;
        switch (*tmp_fmt) {
            case '%':
                *valid_arg = VOID_VAR;
                break;
            case 'd':
            case 'x':
            case 'o':
                if (*valid_arg == VOID_VAR)
                    *valid_arg = INT_VAR;
                break;
            case 's':
                if (*valid_arg == VOID_VAR)
                    *valid_arg = STR_VAR;
                break;
            case 'c':
                if (*valid_arg == VOID_VAR)
                    *valid_arg = CHR_VAR;
                break;
            case 'u':
                if (*valid_arg == VOID_VAR)
                    *valid_arg = UINT_VAR;
                break;
            case 'f':
            case 'e':
                if (*valid_arg == VOID_VAR)
                    *valid_arg = FLOAT_VAR;
                break;
            default:
                break;
        }
        tmp_fmt++;
        fmt++;
        if (*valid_arg > VOID_VAR)
            break;
    } while (*fmt != '\0');
    return fmt;
}

char locbuffer[READ_SIZE];
char dummy_buf[READ_SIZE];

int m_fscanf(FIL* fp, const char* fmt, ...)
{
    FRESULT fres = FR_OK;
    uint8_t tmpSize = READ_SIZE, nb_parsed, counter = 0, tmp_len, dummy_len;
    uint32_t valid_arg;
    char next_string_format[8]={0};
    char *tmp_buffer = locbuffer, *backup_ptr;
    char *tmp_fmt = fmt;
    void *var;
    va_list ap;

    unsigned int *tmp_uint;
    int  *tmp_int;
    char *tmp_str;
    float *tmp_flt, dummy_flt;
     
    if (fmt == NULL || fp == NULL) {
        return -1;
    }
    va_start(ap, fmt);
    while ( *tmp_fmt != 0) {
        if (READ_SIZE - tmpSize < MIN_SIZE) {
            if (locbuffer != tmp_buffer) {
                strcpy(locbuffer, tmp_buffer);
            }
            fres = f_read (fp, locbuffer + (READ_SIZE - tmpSize), (tmpSize != READ_SIZE) ? READ_SIZE - tmpSize : READ_SIZE, &valid_arg);
            if (fres != FR_OK) {
                printf("read %d chars -> %s\r\n", valid_arg, locbuffer);
                if (valid_arg == 0)
                    break;
            }
            tmp_buffer = locbuffer;
            tmpSize = 0;
            memcpy(dummy_buf, locbuffer, READ_SIZE);
        }
        tmp_fmt = get_next_str_fmt(tmp_fmt, next_string_format, &valid_arg);
        tmp_len = strlen(next_string_format);
        *(next_string_format+tmp_len)='%';
        *(next_string_format+tmp_len+1)='n';
        *(next_string_format+tmp_len+2)='\0';
        if (valid_arg > NO_VAR) {
            switch (valid_arg) {
                case INT_VAR:
                    tmp_int = va_arg(ap, int);
                    sscanf( tmp_buffer, next_string_format, tmp_int, &nb_parsed);
                    break;
                case STR_VAR:
                    tmp_str = va_arg(ap, char *);
                    sscanf( tmp_buffer, next_string_format, tmp_str, &nb_parsed);
                    break;
                case UINT_VAR:
                    tmp_uint = va_arg(ap, unsigned int *);
                    sscanf( tmp_buffer, next_string_format, tmp_uint, &nb_parsed);
                    break;
                case CHR_VAR:
                    tmp_str = va_arg(ap, char *);
                    sscanf( tmp_buffer, next_string_format, tmp_str, &nb_parsed);
                    break;
                case FLOAT_VAR:
                    tmp_flt = va_arg(ap, float *);
                    sscanf( dummy_buf + tmpSize, next_string_format, &dummy_flt, &nb_parsed);
                    next_string_format[tmp_len]='\0';
                    //printf("input : %s -> ", tmp_buffer);
                    sscanf( tmp_buffer, next_string_format, tmp_flt, &dummy_len);
                    printf( "flt=%f (%d chars)\r\n", *tmp_flt, nb_parsed);
                    break;
                default:
                    tmp_str = va_arg(ap, char *);
                    sscanf( tmp_buffer, next_string_format, tmp_str, &nb_parsed);
                    break;
            }
            tmpSize += nb_parsed;
            tmp_buffer = locbuffer + tmpSize;
            counter++;
        } else {
            break;
        }
    }
    va_end(ap);
    if (tmpSize != READ_SIZE) {
        m_fseek(fp, tmpSize - READ_SIZE, SEEK_CUR);
    }
    return counter;
}

int m_fgetc(FIL* fp)
{
    FRESULT fres;
    uint8_t ret = 0;
    int tmp;
    if (fres = f_read( fp, &ret, 1, &tmp) != FR_OK)
    {
        printf("Err %d while getiing single char\r\n", fres);
        return EOF;
    }
    return (int) ret;
}

long m_ftell(FIL* fp)
{
    if (fp == NULL)
    {
        return -1L;
    }
    return (fp->fptr);
}

int m_fseek(FIL* fp, long offset, int whence)
{
    FRESULT fres;
    long ofs = 0;

    if (fp == NULL) {
        return EOF; //ToDo : May need to redefine EOF
    }
    switch (whence) {
        case SEEK_SET:
            ofs = offset;
            break;
        case SEEK_CUR:
            ofs = fp->fptr + offset;
            break;
        case SEEK_END:
            ofs = fp->fsize - offset;
            break;
        default:
            printf("Uknown whence in m_fseek\r\n");
            return EOF;
    }

    if (fres = f_lseek(fp, ofs) != FR_OK) {
        printf("Error seeking to position %u\r\n", ofs);
        return EOF;
    }

    return 0;
}

int m_fclose(FIL* file)
{
    f_close(file);
}

