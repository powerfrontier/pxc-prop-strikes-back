#include "ssl.h"
#include <stdio.h>
#include <string.h>

BIO* bio;

int main(int argc, char** argv) {
char buf[256] = "";
int rvd = 0;
int i = 0;
char input[256] = "";
char a;

bio = BIO_new_connect(argv[1]);
if (bio == NULL) {
printf("Error\n");
return 1;
}
if(BIO_do_connect(bio) <= 0) {
printf("Mala conexión\n");
return 1;
}

scanf("%s", &input);
BIO_write(bio, input, strlen(input));
rvd = BIO_read(bio, buf, 256);
printf("Respuesta: %s\n", buf);

BIO_reset(bio);
BIO_free_all(bio);

printf("FIN\n");
}
