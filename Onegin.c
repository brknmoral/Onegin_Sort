#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
//TODO STRCMP AND ONEGIN;

int ReadFromFile(const char* name, char** text, off_t *FILE_SIZE);
void DataOuptut(char* index[], off_t FILE_SIZE);
int CountNumString(char* text);
int FillIndexData(char* text, char* index[], int str_count);

int main(void)
{
    char* text = NULL;
    off_t FILE_SIZE = 0;
    int str_count = 0;
    int ret = ReadFromFile("debug_txt.txt", &text, &FILE_SIZE);

    str_count = CountNumString(text);
    char** index = (char**)calloc(str_count, sizeof(char*));// pointers data
    printf("Before sort:\n");
    printf("%s\n", text);

    printf("After Cleaning:\n");
    str_count = FillIndexData(text,index, str_count);
    DataOuptut(index, str_count);

    printf("---------------------------\n");
    printf("After Sort:\n");
    printf("Sort Number 1:\n");

    return ret;
}

int ReadFromFile(const char* name, char* * text, __off_t *FILE_SIZE)
{
    printf("!\n");
    struct stat file_stat = {};
    //struct stat* tmp = &file_stat;  tmp =0x1000
    //tmp->st_size;
    // file_stat.st_size;

    int ret = 0;
    int file_descriptor = 0;
    FILE* file = NULL;
    off_t size = 0;

    if (name == NULL || text == NULL || FILE_SIZE == NULL)
    {
        ret = -EINVAL;
        goto exit;
    }

    ret = -ENOENT;
    file = fopen(name, "r");
     if (file == NULL)
     {
        fprintf(stderr, "UNpossible to open file (errno = %d)\n", errno);
        goto exit;
     }

    file_descriptor = fileno(file);
    printf("File descriptor: %d\n", file_descriptor);

    fstat(file_descriptor, &file_stat);

    *FILE_SIZE = file_stat.st_size;
    printf("File size : %ld\n", *FILE_SIZE);

    ret = -ENOMEM;

    *text= (char *)calloc(*FILE_SIZE + 1, sizeof(char));

    if (*text == NULL)
    {
        printf("Error memory allocation\n");
        goto exit_close;
    }
    ret = -EIO;

    size = fread(*text ,sizeof(char), *FILE_SIZE, file);
    if (size != *FILE_SIZE)// Верно для строк, тк 1 байт размер char
    {
        if (feof(file))
            printf("Reached end of file (readed %zu out of %ld)\n", size, *FILE_SIZE);
        else if (ferror(file))
            perror("fread");
        goto error_free;
    }
    ret = 0;

    goto exit_close;

error_free:
    free(*text);
    *text = NULL;
exit_close:
    fclose(file);
exit:
    return ret;
}

int FillIndexData(char* text, char* index[], int str_count)
{ 
    int text_length = strlen(text);
    printf("Str_count: %d\n", str_count);
    printf("text_length : %d\n", text_length);
    if (text == NULL || index == NULL || str_count == 0)
    {
        printf("Letter from FillIndData:\n");
        printf("Problem ind_array, text, count of strings\n");
        return -EINVAL;
    }

#if 0
    int idx = 0;
    char *start_ptr = text;
    char *end_ptr = text + text_length;
    char *found = NULL;

    do {
        found = strchr(start_ptr, '\n');
        if (found && (found - start_ptr) > 0) {
            index[idx++] = start_ptr;
            *found = '\0';
        }
        start_ptr = found + 1;
    } while (start_ptr < end_ptr);
    
#else
    int idx = 1;
    index[0] = text;

    int i = 0;
    while (i < text_length)
    {
        if (text[i] == '\n')// text[i+1] \n?
        {
            text[i] = '\0';
            index[idx] = &text[i] + 1;//text + i + 1
            // i++; // <- Bug
            idx++;
        }
        i++;
    }
    printf("Mas Index output:\n");
    for (int i = 0; i < idx; i++){
        printf("[%d] %p -> <%s>\n", i, index[i], index[i]);
    }
    printf("\n");

    for (i = 0, idx = 0; i < str_count; i++) {
        size_t tmp = strlen(index[i]);
        printf("i = %d; idx = %d; strlen(%s) = %zu \n", i, idx, index[i], tmp);
        if (tmp != 0) {
            index[idx++] = index[i];
            index[i] == NULL;
        }
    }
#endif
    return idx;
}

int CountNumString(char* text)
{
    printf("!!!!!\n");
    int len = strlen(text);
    int count = 0;
    printf("%s\n", text);
    printf("??????\n");
    printf("%d\n", len);
    for (int i = 0; i < len; i++)
    {
        printf("Count befor /n: %d\n", count);
        printf("Letter : <%c>\n", text[i]);
        if (text[i] == '\n')
        {
            count ++;
            printf("Count after /n: %d\n", count);
        }
    }

    if (len <= 0)
    {
        return -EIO;
    }

    printf("text[len = %d] = %d <%c>\n", len - 1, text[len - 1], text[len - 1]);
    if (*(text + len) != '\n')
    {
        count += 1;
    }
    return count;
}

void DataOuptut(char* index[], off_t count)
{
    for (int i = 0; i < count; i++)
    {
        printf("[%d] = %p, <%s>\n", i, &index[i], index[i]);
    }
    printf("END\n\n");
}
