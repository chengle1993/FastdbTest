#pragma once
class ConnPool;

class ConnectionPoolManager
{
public:
	ConnectionPoolManager();
	~ConnectionPoolManager();
	static ConnectionPoolManager* getInstance();

private:
	static ConnectionPoolManager* singleton;
};

