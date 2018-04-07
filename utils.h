

void createFolder(char * path);
void getDateStr(char str[30]);
unsigned int filesByName(char* names[24], char* dir,
		unsigned int maxNameFiles2Retrieve);
void printFiles(char names[][24], unsigned int file_count);
unsigned int countFiles(char* dir);
unsigned int removeFiles(char* dir, unsigned int maxNumPics, char names[][24]);
