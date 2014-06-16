////
//  AppClient.cpp
//  NewProject
//
//  Created by Jiangy on 13-1-5.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#include "AppClient.h"
#include "AppConnect.h"

AppClient::AppClient()
{
    this->init();
}

AppClient::~AppClient()
{
    
}

bool AppClient::init()
{
    logToFile = false;
    heartbeatEnabel = false;
    ip = SERVERIP;
    port = SERVERPROT;
    detectionedIP = "";
    detectionedPort = 0;
    serverTipDelay = 0.5;
    expectPacketId = expectRecvId = -1;
    expectSpec = 0;
    uiDelegate = nullptr;
    
    return true;
}

void AppClient::startServer()
{
    AppConnect::server();
}

void AppClient::startServer(string dstIP, int dstPort)
{
    ip = dstIP;
    port = dstPort;
    AppConnect::server();
}

void AppClient::reconnect()
{
    if (++reconnectCount < 5) {
        AppConnect::server()->restartConnect();
    }
}

void AppClient::reconnect(string dstIP, int dstPort, bool mandatory)
{
    if (!mandatory && dstIP.compare(ip) == 0 && dstPort == port) {
        return;
    }
    ip = dstIP;
    port = dstPort;
    if (port == SERVERPROT && ip.compare(SERVERIP) == 0 && detectionedPort > 0) {
        ip = detectionedIP;
        port = detectionedPort;
    }
    if (++reconnectCount < 5) {
        AppConnect::server()->restartConnect();
    }
}

void AppClient::resumeConnect(float time)
{
#ifdef CHECK_NETWORK
    if (uiDelegate && !uiDelegate->networkEnable()) {
        uiDelegate->lostConnect(true);
        return;
    }
#endif
    if (uiDelegate && uiDelegate->isConnectGS()) {
        onReconnectGame();
    } else {
        reconnect();
    }
}

void AppClient::onReconnectGame()
{
    if (uiDelegate) {
        uiDelegate->readyForReconnect();
    }
    reconnect();
}

void AppClient::connectFail(bool isLost)
{
    ConnUtil::log(ConnUtil::format("connectFail (%s:%i)", ip.c_str(), port));
#ifdef DISPATCH_ENABEL
    dispatch_async(dispatch_get_main_queue(), ^{
        if (uiDelegate) {
            if (isLost) {
                uiDelegate->lostConnect(false);
            } else {
                uiDelegate->connectFail(0);
            }
        }
    });
#else
    if (uiDelegate) {
        if (isLost) {
            uiDelegate->lostConnect(false);
        } else {
            uiDelegate->connectFail(0);
        }
    }
#endif
}

void AppClient::setDetectioned(string dstIP, int dstPort)
{
    detectionedIP = dstIP;
    detectionedPort = dstPort;
    ip = dstIP;
    port = dstPort;
}

bool AppClient::onRequest(ushort packetId, void * data)
{
    return AppConnect::server()->sendRequest(packetId, data);
}

void AppClient::onResponse(Packet * packet)
{
    reconnectCount = 0;
    switch (packet->receivedPacketId) {
        case PACKET_ERROR:
            this->onResponseError(packet, ((PacketError *)packet)->msgId);
            break;
        case PACKET_SERVER_TIP:
            this->onResponseServerTip((PacketServerTip *)packet);
            break;
        case PACKET_GET_HANDLE:
        case PACKET_GET_HANDLE_RES:
            this->onGetHandleResponse((PacketGetHandle *)packet);
            break;
        case PACKET_TEST:
            this->onTestResponse((PacketTest *)packet);
            break;
            
        default:
            break;
    }
}

#pragma mark - == Process response single ==

void AppClient::onGetHandleResponse(PacketGetHandle * packet)
{
    ClientConnect * conn = INSTANCE(ClientConnect);
    if (conn->currHandle >= 0) {
        conn->lastHandle = conn->currHandle;
    }
    conn->currHandle = packet->handleId;
    conn->sendSavedPacket();
}

void AppClient::onResponseError(Packet *packet, int msgId)
{
    string log = ConnUtil::format("Error(%i-%i): %s", msgId, packet->errorId, packet->requestErrMsg.c_str());
    ConnUtil::log(log, logToFile);
    
    if (dynamic_cast<PacketError *>(packet)) {
        switch (((PacketError *)packet)->msgId) {
            case PACKET_RECONNECT:
                if (uiDelegate) {
                    uiDelegate->reconnectError(1);
                }
                heartbeatEnabel = false;
                reconnect(SERVERIP, SERVERPROT, true);
                break;
                
            default:
                break;
        }
    }
}

void AppClient::onResponseServerTip(PacketServerTip *packet)
{
    if (uiDelegate && !uiDelegate->needTipServer()) {
        uiDelegate->reconnectError(1);
        packet->tips.clear();
        return;
    }
#if COCOS2D_ENGINE
    unschedule(schedule_selector(AppClient::resetServerTip));
    std::list<ServerTip>::iterator it = packet->tips.begin();
    while (it != packet->tips.end()) {
        if (uiDelegate) {
            uiDelegate->flyTip(it->message, serverTipDelay, it->tipType);
        }
        serverTipDelay += 0.5;
        packet->tips.pop_front();
        it = packet->tips.begin();
    }
    scheduleOnce(schedule_selector(AppClient::resetServerTip), serverTipDelay);
#endif
}
#if COCOS2D_ENGINE
void AppClient::resetServerTip(float time)
{
    serverTipDelay = 0.5;
}
#endif

void AppClient::onTestResponse(PacketTest *packet)
{
    
}


