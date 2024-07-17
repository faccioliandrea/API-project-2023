#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Vehicles {
    int * arr;
    int size;
};

typedef struct Vehicles Vehicles;

struct Station {

    int km;
    Vehicles * vehicles;
    struct Station *left, *right;
    struct Station *parent;
};

struct node {
    int km;
    struct node *next;
};

typedef struct node * Route;

struct reverse {
    Route route;
    int tappe;
};

typedef struct reverse * ReverseRoute;

typedef struct Station * Station;

// Station (BST) utility functions
struct Station* newStation(int km, Station street);
Station addStation(Station street, Station station);
Station * Search(Station * street, int x);
Station * SearchApprox(Station * street, int x);
Station MIN(Station street);
Station MAX(Station street);
Station Successore(Station street, Station root);
void inOrder(Station street);
Station deleteStation(Station node, int key);

// Station->Vehicles (Heap) utility functions
Vehicles * buildVehiclesHeap(Vehicles * vehicles);
void MaxHeapify(Vehicles * vehicles, int i);
void printHeap(Vehicles* h);
void insert(Vehicles* vehicles, int i);
void addCar(Station * searched,  int autonomia);
void deleteCar(Vehicles* vehicles, int autonomia);

// Route (Linked List) utility functions
void insertInListAtStart(Route *route, int km, int * tappe);
void insertatEnd(Route *route, int km, int * tappe);
void freeRoute(Route * route, int * tappe);
void print(Route head, int* tappe);

// Plan route
void planRoute(Station startingStation, Route * route, Station apex,  int start, int stop, int * tappe, Station root);
ReverseRoute planRouteReverse2(Station startingStation, Station  endingStation, ReverseRoute  route, Station apex,  int start, int stop, int * tappe, Station * root);

int main ()
{
    char command[1000];

    Station street = NULL;

    while(scanf("%s", command)!=EOF){
        if(strcmp(command, "aggiungi-stazione")==0){
            int km;
            if(scanf("%d", &km)){}

            Station * searched = Search(&street, km);
            if(*searched!=NULL){
                printf("non aggiunta\n");
            } else{
                street = newStation(km, street);
            }

        } else if(strcmp(command, "aggiungi-auto")==0){
            int station, autonomia;
            if(scanf("%d", &station)){}
            if(scanf("%d", &autonomia)){}

            Station * searched = Search(&street, station);
            if(*searched==NULL){
                printf("non aggiunta\n");
            } else{
                addCar(searched,  autonomia);
            }


        } else if(strcmp(command, "rottama-auto")==0){
            int station, autonomia;
            if(scanf("%d", &station)){}
            if(scanf("%d", &autonomia)){}

            Station * searched = Search(&street, station);
            if(*searched==NULL){
                printf("non rottamata\n");
            } else{
                deleteCar((*searched)->vehicles, autonomia);
            }

        } else if(strcmp(command, "demolisci-stazione")==0){
            int station;
            if(scanf("%d", &station)){}

            Station * searched = Search(&street, station);
            if(*searched==NULL){
                printf("non demolita\n");
            } else{
                street = deleteStation(street, station);
                printf("demolita\n");
            }


        } else if(strcmp(command, "pianifica-percorso")==0){

            int start, stop;
            Route route=NULL;

            if(scanf("%d", &start)){}
            if( scanf("%d", &stop)){}

            int tappe=0;

            Station * startingStation = Search(&street, start);
            if(start<=stop){
                insertInListAtStart(&route, stop,  &tappe);
                planRoute(*startingStation, &route, *startingStation,start, stop, &tappe, street);
            } else {
                Station * endingStation = Search(&street, stop);
                ReverseRoute  reverse = (ReverseRoute)malloc(sizeof (struct reverse));
                reverse->tappe=tappe;
                reverse->route=NULL;
                reverse = planRouteReverse2(*startingStation, *endingStation, reverse, *endingStation,start, stop, &reverse->tappe, &street);
                route = reverse->route;
                tappe=reverse->tappe;
                insertInListAtStart(&route, start, &tappe);
            }

            if(tappe==0){
                printf("nessun percorso\n");
                freeRoute(&route, &tappe);
            } else {
                print(route, &tappe);
                freeRoute(&route, &tappe);
            }
        }
    }
    return 0;
}

void MaxHeapify(Vehicles * vehicles, int i){
    int l = 2*i+1;
    int r = 2*i+2;
    int posmax = i;
    if(l<vehicles->size && vehicles->arr[l]>vehicles->arr[i]){
        posmax=l;
    } else{
        posmax=i;
    }
    if(r<vehicles->size && vehicles->arr[r]>vehicles->arr[posmax]){
        posmax=r;
    }
    if(posmax!=i){
        int tmp=vehicles->arr[i];
        vehicles->arr[i]=vehicles->arr[posmax];
        vehicles->arr[posmax]=tmp;
        MaxHeapify(vehicles, posmax);
    }
}

Vehicles * buildVehiclesHeap(Vehicles * vehicles){
    int i;
    for(i=((vehicles->size)-2/2); i>=0; i--){
        MaxHeapify(vehicles, i);
    }
    return vehicles;
}

Station addStation(Station street, Station station){
    if(street==NULL){
        return station;
    }
    Station temp;
    if(station->km < street->km){
        temp= addStation(street->left, station);
        street->left= temp;
        temp->parent=street;
    } else if (station->km > street->km){
        temp= addStation(street->right, station);
        street->right= temp;
        temp->parent=street;
    }
    return street;
}

struct Station* newStation(int km, Station street){
    struct Station* addedStation = (struct Station*)calloc(1, sizeof( struct Station));
    addedStation->km = km;
    int size;
    if( scanf("%d", &size)){}

    Vehicles * vehiclesHeap = (Vehicles*)calloc(1, sizeof(Vehicles));
    vehiclesHeap ->size=size;
    vehiclesHeap ->arr=(int*)calloc(512, sizeof(int));

    int i=0;
    while(scanf("%d[^\n]",&vehiclesHeap->arr[i])!=0){
        i++;
    }

    buildVehiclesHeap(vehiclesHeap);
    addedStation->vehicles=vehiclesHeap;
    street= addStation(street, addedStation);
    printf("aggiunta\n");

    return street;
}

void inOrder(Station street){
    if(street!=NULL){
        inOrder(street->left);
        printf("%d ", street->km);
        inOrder(street->right);
    }
}

void printHeap(Vehicles* h){
    for (int i = 0; i < 512; i++) {
        printf("%d ", h->arr[i]);
    }
    printf("\n");
}

void insertInListAtStart(Route *route, int km, int * tappe){
    Route node = (Route) malloc(sizeof(struct node));
    node->km=km;
    if(*tappe>0){
        node->next=*route;
    }
    *route= node;
    * tappe = *tappe +1;
}

void insertatEnd(Route *route, int km, int * tappe){
    int i=0;
    Route node = (Route) malloc(sizeof(struct node));
    node->km=km;
    if(*route == NULL){
        *route = node;
        * tappe = *tappe +1;
        return;
    }
    Route  temp = *route;
    while(i<*tappe-1){
        temp = temp->next;
        i++;
    }
    temp->next = node;
    * tappe = *tappe +1;
}

Station * Search(Station * street, int x){
    if((*street)==NULL || (*street)->km==x){
        return street;
    }
    if((*street)->km<x){
        return Search(&(*street)->right, x);
    } else{
        return Search(&(*street)->left, x);
    }
}

// Find the closest station A to x, with x->km <= A->km
Station * SearchApprox(Station * street, int x){
    if((*street)==NULL || (*street)->km==x){
        return street;
    }
    Station * curr = street;
    if((*street)->km>x){
        if((*street)->left==NULL){
            return street;
        }
        Station * new= SearchApprox(&(*street)->left, x);
        if((*new)->km>=x){
            return new;
        } else{
            return curr;
        }
    } else{
        if((*street)->right==NULL){
            return street;
        }
        Station * new= SearchApprox(&(*street)->right, x);
        if((*new)->km>=x){
            return new;
        } else{
            return curr;
        }
    }
}

Station MIN(Station street){
    Station cur= street;
    while(cur->left!=NULL){
        cur=cur->left;
    }
    return cur;
}

Station MAX(Station street){
    Station cur= street;
    while(cur->right!=NULL){
        cur=cur->right;
    }
    return cur;
}

Station Successore(Station street, Station root){
    if(street->right!=NULL){
        return MIN(street->right);
    }
    Station y= street->parent;
    while(y!=NULL && y->right==street){
        street=y;
        y=y->parent;
    }
    return y;

}

void planRoute(Station startingStation, Route * route, Station apex,  int start, int stop, int * tappe, Station root){
    if(start<=stop){
        if(apex->km==start && (startingStation->vehicles->arr[0])-(stop-start)>=0){
            insertInListAtStart(route, start, tappe);
            return;
        }
        Station next= Successore(startingStation, root);
        if((startingStation->vehicles->arr[0])-(stop-start)<0){
            if(next->km==stop){
                if(route!=NULL){
                    freeRoute(route, tappe);
                }
                return;
            }
            planRoute(next, route, apex,next->km, stop, tappe, root);
        } else{
            insertInListAtStart(route, start, tappe);
            planRoute(apex, route, apex,apex->km, start, tappe, root);
        }
    }
}

void print(Route head, int * tappe) {
    int tapp= *tappe;
    Route current_node = head;
    while ( current_node != NULL) {
        printf("%d", current_node->km);
        tapp=tapp-1;
        if(tapp>0){
            printf(" ");
        }
        if(tapp<1){
            printf("\n");
            return;
        }
        current_node = current_node->next;
    }
    printf("\n");
}

void freeRoute(Route * route, int * tappe){
    Route current = *route;
    Route next;
    while(current!=NULL){
        *tappe=*tappe-1;
        if(*tappe<1){
            free(current);
            *route=NULL;
            return;
        }
        next=current->next;
        current=next;
    }
    *route=NULL;


}

void insert(Vehicles* vehicles, int i) {
    if(vehicles->size<=512){
        int j;
        vehicles->arr[vehicles->size]=i;
        vehicles->size++;
        for(j=((vehicles->size)-2/2); j>=0; j--){
            MaxHeapify(vehicles, j);
        }
    }
}


void addCar(Station * searched, int autonomia){
    insert((*searched)->vehicles, autonomia);
    printf("aggiunta\n");
}

// Delete function for BST (source: API course slides 2023)
Station deleteStation(Station root, int value){
    Station da_canc=NULL;
    Station sottoa=NULL;
    Station * del= Search(&root, value);

    Station delly = *del;
    if(delly->left==NULL || delly->right==NULL){
        da_canc=delly;
    } else{
        da_canc= Successore(delly, NULL);
    }
    if(da_canc->left!=NULL){
        sottoa=da_canc->left;
    } else{
        sottoa=da_canc->right;
    }
    if(sottoa!=NULL){
        sottoa->parent=da_canc->parent;
    }
    if(da_canc->parent==NULL){

    }else if(da_canc==da_canc->parent->left){
        da_canc->parent->left=sottoa;

    } else{
        da_canc->parent->right=sottoa;
    }
    if(da_canc!=delly){
        delly->km=da_canc->km;
        delly->vehicles=da_canc->vehicles;
    }
    free(da_canc);
    da_canc=NULL;
    return root;
}


void deleteCar(Vehicles* vehicles, int autonomia){
    int i;
    for(i=0; i<vehicles->size; i++){
        if(vehicles->arr[i]==autonomia){
            vehicles->arr[i]=vehicles->arr[vehicles->size-1];
            vehicles->arr[vehicles->size-1]=0;
            vehicles->size--;
            for(i=((vehicles->size-2)/2); i>=0; i--){
                MaxHeapify(vehicles, i);
            }
            printf("rottamata\n");
            return;
        }
    }
    printf("non rottamata\n");
}



ReverseRoute planRouteReverse2(Station startingStation, Station endingStation, ReverseRoute  route,Station apex,  int start, int stop, int * tappe, Station * root) {
    ReverseRoute curr= (ReverseRoute) malloc(sizeof (struct reverse));
    curr->tappe=0;
    curr->route=(Route) calloc(1,sizeof (struct node));

    if(startingStation->km-startingStation->vehicles->arr[0] <= stop){
        curr->route->km=stop;
        curr->tappe++;
        return curr;
    } else{
        Station succ= *SearchApprox(root, startingStation->km-startingStation->vehicles->arr[0]);
        if(succ->km==start){
            curr->tappe=-1;
            return curr;
        }
        Station savedSucc= succ;
        Station ipNext= *SearchApprox(root, succ->km-succ->vehicles->arr[0]);
        ReverseRoute ip= (ReverseRoute) malloc(sizeof (struct reverse));
        ip->tappe=0;
        ip->route=NULL;
        ip = planRouteReverse2(succ, endingStation, ip, apex, succ->km, stop, &ip->tappe, root);
        if(ip->tappe==-1){
            return ip;
        }
        if(Successore(succ, *root)->km<start){
            for(succ= Successore(succ, *root); succ->km<start; succ= Successore(succ, *root)){
                Station aux= (*SearchApprox(root,succ->km-succ->vehicles->arr[0]));
                if(aux->km < ipNext->km){
                    ipNext=aux;
                    ReverseRoute newIp= (ReverseRoute) malloc(sizeof (struct reverse));
                    newIp->route=NULL;
                    newIp->tappe=0;
                    newIp = planRouteReverse2(succ, endingStation, newIp, apex, succ->km, stop, &newIp->tappe, root);
                    if(newIp->tappe<=ip->tappe && newIp->route->km<ip->route->km){
                        ip = newIp;
                        savedSucc=succ;
                    }
                }
            }
        }
        if(ip->tappe>0){
            insertInListAtStart(&(ip->route), savedSucc->km, &ip->tappe);
            curr->route=ip->route;
            curr->tappe=ip->tappe;
        }
        return curr;
    }
}
