//
//  AppClient.h
//  NewProject
//
//  Created by Jiangy on 13-1-5.
//  Copyright (c) 2013 35VI. All rights reserved.
//

#ifndef __NewProject__AppClient__
#define __NewProject__AppClient__

#include <iostream>
#include "ConnectDefine.h"
#include "Packets/PacketAll.h"

#if COCOS2D_ENGINE
using namespace std;
class AppClient : public cocos2d::Node
#else
using namespace std;
class AppClient
#endif
{
    
public:
    ConnUIDelegate * uiDelegate;
    bool logToFile, heartbeatEnabel;
    string ip, detectionedIP;
    int port, detectionedPort;
    float serverTipDelay;
    int expectPacketId, expectRecvId, expectSpec;
    int reconnectCount;
    
    AppClient();
    virtual ~AppClient();
    virtual bool init();

    void startServer();
    void startServer(string dstIP, int dstPort);
    void reconnect();
    void reconnect(string dstIP, int dstPort, bool mandatory = false);
    void connectFail(bool isLost = false);
    void setDetectioned(string dstIP, int dstPort);
    void resumeConnect(float t = 0);
    void onReconnectGame();
    
    /**
     @brief     do some request in main thread
     @param     packetId : ushort
     @param     data : void *
     @return    state
     */
    bool onRequest(ushort packetId, void * data);
    
    /**
     @brief     process response in main thread
     @param     packet : Packet
     */
    void onResponse(Packet * packet);
    
    /**
     @brief     process error response in main thread
     @param     packet : Packet
     */
    void onResponseError(Packet * packet, int msgId = -1);
    
    /**
     @brief     process server tip response in main thread
     @param     packet : PacketServerTip
     */
    void onResponseServerTip(PacketServerTip * packet);
#if COCOS2D_ENGINE
    void resetServerTip(float t = 0);
#endif
   
#pragma mark - Framework response
    void onGetHandleResponse(PacketGetHandle * packet);
    void onTestResponse(PacketTest * packet);
    
#pragma mark - Private response
    
    
};

#endif /* defined(__NewProject__AppClient__) */
