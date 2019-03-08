/***********************************
*
* AUTHOR : BOUFFARD-VERCELLI FLORIAN
* GROUP : ACPI
* DATE : 26/02/2019
* PROGRAM : CLIENT
* TO COMPILE : gcc -Wall client.c -o client -lws2_32
*
*****************/

/*
ATTENTION ! On ne peut réserver et créer que
des spectacles de l'identifiant 0 à 9
Pareil pour la réservation, 9 places maxi.
*/

#include <winsock2.h>
#include <stdio.h>

int main()
{
    WSADATA WSAData;
    SOCKET sock;
    SOCKADDR_IN sin;
    char buffer[255] = { 0 };
    WSAStartup(MAKEWORD(2,0), &WSAData);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");
    sin.sin_family = AF_INET;
    sin.sin_port = htons(6667);


    if(connect(sock, (SOCKADDR *)&sin, sizeof(sin)) < 0){
      printf("ERROR: CAN'T CONNECT TO THE SERVER\n");
    }else{
      printf("YOU'RE CONNECTED\n");

      int a;
      printf("Numero Spectacle [0-9] : ");
      scanf("%d",&a);
      buffer[0] = a + 48; // 48 via ASCII (Conversion int to char)


      printf("Nombre de place [0-9] : ");
      scanf("%d",&a);
      buffer[1] = a + 48; // 48 via ASCII (Conversion int to char)

      //On envoie les données au server
      send(sock, buffer, strlen(buffer)+10, 0);
    }
    //On reçoit l'eventuelle réponse du server
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s\n", buffer);
    closesocket(sock);
    printf("DISCONNECTED...\n");
    WSACleanup();
    return 0;
}
