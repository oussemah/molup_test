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

FIL* m_fopen(const char* __filename, const char* __modes)
{
    FRESULT fres;
    ret = malloc(sizeof(FIL));
    if ((fres = f_open(ret, __filename,  fopenparams(__modes))) != FR_OK) { 
        return NULL;
    }
    return ret;
}

int m_fclose(FIL* file)
{
    f_close(file);
}

