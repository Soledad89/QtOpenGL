#pragma once

class VilarEventHandler
{
public:
	VilarEventHandler() {};
	virtual ~VilarEventHandler() {};

	void DealWithEvent( int notifyEvent, void * param = 0, bool isBlockEvent = true );

private:
	virtual void __stdcall DoDealWithEvent( int notifyEvent, void * param, bool isBlockEvent ) = 0;

	static unsigned long __stdcall NotifyThread( void * param );
};
