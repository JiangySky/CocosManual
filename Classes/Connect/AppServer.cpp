//
//  AppServer.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-5.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "AppServer.h"
#include "AppConnect.h"
#include "Packets/PacketAll.h"

#if CPU_OPTIMIZE
static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static bool sendActive = false;
#endif

AppServer::AppServer()
{
    init();
}

AppServer::~AppServer()
{
    stopConnect(true);
}

bool AppServer::init()
{
    sendthread = 0;
    recvthread = 0;
    heartBeatThread = 0;
    processthread = 0;
    
    reconnectCount = 0;
    initConnect();
    if (INSTANCE(ClientConnect)->connected()) {
        sendGetHandle();
    }
    return true;
}

bool AppServer::initOffline()
{
    return true;
}

int AppServer::checkNet()
{
    return NETNOT;
}

static void * handleSendThread(void *)
{
    // pthread_setname_np("Send");
    static int pauseDelay = 10;
    while (true) {
#if CPU_OPTIMIZE
        pthread_mutex_lock(&mut);
        while (!sendActive) {
            CNLOG("=== SendThread pause  ===");
            pthread_cond_wait(&cond, &mut);
        }
        pthread_mutex_unlock(&mut);
#endif
        usleep(0.1 * 1000000);
        int ret = INSTANCE(ClientConnect)->processSend();
#if CPU_OPTIMIZE
        if (ret <= 0) {
            if (--pauseDelay <= 0) {
                pthread_mutex_lock(&mut);
                sendActive = false;
                pthread_mutex_unlock(&mut);
            }
        } else {
            pauseDelay = 10;
        }
#endif
    }
    return NULL;
}
#if CPU_OPTIMIZE
static void * activeSendThread()
{
    if (!sendActive) {
        pthread_mutex_lock(&mut);
        sendActive = true;
        pthread_cond_signal(&cond);
        CNLOG("+++ SendThread resume +++");
        pthread_mutex_unlock(&mut);
    }
    return NULL;
}
#endif
static void * handleRecvThread(void *)
{
    // pthread_setname_np("Recv");
    while (true) {
        usleep(0.1 * 1000000);
        INSTANCE(ClientConnect)->processRecv();
#if defined (PROCESS_ASYN) && CPU_OPTIMIZE == 1
        INSTANCE(ClientConnect)->processCommand();
#endif
    }
    return NULL;
}

static void * startHeartbeat(void *)
{
    // pthread_setname_np("Heartbeat");
    while (true) {
        sleep(HEART_BEAT);
        AppConnect::server()->sendHeartBeat(NULL);
    }
    return NULL;
}

static void * handleProcessThread(void *)
{
    // pthread_setname_np("Process");
    while (true) {
        usleep(0.1 * 1000000);
        INSTANCE(ClientConnect)->processCommand();
    }
    return NULL;
}

void AppServer::sendInMainThread(float)
{
    if (INSTANCE(ClientConnect)->processSend() < 0) {
        CNLOG("Lose connection");
    }
}

void AppServer::recvInMainThread(float)
{
    INSTANCE(ClientConnect)->processRecv();
}

void AppServer::heartbeatInMainThread(float)
{
    AppConnect::server()->sendHeartBeat(NULL);
}

void AppServer::processInMainThread(float)
{
    INSTANCE(ClientConnect)->processCommand();
}

void AppServer::initConnect(ConnetStatus flag)
{
    AppClient * client = AppConnect::client();
#ifdef CHECK_NETWORK
    if (client->uiDelegate && !client->uiDelegate->networkEnable()) {
        client->uiDelegate->lostConnect(true);
        return;
    }
#endif
    if (INSTANCE(ClientConnect)->connectServer(client->ip, client->port, flag)) {
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
        restartThread(false);
#else
        restartThread(true);
#endif
        INSTANCE(ClientConnect)->setConnected(true);
        string connetInfo = ConnUtil::format("*** Connect to %s:%i (socket:%i) ***",
                                             client->ip.c_str(), client->port, INSTANCE(ClientConnect)->m_socket);
        ConnUtil::log(connetInfo, client->logToFile);
        client->reconnectCount = 0;
    } else {
        client->connectFail();
    }
}

void AppServer::restartConnect()
{
    ClientConnect * conn = INSTANCE(ClientConnect);
    if (conn->socketLimit > 0) {
        initConnect();
        if (conn->currentSocket(true) > 1) {
            stopConnect();
        }
    } else {
        stopConnect();
        initConnect();
    }
    if (conn->connected()) {
        sendGetHandle();
    }
}

void AppServer::restartThread(bool reset)
{
    // stop
    if (reset) {
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
        pthread_cancel(sendthread);
        pthread_cancel(recvthread);
        pthread_cancel(processthread);
        pthread_cancel(heartBeatThread);
#else
        if (sendthread != 0) {
            pthread_kill(sendthread, SIGQUIT);
        }
        if (recvthread != 0) {
            pthread_kill(recvthread, SIGQUIT);
        }
        if (processthread != 0) {
            pthread_kill(processthread, SIGQUIT);
        }
        if (heartBeatThread != 0) {
            pthread_kill(heartBeatThread, SIGQUIT);
        }
#endif
#if COCOS2D_ENGINE
        unschedule(schedule_selector(AppServer::processInMainThread));
#if defined(HEARTBEAT_ENABLE) && HEARTBEAT_ENABLE == 1
        unschedule(schedule_selector(AppServer::heartbeatInMainThread));
#endif
#endif
        sendthread = 0;
        recvthread = 0;
        processthread = 0;
        heartBeatThread = 0;
    }
    
    // start
    if (sendthread == 0) {
        pthread_create(&sendthread, NULL, handleSendThread, NULL);
    }
    if (recvthread == 0) {
        pthread_create(&recvthread, NULL, handleRecvThread, NULL);
    }
#ifdef PROCESS_ASYN
#if CPU_OPTIMIZE == 0
    if (processthread == 0) {
        pthread_create(&processthread, NULL, handleProcessThread, NULL);
    }
#endif
#elif COCOS2D_ENGINE
    m_bIsRunning = true;
    unschedule(schedule_selector(AppServer::processInMainThread));
    schedule(schedule_selector(AppServer::processInMainThread), 0.1);
#else
    // other platform or engine
#endif
#if defined(HEARTBEAT_ENABLE) && HEARTBEAT_ENABLE == 1
#ifdef HEARTBEAT_ASYN
    if (heartBeatThread == 0) {
        pthread_create(&heartBeatThread, NULL, startHeartbeat, NULL);
    }
#elif COCOS2D_ENGINE
    unschedule(schedule_selector(AppServer::heartbeatInMainThread));
    schedule(schedule_selector(AppServer::heartbeatInMainThread), HEART_BEAT);
#else
    // other platform or engine
#endif
#endif
}

bool AppServer::checkHeartbeat()
{
    return true;
}

int AppServer::connectOverTime()
{
    return -1;
}

void AppServer::stopConnect(bool resetThread, ConnetStatus flag, bool clear)
{
#if CPU_OPTIMIZE
    activeSendThread();
#endif
    if (resetThread) {
#ifdef __IPHONE_OS_VERSION_MAX_ALLOWED
        pthread_cancel(sendthread);
        pthread_cancel(recvthread);
        pthread_cancel(processthread);
        pthread_cancel(heartBeatThread);
#else
        if (sendthread != 0) {
            pthread_kill(sendthread, SIGQUIT);
        }
        if (recvthread != 0) {
            pthread_kill(recvthread, SIGQUIT);
        }
        if (processthread != 0) {
            pthread_kill(processthread, SIGQUIT);
        }
        if (heartBeatThread != 0) {
            pthread_kill(heartBeatThread, SIGQUIT);
        }
#endif
#if COCOS2D_ENGINE
        unschedule(schedule_selector(AppServer::processInMainThread));
#if defined(HEARTBEAT_ENABLE) && HEARTBEAT_ENABLE == 1
        unschedule(schedule_selector(AppServer::heartbeatInMainThread));
#endif
#endif
        sendthread = 0;
        recvthread = 0;
        processthread = 0;
        heartBeatThread = 0;
    }
    
    INSTANCE(ClientConnect)->closeSocket(flag, clear);
}

bool AppServer::sendRequest(ushort packetId, void * data)
{
    switch (packetId) {
        case PACKET_HEARTBEAT:
            sendHeartBeat(data);
            break;
            
        default:
            break;
    }
    return true;
}

#pragma mark - == Send Packet == -
#pragma mark - Framework send

void AppServer::sendGetHandle()
{
    PACKET(PacketGetHandle, PACKET_GET_HANDLE);
    SEND_PACKET(packet);
}

void AppServer::sendHeartBeat(void * data)
{
    if (AppConnect::client()->heartbeatEnabel) {
        PACKET(PacketHBeat, PACKET_HEARTBEAT);
        SEND_PACKET(packet)
    }
}

void AppServer::sendTest(std::string msg)
{
    PACKET(PacketTest, PACKET_TEST);
#ifdef PACKET_WITH_JSON
    std::string data = "{\"msg\":" + msg + "}";
    packet->setData(data, 0, false);
#else
    packet->msg = msg;
#endif
    SEND_PACKET(packet)
}

void AppServer::sendReconnect(int pid, string pName)
{
    PACKET(PacketReconnect, PACKET_RECONNECT);
    packet->pId = pid;
    packet->pName = pName;
    SEND_PACKET(packet)
}

#pragma mark - Private send



