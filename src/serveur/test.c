
#include <stdio.h>
#include <stdlib.h>
 
 
int main () {
        char byte;
        FILE *fjpg = fopen("image.jpg","rb");
 
        if (fjpg!=NULL) {
                while (feof(fjpg) == 0)
                {
                        fread (&byte, sizeof(char),1, fjpg);
                        
                        
                } 
                fclose(fjpg);  
        } else {
                printf("Erreur ouverture\n");
        }
        return 0;
}