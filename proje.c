#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BOYUT 2415

struct n{
  int numara;
  char isim[5];   //Graph icin tanimladigim struct
};
typedef struct n node;

struct m{
  int data;       //BFS yapabilmek icin tanimladigim queue yapisindaki struct
  struct m *next;
};
typedef struct m queue;

void insertword(node *words);
void matrisolustur(int **matris, node *words);
int  testmatris(int **matris, node *words);

int getnode(node *words, char isim[5]);
void enque(int v);
int  deque();
int next();
void bfs(node* words, int **matris);

queue *root = NULL; //Bazi fonksiyonlarin icinde degistigi icin mainin disinda tanimladim
queue *last = NULL; //Queue nin ilk ve son elemanlarini gosteren pointerlar.


int main(){
    node *words = (node *)malloc(sizeof(node)*BOYUT);  // Kelimeleri tuttugum dizi
    int i,flag;    // Bazi islemlerde kontrol icin
    int secim = 1; //Kullanici menusu icin
    int **matris = (int**)malloc(sizeof(int*)*BOYUT);  //Graph iliskilerini gostermek icin matris
    for(i=0;i<BOYUT;i++)                                //tanimladim ve pointer yapisini kullandim.
      matris[i] = (int*)malloc(sizeof(int)*BOYUT);
      insertword(words);  // Kelimeleri txt den alip diziye atadigim fonksiyon
      matrisolustur(matris,words); //kelime dizisiyle graph komsuluk matrisi olusturan fonksiyon


    while(secim != 0 ){// Secim menusu
      printf("\n--------------------------------------------------------------");
      printf("\n\nIki kelime arasindaki baglantiyi ogrenmek icin 1 girin.\n");
      printf("Iki kelime arasindaki donusum adimlarini ogrenmek icin 2 girin.\n");
      printf("Programdan cikmak icin 0 girin.\n");
      scanf("%d",&secim);
      printf("--------------------------------------------------------------");

      if(secim == 1){
        flag = testmatris(matris, words); //
        if(flag == 1)
          printf("\nSonuc : 1 Baglanti var.\n");
          else
          printf("\nSonuc : 0 Baglanti yok.\n");
      }
      if(secim == 2){
        bfs(words ,matris);
      }
  }
}

void insertword(node *words){ // Kelimeleri txt den alip diziye atadigim fonksiyon
  FILE *fp;
  fp = fopen("kelime.txt","r+");
  int i = 0;
  char c;
  do{                       //Dosyadan keliemeleri sirayla okuyup words dizisine atiyoruz
    c = fscanf(fp,"%s",words[i].isim);
    words[i].numara = i;
    i++;
  }while(c != EOF);
  fclose(fp);
}

void matrisolustur(int **matris, node *words){ //kelime dizisiyle graph komsuluk matrisi olusturan fonksiyon
  int i,j,k;
  int count;
  for(i=0; i<BOYUT; i++)
    for(j=0; j<BOYUT; j++){
      count = 0;
      for(k=0;k<5;k++){       //Eger harf farki 1'se matristeki kelimelerin degeri 1 oluyor
        if(words[i].isim[k] == words[j].isim[k])    //Degilse 0
          count++;
      }
      if(count == 4)
        matris[i][j] = matris[j][i] = 1;
      else
        matris[i][j] = matris[j][i] = 0;
    }
}

int  testmatris(int **matris, node *words){//Matrisin dogru olup olmdaigini test etmek icin
  char *kelime1 =(char*)malloc(sizeof(int)*5);
  char *kelime2 =(char*)malloc(sizeof(int)*5);
  int sira1=-1, sira2=-1;
  while(sira1 == -1 || sira2 == -1){  //2 kelime aliniyor
    printf("\n1.kelimeyi girin :");
    scanf("%s",kelime1);
    printf("\n2.kelimeyi girin :");
    scanf("%s",kelime2);
    sira1 = getnode(words, kelime1);  //kelimelerin dizideki yerleri bulunuyor
    sira2 = getnode(words, kelime2);
  }
  return matris[sira1][sira2];        //matriste kelimelerin degerine denk dusen deger return ediliyor
  }

int getnode(node *words , char isim[5]){  //Verilen kelimenin dizideki sirasini bulan fonksiyon
  int i=0;
  while(strcmp(words[i].isim, isim) != 0 && i < BOYUT){  //Eger kelime dizideki i. elemana esitse
    i++;
  }
  if(i >= BOYUT){
    printf("Aranan kelime bulunamadi.\n"); //eger esit degilse -1 dondur
    return -1;
  }
  else
    return i;  //esitse kelimenin sirasini dondur
}

void enque(int v){      //bfs icin queue ya eleman koyan fonk
  if(root == NULL){
    root = (queue*)malloc(sizeof(queue));  //Eger root nullsa elemani roota yerlestir
    root->next = NULL;
    root->data = v;
    last = root;             //last pointeri da rootu gostersin
  }
  else{
    last->next = (queue*)malloc(sizeof(queue)); //Eger root null degilse
    last->next->data = v;                         //lasttan sonraki yere queue  kadar yer ac
    last->next->next = NULL;                      //gelen bilgiyi oraya yerlestir
    last = last->next;                        //last en son yerlestirilen elemani gostersin
  }
}

int  deque(){                     //queue dan eleman silme
  if(root == NULL){
    printf("Yigin bos");
    return -1;
  }
  else{
    int tmpdata;          //ilk eleman olan rootun degerini dondur
    queue *tmp=root;      //root sonraki elemani gostersin
    tmpdata = root->data; // rootunu alanini free fonksiyonu ile bosalttik
    root = root->next;
    free(tmp);
    return tmpdata;     //Rootun bastaki degerini geri dondur
  }
}

int next(){                   //queue'nun basindaki degeri dondurur
    return root->data;
}

void bfs(node *words, int **matris){  //breadth first search
  char *kelime1 = (char*)malloc(sizeof(char)*5);
  char *kelime2 = (char*)malloc(sizeof(char)*5);
  int *visit = (int*)malloc(sizeof(int)*BOYUT);
  int i,j;
  int node1,node2;
  int **atamatris = (int**)malloc(sizeof(int*)*BOYUT);  //Graph iliskilerini gostermek icin matris
  for(i=0;i<BOYUT;i++)                                //tanimladim ve pointer yapisini kullandim.
    atamatris[i] = (int*)malloc(sizeof(int)*BOYUT);
  for(i=0; i < BOYUT;i++)
    for(j=0; j < BOYUT;j++)
      atamatris[i][j] = 0;
  int *dizi =(int*)malloc(sizeof(int)*BOYUT);
  int k=1;
  j=0;

  printf("\nDonusumu arastirilan 1.kelimeyi girin: ");
  scanf("%s",kelime1);
  printf("\nDonusumu arastirilan 2.kelimeyi girin: ");
  scanf("%s",kelime2);
  node1 = getnode(words, kelime1);
  node2 = getnode(words, kelime2);    //Donusumu arastirilan kelimeler alinip
  printf("Donusum : ");               //getnode fonksiyonuyla dizideki yerleri bulundu

  for(i=0; i < BOYUT; i++)            //Visit dizisi sifirlandi.
    visit[i] = 0;

  visit[node1] = 1;                   //ilk alinan kelime node1 visited oldu
  enque(node1);                       // ilk kelime queue'ya koyuldu.


  while(root != NULL && (node2 != (next()))){ //queue bos degilken ve 2. kelimeye ulasilmamisken
    node1 = next();
    deque();
    for(i=0; i < BOYUT; i++){                 //Eger elemanin matriste eslestigi elemanlar visited degilse queueya atiliyor
      if(visit[i] == 0 && matris[node1][i] == 1){
        visit[i] = 1;
        enque(i);
        atamatris[node1][i] = 1;              //Elemanin cocuklari matriste 1 ile isaretlendi
      }
    }
  }

  while(k != 0){
    k=0;
    for(i=0; i < BOYUT; i++){
      if(atamatris[i][node2] == 1){ //Matrisin o sutununda satiri 1 olan atasi oluyor
      dizi[j++] = i;              //Atasini yeni bir diziye sonradan yazdirmak icin koyuyoruz
      node2 = i;                  //Eger atasi olmayan bir node bulursak baslangica kadar gitmis oldugumuz
      k = 1;                      // icin donguden artik cikabiliriz
      }
    }
  }
  if(root != NULL){                         //Eger 2. kelime bulundugu icin donguden cikilmissa
    for(i = j-1 ; i >= 0; i--)
      printf("%s ",words[dizi[i]].isim);//Diziyi tersten yazdiriyoruz.
    printf("%s",words[next()].isim);

    while(root != NULL)           //Birdahaki islemler icin queue bosalana kadar dequeue yap.
    deque();
  }
  else                            //Eger root null oldugu icin donguden cikilmissa
  printf("Donusum yok");
}
