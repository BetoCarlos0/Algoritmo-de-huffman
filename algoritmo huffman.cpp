#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define getbit(by, pos)  ((by >> pos)&0x1)

typedef struct node_t {
	struct node_t *left, *right;
	int freq;
	char c;
} *node;
 
struct node_t pool[256] = {{0}};
node qqq[255], *q = qqq - 1;
int n_nodes = 0, qend = 1;
char *code[128] = {0}, buf[1024];
 
node new_node(int freq, char c, node a, node b)
{
	node n = pool + n_nodes++;
	if (freq) n->c = c, n->freq = freq;
	else {
		n->left = a, n->right = b;
		n->freq = a->freq + b->freq;
	}
	return n;
}

void byte2bits(char c, char* s)
{
    int i = 7;
    while(i >= 0)
        *s++ = '0' + getbit(c, i--);
    *s = 0;
}

/* priority queue */
void qinsert(node n)
{
	int j, i = qend++;
	while ((j = i / 2)) {
		if (q[j]->freq <= n->freq) break;
		q[i] = q[j], i = j;
	}
	q[i] = n;
}
 
node qremove()
{
	int i, l;
	node n = q[i = 1];
 
	if (qend < 2) return 0;
	qend--;
	while ((l = i * 2) < qend) {
		if (l + 1 < qend && q[l + 1]->freq < q[l]->freq) l++;
		q[i] = q[l], i = l;
	}
	q[i] = q[qend];
	return n;
}
 
/* walk the tree and put 0s and 1s */
void build_code(node n, char *s, int len)
{
	static char *out = buf;
	if (n->c) {
		s[len] = 0;
		strcpy(out, s);
		code[n->c] = out;
		out += len + 1;
		return;
	}
 
	s[len] = '0'; build_code(n->left,  s, len + 1);
	s[len] = '1'; build_code(n->right, s, len + 1);
}
 
void init(const char *s)
{
	int i, freq[128] = {0};
	char c[16];
 
	while (*s) freq[(int)*s++]++;
 
	for (i = 0; i < 128; i++)
		if (freq[i]) qinsert(new_node(freq[i], i, 0, 0));
 
	while (qend > 2) 
		qinsert(new_node(0, 0, qremove(), qremove()));
 
	build_code(q[1], c, 0);
}
 
void compressao(const char *s, char *out)
{
	while (*s) {
		strcpy(out, code[*s]);
		out += strlen(code[*s++]);
	}
}
 
void descompressao(FILE *file2, const char *s, node t)
{
	char texto[257];
	int i=0;
	node n = t;
	while (*s) {
		if (*s++ == '0') n = n->left;
		else n = n->right;
		
		
		if (n->c){
			fputchar(n->c);
			texto[i] = n->c;
			++i;
			n = t;
		}
	}
 	fputs(texto,file2);
	putchar('\n');
	if (t != n) printf("Entrada de lixo\n");
}

copiarConteudo(FILE *file2, char text[257]){

	fputs(text, file2);
}

int main()
{
	FILE *file1 = fopen("arquivos/originais/arquivo10.txt", "r");
	FILE *file2 = fopen("arquivos/descompressoes/descompressao10.txt","w");
	FILE *file3 = fopen("arquivos/descompressoes binarias/descompressaoBinaria10.txt","w");
	FILE *file4 = fopen("arquivos/compressoes binarias/compressaoBinaria10.txt","w");
	struct timeb ini, fim;
	int tempo;
	srand(time(NULL));
	int i;
	char text[257], bits[9], c, buf[1024];
	const char *texto;
    texto = &text[0];

	ftime(&ini);
	while(fgets(text,257, file1) != NULL);
	fclose(file1);
	
	for(i=0; i < strlen(text); i++){
		c = text[i];
		byte2bits(c, bits);
		fputs(bits,file3);
	}
	fclose(file3);
	init(texto);
	printf("Compressao realizada\n");
	for (i=0; i < 128; i++)
		if (code[i]) printf("'%c': %s\n", i, code[i]);

	compressao(texto, buf);
	printf("\nCompressao binaria realizada e salvo no arquivo compressaoBinaria.txt:\n%s\n", buf);
	fputs(buf,file4);
	fclose(file4);

	printf("\nDescompressao do arquivo original feita em descompressao.txt:\n");
	descompressao(file2,buf, q[1]);
	fclose(file2);
	ftime(&fim);
	
	printf("\nDescompressao binaria do arquivo original feita em descompressaoBinaria.txt:\n");
	
 	tempo = (int) (1000.0 * (fim.time - ini.time) + (fim.millitm - ini.millitm));
    printf("\nTempo de compactacao: %u milisegundos\n", tempo);
	return 0;
}
