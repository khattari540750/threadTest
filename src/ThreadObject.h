#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"



class ThreadedObject : public ofThread
{
public:

public:
	ThreadedObject();
	~ThreadedObject();

public:
	void setup();
	void update();
    void draw();
    void exit();

private:
    void start();
	void stop();
    void threadedFunction();

private:
    std::condition_variable condition;
	ofxTCPClient tcpClient;
    string msgTx, msgRx;
    int connectTime;
    int deltaTime;
};