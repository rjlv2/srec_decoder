#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void subString(char* inputLine, int start, size_t n, char* dest) {
	char* src = &inputLine[start];
	strncpy(dest, src, n);
	//dest[start+n-1] = '\0';
	//printf("%s\n", dest);
}

int main(int argc, char * argv[]){
	const int maxSize = 47;
	FILE* fp = fopen(argv[1], "r");
	char* inputLine = malloc(maxSize*sizeof(char));
	char* recordType = malloc(2*sizeof(char));
	char* byteCount = malloc(2*sizeof(char));
	char* instr_hex;
	if (fp==NULL) {
		perror("Couldn't open file for reading.");
		exit(1);
	}
	while(fgets(inputLine, maxSize, fp)!=NULL){
		//printf("%s", inputLine);
		
		size_t addr_size;
		long byteCount_long;
		char* end = NULL;
		
		subString(inputLine, 0, 2, recordType);
		subString(inputLine, 2, 2, byteCount);
		switch(recordType[1]){
			case '0':
			case '1':
			case '5':
			case '9':
				addr_size = 16 >> 2; //16 bits = 4 bytes (hex digits)
				break;
			
			case '2':
			case '6':
			case '8':
				addr_size = 24 >> 2;
				break;
				
			case '3':
			case '7':
				addr_size = 32 >> 2;
				break;

			case '4':
			default:
				break;
		} 
		byteCount_long = strtol(byteCount, &end, 16) - 3; //2 bytes addr and 1 byte checksum
		
		instr_hex = (char*)malloc((2*byteCount_long)*sizeof(char));
		subString(inputLine, 4+addr_size, 2*byteCount_long, instr_hex);
		
		int i;
		char instruction[8];
		for(i=0; i<2*byteCount_long; i+=8){
			//instruction = {instr_hex[i+6], instr_hex[i+7], instr_hex[i+4], instr_hex[i+5], instr_hex[i+2], instr_hex[i+3], instr_hex[i], instr_hex[i+1]};
			instruction[0] = instr_hex[i+6];
			instruction[1] = instr_hex[i+7];
			instruction[2] = instr_hex[i+4];
			instruction[3] = instr_hex[i+5];
			instruction[4] = instr_hex[i+2];
			instruction[5] = instr_hex[i+3];
			instruction[6] = instr_hex[i];
			instruction[7] = instr_hex[i+1];
			printf("%s\n", instruction);
		}
		
		//printf("instr hex: %s\n", instr_hex);
		//printf("\n%ld\n", byteCount_long);
		//printf("\n%ld\n\n", addr_size);
		free(instr_hex);
	}
	
}
