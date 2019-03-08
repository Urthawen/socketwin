/***********************************
*
* AUTHOR : BOUFFARD-VERCELLI FLORIAN
* GROUP : ACPI
* DATE : 26/02/2019
* PROGRAM : SERVER
* TO COMPILE : gcc -Wall server.c -o server -lws2_32
*
*****************/

/*
ATTENTION ! On ne peut réserver et créer que
des spectacles de l'identifiant 0 à 9
Pareil pour la réservation, 9 places maxi.
*/

#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h> // for close


typedef struct Spectacle Spectacle;

struct Spectacle{
  char id;
  int nbPlace;
};

DWORD WINAPI ThreadFun(LPVOID lpParam){
  printf("Thread running...\n");
  return 0;
}

int main()
{
    printf("===== SERVER: STARTING...\n");
    //Thread
    HANDLE hThread;
    DWORD ThreadID;

    hThread = CreateThread(
      NULL,
      0,
      ThreadFun,
      NULL,
      0,
      &ThreadID
    );

    if(hThread == NULL){
      //GetLastError();
      printf("Thread Creation Failed : ERROR \n");
    }
    else{
      printf("Thread Creation Success\n");
      printf("ThreadID = %lu\n", ThreadID);
    }

    WSADATA WSAData;
    SOCKET sock;
    SOCKET csock;
    SOCKADDR_IN sin;
    SOCKADDR_IN csin;
    char buffer[2];

    // SPECTACLE INIT //
    int nbSpectacle = 0;
    Spectacle spectacle[255];

    Spectacle s1;
    s1.id = '5';
    s1.nbPlace = 256;
    spectacle[nbSpectacle] = s1;
    nbSpectacle++;

    Spectacle s2;
    s2.id = '3';
    s2.nbPlace = 14;
    spectacle[nbSpectacle] = s2;
    nbSpectacle++;

    Spectacle s3;
    s3.id = '6';
    s3.nbPlace = 0;
    spectacle[nbSpectacle] = s3;
    nbSpectacle++;

    // END SPECTACLE INIT //

    WSAStartup(MAKEWORD(2,0), &WSAData);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1"); //Adresse du server
    sin.sin_family = AF_INET;                     //Famille du socket
    sin.sin_port = htons(6667);                   //Port du server
    bind(sock, (SOCKADDR *)&sin, sizeof(sin));    //Attache le server à l'ip et port choisie
    listen(sock, 0);                              //Ecoute le port sur le socket
    printf("===== SERVER: ONLINE\n");
    while(1) /* Boucle infinie.*/
    {
        int sinsize = sizeof(csin);
        if((csock = accept(sock, (SOCKADDR *)&csin, &sinsize)) != INVALID_SOCKET)
        {
            printf("===== SERVER: USER CONNECTED\n");
            //On reçoit les informations send par le client
            recv(csock, buffer, sizeof(buffer), 0);

            //On récupére le nombre de place en INT du buffer
            int nbPlaceVoulu = buffer[1] - 48; //48 via ASCII (Conversion char to int)

            //On va chercher si le spectacle existe
            int i =0;
            int find = 0;
            while(i<nbSpectacle && !find){
              if(buffer[0] == spectacle[i].id){
                find = 1;
              }else{
                i++;
              }
            }

            // Si le spectacle existe
            if(find){
              printf("SERVER: SPECTACLE FOUND...\n");
              //On vérifie son nombre de nombre de place
              printf("SERVER: CHECKING SPECTACLE...\n");
              if(spectacle[i].nbPlace-nbPlaceVoulu >=0){
                spectacle[i].nbPlace-=nbPlaceVoulu;
                printf("SERVER: SPECTACLE ID (%c) UPDATED\n", spectacle[i].id);
                send(csock, "Vos places sont reserves !", 255, 0);
              }else{
                printf("SERVER: SPECTACLE ID (%c) FULL !\n", spectacle[i].id);
                  send(csock, "Pas assez de places disponibles. Desole !!", 255, 0);
              }
            }else{
              printf("SERVER: SPECTACLE NOT FOUND...\n");
            }

            closesocket(csock);
            CloseHandle(hThread);
        }
    }
    /* On devrait faire WSACleanup();
    Mais nous sommes dans une boucle infinie ==> arrêt du server par Ctrl+C*/

    return 0;
}
