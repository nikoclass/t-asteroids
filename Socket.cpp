/*
   Socket.cpp

   Copyright (C) 2002-2004 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch
*/


#include "Socket.h"
#include <iostream>

#include "Logger.h"

using namespace std;

int Socket::nofSockets_= 0;

void Socket::Start()
{
    if (!nofSockets_)
    {
        WSADATA info;
        if (WSAStartup(MAKEWORD(2,0), &info))
        {
            throw "Could not start WSA";
        }
    }
    ++nofSockets_;
}

void Socket::End()
{
    WSACleanup();
}

Socket::Socket() : s_(0)
{
    Start();
    // UDP: use SOCK_DGRAM instead of SOCK_STREAM
    s_ = socket(AF_INET,SOCK_STREAM,0);

    if (s_ == INVALID_SOCKET)
    {
        throw "INVALID_SOCKET";
    }

    refCounter_ = new int(1);
}

Socket::Socket(SOCKET s) : s_(s)
{
    Start();
    refCounter_ = new int(1);
};

Socket::~Socket()
{
    if (! --(*refCounter_))
    {
        Close();
        delete refCounter_;
    }

    --nofSockets_;
    if (!nofSockets_) End();
}

Socket::Socket(const Socket& o)
{
    refCounter_=o.refCounter_;
    (*refCounter_)++;
    s_         =o.s_;

    nofSockets_++;
}

Socket& Socket::operator=(Socket& o)
{
    (*o.refCounter_)++;

    refCounter_=o.refCounter_;
    s_         =o.s_;

    nofSockets_++;

    return *this;
}

void Socket::Close()
{
    closesocket(s_);
}

//By NiKo

//1234holaholaholah
//olahola

void Socket::RecibirBytes(char *datos, int &len)
{
    char buf[1024*256];
    int lenRecibido=0;
    int lenEsperado;
    int lenAcumulado=0;


    lenRecibido = recv (s_, buf, 1024*256, 0);

    if(lenRecibido==0 || lenRecibido==SOCKET_ERROR){
        len=-1;
        return;
    }

    lenRecibido-=sizeof(int); //-sizeof(int) por el tamaño (indicado en los primeros 4 bytes)
    lenAcumulado=lenRecibido;

    memcpy(&lenEsperado,buf,sizeof(int));
    memcpy(datos,buf+sizeof(int),lenRecibido);

    while(lenAcumulado<lenEsperado){

        datos+=lenRecibido;
        lenRecibido = recv (s_, buf, 1024*256, 0);
        lenAcumulado+=lenRecibido;

        memcpy(datos,buf,lenRecibido);
    }

    len=lenEsperado;

}

std::string Socket::ReceiveBytes()
{
    std::string ret;
    char buf[1024];

    while (1)
    {
        u_long arg = 0;
        if (ioctlsocket(s_, FIONREAD, &arg) != 0)
            break;

        if (arg == 0)
            break;

        if (arg > 1024)
        {
            cout<<"Error tam del paquete mayor que 1024"<<endl;
            arg = 1024;
        }
        int rv = recv (s_, buf, arg, 0);
        if (rv <= 0) break;

        std::string t;

        t.assign (buf, rv);
        ret += t;
    }

    return ret;
}

std::string Socket::ReceiveLine()
{
    std::string ret;
    while (1)
    {
        char r;

        switch (recv(s_, &r, 1, 0))
        {
        case 0: // not connected anymore;
            // ... but last line sent
            // might not end in \n,
            // so return ret anyway.
            return ret;
        case -1:
            return "";
//      if (errno == EAGAIN) {
//        return ret;
//      } else {
//      // not connected anymore
//      return "";
//      }
        }

        ret += r;
        if (r == '\n')  return ret;
    }
}

void Socket::SendLine(std::string s)
{
    s += '\n';
    send(s_,s.c_str(),s.length(),0);
}

void Socket::SendBytes(char *datos,int len)
{
    char buf[1024*256];

    if(len>1024*256){
        LOG(LogLevel::Error,"Se intento enviar un paquete de mas de 256 kb");
        cout<<"Error,Se intento enviar un paquete de mas de 256 kb"<<endl;
        return;
    }


    memcpy(buf,&len,sizeof(int));
    memcpy(buf+sizeof(int),datos,len);

    int res=send(s_,buf,len+sizeof(int),0);

    if (res == SOCKET_ERROR) {
        //cout<<"Socket: Error: SendBytes failed: "<<WSAGetLastError()<<endl;
        //LOG_MORE(LogLevel::Error,"Socket: Error: SendBytes failed %d",WSAGetLastError());
    }else if(res!=len+sizeof(int)){
        cout<<"Socket: Error: no se envio la cantidad de bytes especificados"<<endl;
        LOG(LogLevel::Error,"Socket: Error: no se envio la cantidad de bytes especificados");
    }



    ///---------------------------///
    ///send(s_,s.c_str(),s.length(),0);
}

SocketServer::SocketServer(int port, int connections, TypeSocket type)
{
    sockaddr_in sa;

    memset(&sa, 0, sizeof(sa));

    sa.sin_family = PF_INET;
    sa.sin_port = htons(port);
    s_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (s_ == INVALID_SOCKET)
    {
        throw "INVALID_SOCKET";
    }

    if (type==NonBlockingSocket)
    {
        u_long arg = 1;
        ioctlsocket(s_, FIONBIO, &arg);
    }

    /* bind the socket to the internet address */
    if (bind(s_, (sockaddr *)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR)
    {
        closesocket(s_);
        throw "INVALID_SOCKET";
    }

    listen(s_, connections);
}

Socket* SocketServer::Accept()
{
    SOCKET new_sock = accept(s_, 0, 0);
    if (new_sock == INVALID_SOCKET)
    {
        int rc = WSAGetLastError();
        if (rc==WSAEWOULDBLOCK)
        {
            return 0; // non-blocking call, no request pending
        }
        else
        {
            throw "Invalid Socket";
        }
    }

    Socket* r = new Socket(new_sock);
    return r;
}

SocketClient::SocketClient(const std::string& host, int port) : Socket()
{
    hostent *he;
    if ((he = gethostbyname(host.c_str())) == 0)
    {
        throw SocketException("No se encontro el host especificado");
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr = *((in_addr *)he->h_addr);
    memset(&(addr.sin_zero), 0, 8);

    // Aca tira cero si es Ok y -1 si dio mal
    if (::connect(s_, (sockaddr *) &addr, sizeof(sockaddr)))
    {
        throw SocketException("No se puede abrir la conexion al host");
    }
}

SocketSelect::SocketSelect(Socket const * const s1, Socket const * const s2, TypeSocket type)
{
    FD_ZERO(&fds_);
    FD_SET(const_cast<Socket*>(s1)->s_,&fds_);
    if (s2)
    {
        FD_SET(const_cast<Socket*>(s2)->s_,&fds_);
    }

    TIMEVAL tval;
    tval.tv_sec  = 0;
    tval.tv_usec = 1;

    TIMEVAL *ptval;
    if (type==NonBlockingSocket)
    {
        ptval = &tval;
    }
    else
    {
        ptval = 0;
    }

    if (select (0, &fds_, (fd_set*) 0, (fd_set*) 0, ptval) == SOCKET_ERROR)
        throw "Error in select";
}

bool SocketSelect::Readable(Socket const* const s)
{
    if (FD_ISSET(s->s_,&fds_)) return true;
    return false;
}
