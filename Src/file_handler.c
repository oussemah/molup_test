#include "main.h"

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
     while (*s1 != '\0' && (uint8_t)*s1 == (uint8_t)*s2)
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

int m_fclose(FIL* file)
{
    f_close(file);
}

