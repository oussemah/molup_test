#include "main.h"

FIL* m_fopen(const char* __filename, const char* __modes);
int m_fread(void *buf, size_t size, size_t count, FIL* fp);
int m_fscanf(FIL* fp, const char* fmt, ...);
int m_fgetc(FIL* fp);
int m_fclose(FIL* file);
