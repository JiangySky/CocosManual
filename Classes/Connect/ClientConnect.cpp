//
//  ClientConnect.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-4.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "ClientConnect.h"
#include "AppConnect.h"
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
#import <CoreFoundation/CoreFoundation.h>
#endif

static pthread_mutex_t cnmut = PTHREAD_MUTEX_INITIALIZER;

ClientConnect::~ClientConnect()
{
    this->closeSocket();
}

bool ClientConnect::init()
{
    connStatus = kConnectNone;
    listInput.clear();
    listOutput.clear();
    listSocket.clear();
    lastHandle = -1;
    currHandle = -1;
    m_socket = -1;
    m_gamesocket = -1;
    netDelay = 0;
    reconnectCount = 0;
    this->setConnected(false);
#if READ_BUFF
    packetBuff = new SocketInputStream();
    packetBuff->init();
#endif
    listPacket.clear();
    savedPacket.clear();
    deletePacket.clear();
    if (dataMult == 0) {
        inputStream = INSTANCE(SocketInputStream);
        outputStream = INSTANCE(SocketOutputStream);
    } else {
        inputStream = NULL;
        outputStream = NULL;
    }
#ifdef WIN32
    WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData); // win sock start up
	if (ret) {
        return false;
	}
#endif
    return true;
}

bool ClientConnect::connected()
{
    if (!isConnect) {
        return false;
    }
#ifndef __IPHONE_OS_VERSION_MAX_ALLOWED
    if (m_socket < 2 && m_gatesocket < 2 && m_gamesocket < 2) {
        return false;
    }
#endif
#if defined(HEARTBEAT_ENABLE) && HEARTBEAT_ENABLE == 1
    return true;
#else
    int err = 0;
    socklen_t len = sizeof(err);
    int result = getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (char*)&err, &len);
    return (err == 0 && result >= 0);
#endif
}

void ClientConnect::setConnected(bool conn, bool needTip)
{
    isConnect = conn;
    if (needTip) {
#ifdef DISPATCH_ENABEL
        dispatch_async(dispatch_get_main_queue(), ^{
#ifdef CHECK_NETWORK
            AppClient * client = AppConnect::client();
            if (client->uiDelegate && client->uiDelegate->networkEnable()) {
                if (reconnectCount-- > 0) {
                    AppConnect::client()->onReconnectGame();
                } else {
                    client->uiDelegate->lostConnect(false);
                }
            } else {
                client->uiDelegate->lostConnect(true);
            }
#endif
        });
#elif COCOS2D_ENGINE
        AppClient * client = AppConnect::client();
        if (client->uiDelegate && client->uiDelegate->networkEnable()) {
            if (reconnectCount-- > 0) {
                AppConnect::client()->onReconnectGame();
            } else {
                client->uiDelegate->lostConnect(false);
            }
        } else {
            client->uiDelegate->lostConnect(true);
        }
#else
        // other platform or engine
#endif
    }
}

void ClientConnect::sendSavedPacket()
{
    if (connected() && savedPacket.size() > 0) {
        do {
            list<Packet *>::iterator it = savedPacket.begin();
            if (it != savedPacket.end()) {
                sendPacket(*it);
                savedPacket.pop_front();
            }
        } while (savedPacket.size() > 0);
    }
}

int ClientConnect::createSocket(ConnetStatus flag)
{
    switch (flag) {
        case kConnectLoginServer:
            return createLoginSocket();
        case kConnectGateServer:
            return createGateSocket();
        case kConnectGameServer:
            return createGameSocket();
            
        default:
            return 0;
    }
}

void ClientConnect::closeSocket(ConnetStatus flag, bool clear)
{
    pthread_mutex_lock(&cnmut);
    if (socketLimit > 0 || currHandle >= 0) {
        lastHandle = currHandle;
    }
    currHandle = -1;
    listPacket.clear();

    int activeSocket = clear ? 0 : 1;
    if (socketLimit > 0 && listSocket.size() > activeSocket) {
        // close socket
        for (int i = listSocket.size(); i > activeSocket; i--) {
            list<int>::iterator it = listSocket.end();
            it--;
#ifdef WIN32
            closesocket(*it);
#else
            close(*it);
#endif
            ConnUtil::log(ConnUtil::format("----------- remove %i from list -----------", *it));
            listSocket.pop_back();
        }
        // clean data stream
        for (int i = listSocket.size(); i > activeSocket + 1; i--) {
            if (dataMult > 0) {
                list<SocketInputStream *>::iterator itInput = listInput.end();
                itInput--;
                (*itInput)->inputSocket(-1);
                (*itInput)->cleanUp();
                free(*itInput);
                listInput.pop_back();
                list<SocketOutputStream *>::iterator itOutput = listOutput.end();
                itOutput--;
                (*itOutput)->outputSocket(-1);
                (*itOutput)->cleanUp();
                free(*itOutput);
                listOutput.pop_back();
            } else {
                if (inputStream != NULL) {
                    inputStream->cleanUp();
                }
                if (outputStream != NULL) {
                    outputStream->cleanUp();
                }
            }
        }
        if (clear) {
            m_socket = -1;
            m_gatesocket = -1;
            m_gamesocket = -1;
            savedPacket.clear();
            deletePacket.clear();
            pthread_mutex_unlock(&cnmut);
            setConnected(false);
        } else {
            pthread_mutex_unlock(&cnmut);
        }
        return;
    }
    
    if (inputStream != NULL) {
        inputStream->cleanUp();
    }
    if (outputStream != NULL) {
        outputStream->cleanUp();
    }
    switch (flag) {
        case kConnectAll:
#ifdef WIN32
            closesocket(m_socket);
            closesocket(m_gatesocket);
            closesocket(m_gamesocket);
            WSACleanup();
#else
            close(m_socket);
            close(m_gatesocket);
            close(m_gamesocket);
#endif
            m_socket = -1;
            m_gatesocket = -1;
            m_gamesocket = -1;
            break;
            
        case kConnectLoginServer:
#ifdef WIN32
            closesocket(m_socket);
            WSACleanup();
#else
            close(m_socket);
#endif
            m_socket = -1;
            break;
            
        case kConnectGateServer:
#ifdef WIN32
            closesocket(m_gatesocket);
            WSACleanup();
#else
            close(m_gatesocket);
#endif
            m_gatesocket = -1;
            break;
            
        case kConnectGameServer:
#ifdef WIN32
            closesocket(m_gamesocket);
            WSACleanup();
#else
            close(m_gamesocket);
#endif
            m_gamesocket = -1;
            break;
           
        default:
            break;
    }
    pthread_mutex_unlock(&cnmut);
    
    this->setConnected(false);
}

int ClientConnect::createLoginSocket()
{
    int reconn = 0;
    do {
        if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            ConnUtil::log(ConnUtil::format("Socket Error: %i", m_socket));
            this->closeSocket(kConnectLoginServer);
            return -1;
        }
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
        return 0;
#endif
        if (m_socket < 2) {
            usleep(0.5 * 1000000);
        }
    } while (m_socket < 2 && ++reconn < 10);
    return 0;
}

int ClientConnect::createGateSocket()
{
    if((m_gatesocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        ConnUtil::log(ConnUtil::format("Socket Error: %i", m_gatesocket));
        this->closeSocket(kConnectGameServer);
        return -1;
    }
    
    return 0;
}

int ClientConnect::createGameSocket()
{
    if((m_gamesocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        ConnUtil::log(ConnUtil::format("Socket Error: %i", m_gamesocket));
        this->closeSocket(kConnectGameServer);
        return -1;
    }
    
    return 0;
}

int ClientConnect::currentSocket(bool getCount)
{
    if (socketLimit == 0) {
        if (getCount) {
            return 1;
        }
        switch (connStatus) {
            case kConnectLoginServer:
                return m_socket;
            case kConnectGateServer:
                return m_gatesocket;
            case kConnectGameServer:
                return m_gamesocket;
                
            default:
                return -1;
        }
    } else {
        if (getCount) {
            return listSocket.size();
        } else {
            return m_socket;
        }
    }
}

bool ClientConnect::connectServer(std::string ip, int port, ConnetStatus flag)
{
    return this->connectServer(ip, port, flag, false);
}

bool ClientConnect::connectServer(std::string ip, int port, ConnetStatus flag, bool nonblock)
{
    connStatus = flag;
    switch (flag) {
        case kConnectLoginServer:
            return connectLogin(ip, port, nonblock);
        case kConnectGateServer:
            return connectGate(ip, port, nonblock);
        case kConnectGameServer:
            return connectGame(ip, port, nonblock);
            
        default:
            return false;
    }
}

bool ClientConnect::connectLogin(std::string ip, int port)
{
    return this->connectLogin(ip, port, false);
}

bool ClientConnect::connectLogin(std::string ip, int port, bool nonblock)
{
    if (this->createLoginSocket() < 0) {
        return false;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; // family
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str()); // address
    server_addr.sin_port = htons(port); // port
    if (nonblock) { // set to non-blocking
        unsigned long ul = 1;
        ioctl(m_socket, FIONBIO, &ul);
        int flags = fcntl(m_socket, F_GETFL, 0);
        fcntl(m_socket, F_SETFL, flags | O_NONBLOCK);
    }
    if (connect(m_socket, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) < 0 && !nonblock) {
        ConnUtil::log(ConnUtil::format("Cannot connect to %s:%i", ip.c_str(), port));
        return false;
    }
    this->switchSocket(kConnectLoginServer);
    return true;
}

bool ClientConnect::connectGate(std::string ip, int port)
{
    return this->connectGate(ip, port, false);
}

bool ClientConnect::connectGate(std::string ip, int port, bool nonblock)
{
    if (this->createGateSocket() < 0) {
        return false;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; // family
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str()); // address
    server_addr.sin_port = htons(port); // port
    if (nonblock) { // set to non-blocking
        unsigned long ul = 1;
        ioctl(m_gatesocket, FIONBIO, &ul);
        int flags = fcntl(m_gatesocket, F_GETFL, 0);
        fcntl(m_gatesocket, F_SETFL, flags | O_NONBLOCK);
    }
    if (connect(m_gatesocket, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) < 0 && !nonblock) {
        ConnUtil::log(ConnUtil::format("Cannot connect to %s:%i", ip.c_str(), port));
        return false;
    }
    this->switchSocket(kConnectGateServer);
    return true;
}

bool ClientConnect::connectGame(std::string ip, int port)
{
    return this->connectGame(ip, port, false);
}

bool ClientConnect::connectGame(std::string ip, int port, bool nonblock)
{
    if (this->createGameSocket() < 0) {
        return false;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; // family
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str()); // address
    server_addr.sin_port = htons(port); // port
    if (nonblock) { // set to non-blocking        
        unsigned long ul = 1;
        ioctl(m_gamesocket, FIONBIO, &ul);
        int flags = fcntl(m_gamesocket, F_GETFL, 0);
        fcntl(m_gamesocket, F_SETFL, flags | O_NONBLOCK);
    }
    if (connect(m_gamesocket, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) < 0 && !nonblock) {
        ConnUtil::log(ConnUtil::format("Cannot connect to %s:%i", ip.c_str(), port));
        return false;
    }
    this->switchSocket(kConnectGameServer);
    return true;
}

bool ClientConnect::writeDataFinish()
{
    SocketOutputStream * outStream = NULL;
    if (dataMult == 0) {
        outStream = outputStream;
    } else if (listOutput.size() > 0) {
        list<SocketOutputStream *>::iterator it = listOutput.begin();
        outStream = *it;
    }
    if (!outStream || outStream->isEmpty()) {
        return false;
    }
    return true;
}

void ClientConnect::switchSocket(ConnetStatus flag)
{
    pthread_mutex_lock(&cnmut);
    connStatus = flag;
    switch (connStatus) {
        case kConnectLoginServer:
            if (socketLimit > 0) {
                listSocket.push_front(m_socket);
                if (dataMult > 0) {
                    // create data
                    SocketInputStream * inStream = new SocketInputStream();
                    inStream->init();
                    inStream->inputSocket(m_socket);
                    listInput.push_front(inStream);
                    SocketOutputStream * outStream = new SocketOutputStream();
                    outStream->init();
                    outStream->outputSocket(m_socket);
                    listOutput.push_front(outStream);
                } else {
                    inputStream->inputSocket(m_socket);
                    outputStream->outputSocket(m_socket);
                }
                ConnUtil::log(ConnUtil::format("+++++++++++ Add %i to list +++++++++++", m_socket));
            } else {
                inputStream->inputSocket(m_socket);
                outputStream->outputSocket(m_socket);
            }
            break;
        case kConnectGateServer:
            if (socketLimit > 0) {
                listSocket.push_front(m_gatesocket);
                if (dataMult > 0) {                    
                    // create data
                    SocketInputStream * inStream = new SocketInputStream();
                    inStream->init();
                    inStream->inputSocket(m_gatesocket);
                    listInput.push_front(inStream);
                    SocketOutputStream * outStream = new SocketOutputStream();
                    outStream->init();
                    outStream->outputSocket(m_gatesocket);
                    listOutput.push_front(outStream);
                } else {
                    inputStream->inputSocket(m_gatesocket);
                    outputStream->outputSocket(m_gatesocket);
                }
                ConnUtil::log(ConnUtil::format("+++++++++++ Add %i to list +++++++++++", m_gatesocket));
            } else {
                inputStream->inputSocket(m_gatesocket);
                outputStream->outputSocket(m_gatesocket);
            }
            break;
        case kConnectGameServer:
            if (socketLimit > 0) {
                listSocket.push_front(m_gamesocket);
                if (dataMult > 0) {
                    // create data
                    SocketInputStream * inStream = new SocketInputStream();
                    inStream->init();
                    inStream->inputSocket(m_gamesocket);
                    listInput.push_front(inStream);
                    SocketOutputStream * outStream = new SocketOutputStream();
                    outStream->init();
                    outStream->outputSocket(m_gamesocket);
                    listOutput.push_front(outStream);
                } else {
                    inputStream->inputSocket(m_gamesocket);
                    outputStream->outputSocket(m_gamesocket);
                }
                ConnUtil::log(ConnUtil::format("+++++++++++ Add %i to list +++++++++++", m_gamesocket));
            } else {
                inputStream->inputSocket(m_gamesocket);
                outputStream->outputSocket(m_gamesocket);
            }
            break;
            
        default:
            break;
    }
    pthread_mutex_unlock(&cnmut);
#ifdef DISPATCH_ENABEL
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigaction(SIGPIPE, &sa, 0);
#endif
}

#pragma mark - == Process packet ==

int ClientConnect::processSend()
{
    if (currentSocket() < 0) {
        return 0;
    }
    int issend = 100;
    pthread_mutex_lock(&cnmut);
    SocketOutputStream * outStream = NULL;
    if (dataMult == 0) {
        outStream = outputStream;
    } else if (listOutput.size() > 0) {
        list<SocketOutputStream *>::iterator it = listOutput.begin();
        outStream = *it;
    }
    if (outStream == NULL) {
        pthread_mutex_unlock(&cnmut);
        return 0;
    }
    int retSize = outStream->size();
    if (retSize == 0) {
        pthread_mutex_unlock(&cnmut);
        return 0;
    }
    if (retSize > 0 && outStream->isCloseKey) {
        outStream->logInfo();
        issend = outStream->flush();
    }
    pthread_mutex_unlock(&cnmut);
    if (issend <= 0 || (retSize > 0 && !this->connected())) {
        outStream->outputSocket(-1);
        this->setConnected(false);
    }
    return issend;
}

int ClientConnect::processRecv()
{
    if (currentSocket() < 0) {
        return 0;
    }
    pthread_mutex_lock(&cnmut);
    SocketInputStream * inStream = NULL;
    if (dataMult == 0) {
        inStream= inputStream;
    } else if (listInput.size() > 0) {
        list<SocketInputStream *>::iterator it = listInput.begin();
        inStream = *it;
    }
    pthread_mutex_unlock(&cnmut);
    
    if (inStream == NULL) {
        return 0;
    }
    int isrecv;
    isrecv = inStream->receive();
    if (isrecv <= 0) {
        inStream->inputSocket(-1);
        return isrecv;
    }
    inStream->logInfo();

    pthread_mutex_lock(&cnmut);
    int parseStatus = parsePacket();
    switch (parseStatus) {
        case 0: // broken
            break;
        case -1: // error
            inStream->cleanUp();
            break;
        default:
            break;
    }
    pthread_mutex_unlock(&cnmut);

    return isrecv;
}

bool ClientConnect::processCommand()
{
    if (parseDuringRecv) {
        return processPacket();
    }
    int retSize = inputStream->size();
    if (retSize > 0) {
        bool bRet = false;
        char header[PACKET_HEADER_SIZE] = {0};
        uint packetSize = 0;
        ushort packetId = 0;
        ushort errorId = 0;
        ushort encrypt = 0;
        ushort version = 0;
        ushort reserved = 0;
#ifdef PACKET_WITH_JSON
        int compression = 0;
#else
        int readSize = 0;
#endif
        Packet * packet = NULL;
        for (; ; ) {
            pthread_mutex_lock(&cnmut);
            if (!inputStream->peek(&header[0], PACKET_HEADER_SIZE)) { // broken
                pthread_mutex_unlock(&cnmut);
                break;
            }
            if (!inputStream->decrypt()) {
                CNLOG("decrypt head error");
                pthread_mutex_unlock(&cnmut);
                break;
            }
            // head
            memcpy(&packetSize, &header[0], sizeof(uint));
            memcpy(&packetId, &header[sizeof(uint)], sizeof(ushort));
            memcpy(&errorId, &header[sizeof(uint) + sizeof(ushort)], sizeof(ushort));
            memcpy(&encrypt, &header[sizeof(uint) + sizeof(ushort) * 2], sizeof(ushort));
            memcpy(&version, &header[sizeof(uint) + sizeof(ushort) * 3], sizeof(ushort));
            memcpy(&reserved, &header[sizeof(uint) + sizeof(ushort) * 4], sizeof(ushort));
#if DEBUG_PACKET != 0
            if (packetId == DEBUG_PACKET) {
                CNLOG("DEBUG_PACKET");
            }
#endif
            if (packetSize <= 0) {
                CNLOG("packetSize <= 0");
                pthread_mutex_unlock(&cnmut);
                return false;
            }
            if (inputStream->size() < packetSize) { // broken
                ConnUtil::log(ConnUtil::format("Broken package (%i < %i)", inputStream->size(), packetSize));
                pthread_mutex_unlock(&cnmut);
                break;
            }
            if (packetId >= PACKET_MAX) {
                ConnUtil::log(ConnUtil::format("packetId out range %i", packetId));
                pthread_mutex_unlock(&cnmut);
                return false;
            }
            if (!inputStream->skip(PACKET_HEADER_SIZE)) {
                CNLOG("Failed to skip message head");
                pthread_mutex_unlock(&cnmut);
                return false;
            }
            packetSize -= PACKET_HEADER_SIZE;
            // NOTE: process message
            string log = ConnUtil::format("<< Begin process message %i (%i) >>", packetId, packetSize);
            ConnUtil::log(log.c_str(), AppConnect::client()->logToFile);
            packet = INSTANCE(PacketManager)->getPacket(packetId);
            if (NULL == packet) {
                pthread_mutex_unlock(&cnmut);
                return false;
            }
            packet->receivedPacketId = packetId;
#ifdef XML_CONFIG
            packet->errorId = errorId;
            packet->requestErrMsg = INSTANCE(ConfigError)->getErrorMessage(errorId);
#else
            if (errorId != 0 && errorId < ARRAY_LENGTH(ErrMsg)) {
                packet->requestErrMsg = ErrMsg[errorId].c_str();
            }
#endif
            // body
#if READ_BUFF
            packetBuff->cleanUp();
            inputStream->read(packetBuff->getBuffer(), packetSize);
            packetBuff->im_Tail = packetSize;
#endif
#ifdef PACKET_WITH_JSON
#if READ_BUFF
            compression = packetBuff->readInt();
            bRet = packet->setData(packetBuff->readData(packetSize - 4), compression, true);
#else
            compression = inputStream->readInt();
            bRet = packet->setData(inputStream->readData(packetSize - 4), compression, true);
#endif
#else
#if READ_BUFF
            readSize = packet->read(packetBuff);
            // correction packet size
            if (readSize < packetSize) {
                bRet = true;
            } else {
                bRet = (readSize == packetSize);
            }
#else
            readSize = packet->read(inputStream);
            // correction packet size
            if (readSize < packetSize) {
                bRet = inputStream->skip(packetSize - fmax(0, readSize));
            } else {
                bRet = (readSize == packetSize);
            }
#endif
#endif
            pthread_mutex_unlock(&cnmut);

            if (bRet) {
                packet->execute();
                string log = ConnUtil::format("<< E_n_d process message %i (%i) >>", packetId, readSize);
                ConnUtil::log(log, AppConnect::client()->logToFile);
            }
            if (inputStream->size() == 0) {
                break;
            }
        }
        return true;
    }
    return true;
}

int ClientConnect::parsePacket()
{
    SocketInputStream * inStream = NULL;
    if (dataMult == 0) {
        inStream= inputStream;
    } else if (listInput.size() > 0) {
        list<SocketInputStream *>::iterator it = listInput.begin();
        inStream = *it;
    }
    if (inStream == NULL) {
        CNLOG("parsePacket inStream is NULL");
        return -1;
    }
    int retSize = inStream->size();
    if (retSize > 0) {
        bool bRet = false;
        char header[PACKET_HEADER_SIZE] = {0};
        uint packetSize = 0;
        ushort packetId = 0;
        ushort errorId = 0;
        ushort encrypt = 0;
        ushort version = 0;
        ushort reserved = 0;
#ifdef PACKET_WITH_JSON
        int compression = 0;
#else
        int readSize = 0;
#endif
        Packet * packet = NULL;
        for (; ; ) {
            if (!inStream->peek(&header[0], PACKET_HEADER_SIZE)) { // broken
                CNLOG("Broken header");
                return 0;
            }
            if (!inStream->decrypt()) {
                CNLOG("decrypt head error");
                return -1;
            }
            // head
            memcpy(&packetSize, &header[0], sizeof(uint));
            memcpy(&packetId, &header[sizeof(uint)], sizeof(ushort));
            memcpy(&errorId, &header[sizeof(uint) + sizeof(ushort)], sizeof(ushort));
            memcpy(&encrypt, &header[sizeof(uint) + sizeof(ushort) * 2], sizeof(ushort));
            memcpy(&version, &header[sizeof(uint) + sizeof(ushort) * 3], sizeof(ushort));
            memcpy(&reserved, &header[sizeof(uint) + sizeof(ushort) * 4], sizeof(ushort));
#if DEBUG_PACKET != 0
            if (packetId == DEBUG_PACKET) {
                CNLOG("DEBUG_PACKET");
            }
#endif
            if (packetSize <= 0) {
                ConnUtil::log(ConnUtil::format("packetSize exception : %i", packetSize));
                return -1;
            }
            if (inStream->size() < packetSize) { // broken
                ConnUtil::log(ConnUtil::format("Broken package (%i < %i)", inStream->size(), packetSize));
                return 0;
            }
            if (packetId >= PACKET_MAX) {
                ConnUtil::log(ConnUtil::format("packetId out range %i", packetId));
                return -1;
            }
            if (!inStream->skip(PACKET_HEADER_SIZE)) {
                CNLOG("Failed to skip message head");
                return -1;
            }
            packetSize -= PACKET_HEADER_SIZE;
            // NOTE: process message
            packet = INSTANCE(PacketManager)->getPacket(packetId, false);
            if (NULL == packet) {
                CNLOG("Get packet fail");
                return -1;
            }
            packet->receivedPacketId = packetId;
#ifdef XML_CONFIG
            packet->errorId = errorId;
            packet->requestErrMsg = INSTANCE(ConfigError)->getErrorMessage(errorId);
#else
            if (errorId != 0 && errorId < ARRAY_LENGTH(ErrMsg)) {
                packet->requestErrMsg = ErrMsg[errorId].c_str();
            }
#endif
            // body
#if READ_BUFF
            packetBuff->cleanUp();
            inStream->read(packetBuff->getBuffer(), packetSize);
            packetBuff->im_Tail = packetSize;
#endif
#ifdef PACKET_WITH_JSON
#if READ_BUFF
            compression = packetBuff->readInt();
            bRet = packet->setData(packetBuff->readData(packetSize - 4), compression, true);
#else
            compression = inStream->readInt();
            bRet = packet->setData(inStream->readData(packetSize - 4), compression, true);
#endif
#else
#if READ_BUFF
            readSize = packet->read(packetBuff);
            // correction packet size
            if (readSize < packetSize) {
                bRet = true;
            } else {
                bRet = (readSize == packetSize);
            }
#else
            readSize = packet->read(inStream);
            // correction packet size
            if (readSize < packetSize) {
                bRet = inStream->skip(packetSize - fmax(0, readSize));
            } else {
                bRet = (readSize == packetSize);
            }
#endif
#endif
            if (bRet && packet && packetId > 0 &&
                ((currHandle < 0 && (reserved != lastHandle || packetId == PACKET_GET_HANDLE)) || reserved == currHandle)) {
                listPacket.push_back(packet);
                if (currHandle < 0) {
                    currHandle = reserved;
                }
                string log = ConnUtil::format("<< Recv message %i (%i) from [%i] >>", packetId, readSize, reserved);
                ConnUtil::log(log, AppConnect::client()->logToFile);
            }
            if (inStream->size() == 0) {
                break;
            }
        }
        return 1;
    }
    return 1;
}

bool ClientConnect::processPacket()
{
    pthread_mutex_lock(&cnmut);
    list<Packet *>::iterator it = listPacket.begin();
    bool isEmpty = listPacket.empty() || it == listPacket.end();
    pthread_mutex_unlock(&cnmut);

    while (!isEmpty) {
        Packet * packet = (*it);
        // NOTE: process message
        string log = ConnUtil::format("<< Begin process message %i >>", packet->receivedPacketId);
        ConnUtil::log(log, AppConnect::client()->logToFile);
        
        packet->execute();
        
        log = ConnUtil::format("<< E_n_d process message %i >>", packet->receivedPacketId);
        ConnUtil::log(log, AppConnect::client()->logToFile);
        
        pthread_mutex_lock(&cnmut);
        if (!listPacket.empty()) {
            listPacket.pop_front();
        }
        if (!packet->isInstance) {
            deletePacket.push_back(packet);
            while (deletePacket.size() > PACKET_CACHE) {
                list<Packet *>::iterator it = deletePacket.begin();
#if COCOS2D_ENGINE
                (*it)->release();
#else
                delete *it;
#endif
                deletePacket.pop_front();
            }
        }
        it = listPacket.begin();
        isEmpty = listPacket.empty() || it == listPacket.end();
        pthread_mutex_unlock(&cnmut);
    }
    
    return true;
}

bool ClientConnect::sendPacket(Packet * packet)
{
    if (!isConnect) {
        bool ret = false;
        pthread_mutex_lock(&cnmut);
        if (savedPacket.size() < 5) {
            savedPacket.push_back(packet);
            ret = true;
        }
        pthread_mutex_unlock(&cnmut);
        return ret;
    }
#ifdef CHECK_NETWORK
    AppClient * client = AppConnect::client();
    if (netDelay > NET_RECONNECT || (client->uiDelegate && client->uiDelegate->networkEnable())) {
        if (packet->packetId != PACKET_HEARTBEAT && reconnectCount == 0) {
            client->uiDelegate->lostConnect(false);
        }
        return false;
    } else if (netDelay > NET_WARNNING && packet->packetId != PACKET_HEARTBEAT) {
        if (client->uiDelegate) {
            client->uiDelegate->tipNetDelay(netDelay);
        }
    }
#endif
    reconnectCount = 0;
    
    if (currHandle < 0 && packet->packetId != PACKET_GET_HANDLE) {
        AppConnect::server()->sendGetHandle();
        pthread_mutex_lock(&cnmut);
        savedPacket.push_back(packet);
        pthread_mutex_unlock(&cnmut);
        return true;
    }
    // Send packet
    pthread_mutex_lock(&cnmut);
    SocketOutputStream * outStream = NULL;
    if (dataMult == 0) {
        outStream = outputStream;
    } else if (listOutput.size() > 0) {
        list<SocketOutputStream *>::iterator it = listOutput.begin();
        outStream = *it;
    }
    if (outStream == NULL) {
        return false;
    }
    // head
    uint packetSize = 0;
    ushort packetId = 0;
    ushort errorId = 0;
    ushort encrypt = 0;
    ushort version = 0;
    ushort reserved = 0;
    
    packetSize = packet->getPacketSize() + PACKET_HEADER_SIZE;
    packetId = packet->packetId;
    version = PACKET_VERSION;
    reserved = time(0) & 0xffff;
    encrypt = ((packetSize * packetSize) / m_key1
               + (packetId / m_key2) * (packetId / m_key2)
               + (packetSize + packetId) / m_key3
               + version / m_key4 + reserved / m_key5) & 0xffff;
    
    outStream->writeUInt(packetSize);
    outStream->writeUShort(packetId);
    outStream->writeUShort(errorId);
    outStream->writeUShort(encrypt);
    outStream->writeUShort(version);
    outStream->writeUShort(reserved);
    
    // body
    int writeSize = outStream->write(packet);
    
    // correction packet size
    if (packetSize != writeSize + PACKET_HEADER_SIZE) {
        packetSize = writeSize + PACKET_HEADER_SIZE;
        outStream->om_Tail -= packetSize;
        outStream->writeUInt(packetSize);
        outStream->om_Tail += packetSize - sizeof(uint);
    }
    pthread_mutex_unlock(&cnmut);
        
    return true;
}
