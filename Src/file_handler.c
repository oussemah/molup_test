#include "main.h"

#include <stdarg.h>

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
    /* cop count * size bytes from fp into buf */
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
#define READ_SIZE 7
#define MIN_SIZE  2

char* get_next_str_fmt( const char* fmt, char* tmp_fmt, uint32_t *valid_arg)
{
    *valid_arg = 0;
    do {
        *tmp_fmt = *fmt;
        if (*tmp_fmt == '%')
            *valid_arg = 1;
        tmp_fmt++;
        fmt++;
    } while (*fmt != ' ' && *fmt != '\0');
    if (*fmt == '\0')
        return NULL;
    else
        return fmt;
}

char locbuffer[20];

int m_fscanf(FIL* fp, const char* fmt, ...)
{
    FRESULT fres = FR_OK;
    uint8_t tmpSize = READ_SIZE, nb_parsed, counter = 0;
    uint32_t valid_arg;
    char next_string_format[READ_SIZE];
    char *tmp_buffer = locbuffer;
    char *tmp_fmt = fmt;
    void *var;
    va_list ap;

    if (fmt == NULL || fp == NULL) {
        return -1;
    }
    va_start(ap, fmt);
    while ( *fmt != 0) {
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
        }
        tmp_fmt = get_next_str_fmt(tmp_fmt, next_string_format, &valid_arg);
        if (valid_arg) {
            va_arg(ap, void *);
            sscanf( tmp_buffer, next_string_format, var, &nb_parsed);
            tmpSize += nb_parsed;
            counter++;
        } else {
            break;
        }
    }
    va_end(ap);
    return counter;
}

int m_fclose(FIL* file)
{
    f_close(file);
}

