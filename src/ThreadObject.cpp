#include "ThreadObject.h"

ThreadedObject::ThreadedObject() 
{
  connectTime = 0;
  deltaTime = 0;
  msgTx = "";
  msgRx = "";
}

ThreadedObject::~ThreadedObject() {}


void ThreadedObject::setup() 
{ 
	start();
}

void ThreadedObject::update()
{}

void ThreadedObject::draw() {}

void ThreadedObject::exit() 
{ 
	stop();
    waitForThread(true);
}


void ThreadedObject::start()
{ 
	startThread(); 
}

void ThreadedObject::stop()
{
	std::unique_lock<std::mutex> lck(mutex);
	stopThread();
	condition.notify_all();
}

void ThreadedObject::threadedFunction()
{
    while (isThreadRunning()) {
        if (tcpClient.isConnected()) {
            string str = tcpClient.receive();
            if (str.length() > 0) {
                std::unique_lock<std::mutex> lock(mutex);
                msgRx = str;
                condition.wait(lock);
            } 
            std::unique_lock<std::mutex> lock(mutex);
            if (!msgTx.empty()) {
                tcpClient.send(msgTx);
                msgTx.clear();
            condition.wait(lock);
            }
        } else {
            std::unique_lock<std::mutex> lock(mutex);
            msgTx = "";
            condition.wait(lock);
            deltaTime = ofGetElapsedTimeMillis() - connectTime;
            if (deltaTime > 5000) {
                ofLogNotice() << "recoonect";
                tcpClient.setup("127.0.0.1", 11999);
                tcpClient.setMessageDelimiter("\n");
                connectTime = ofGetElapsedTimeMillis();
            }
        }
    }
}