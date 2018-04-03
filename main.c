#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>



#define FILE_PATH_MAX_LENTH 256


struct mimedict {
    char extension[8];
    char content_type[128];
};


int main() {
    char mime_file[FILE_PATH_MAX_LENTH] = "/home/liushan/mylab/clab/temp/mime.types";
    FILE * fd_mime;
    const int buffersize = 4096;
    char line[buffersize];
    struct mimedict mimebook[200];
    char content_type_temp[128];
    char extension_temp[8];
    int index_mimebook = 0, index_line_ch, index_extension;
    bool flag_content_type = true, flag_extension = false;
    int myindex;

    if ( (fd_mime = fopen(mime_file, "r") ) == NULL)
    {
        printf("open %s failed\n", mime_file);
    }

    while (fgets(line, buffersize - 1, fd_mime) != NULL)
    {
        // 每行第一个字符不是字母或者数字的，跳过
        if (! isalnum(line[0]) )
            continue;

        // 每行，循环读取每个字符
        for (index_line_ch = 0, index_extension = 0; index_line_ch < strlen(line); index_line_ch++)
        {
            // flag_content_type开关开启时，读取的非空字符存入content_type_temp
            if (flag_content_type)
            {
                // 读到空白字符后，flag_content_type开关关闭
                if (isblank(line[index_line_ch]) )
                {
                    content_type_temp[index_line_ch] = '\0';
                    flag_content_type = false;
                }
                else
                {
                    content_type_temp[index_line_ch] = line[index_line_ch];
                }
            }
            else
            {
                // flag_extension关闭时，读取到空行，跳过
                // flag_extension开启时，读取到空行，进行赋值操作
                if (isblank(line[index_line_ch]) || line[index_line_ch] == ';')
                {
                    if (! flag_extension)
                        continue;
                    extension_temp[index_extension] = '\0';
                    strcpy(mimebook[index_mimebook].content_type, content_type_temp);
                    strcpy(mimebook[index_mimebook].extension, extension_temp);
                    index_mimebook++;
                    flag_extension = false;
                    index_extension = 0;
                    if (line[index_line_ch] == ';')
                    {
                        flag_content_type = true;
                        break;
                    }

                }
                else
                {
                    extension_temp[index_extension] = line[index_line_ch];
                    index_extension++;
                    flag_extension = true;
                }

            }

        }

    }

    char * file_type = "svgz";

    printf("-------------------\n");
    for (myindex = 0; myindex < index_mimebook; myindex++)
    {
         printf("%s    %s\n", mimebook[myindex].content_type,
               mimebook[myindex].extension);
        if (strcmp(file_type, mimebook[myindex].extension) == 0)
            printf("---------- %s %s\n", mimebook[myindex].extension, mimebook[myindex].content_type);
    }
    printf("count is: %d\n", index_mimebook);

    fclose(fd_mime);
}